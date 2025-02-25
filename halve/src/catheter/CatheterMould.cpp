#include "CatheterMould.h"
#include <QJsonDocument>
#include <qjsonobject.h>
#include <QJsonArray>
#include <QJsonValue>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyLine.h>
#include <vtkUnstructuredGrid.h>
#include <vtkParametricSpline.h>
#include <vtkParametricFunctionSource.h>
#include <vtkTubeFilter.h>
#include <vtkAppendFilter.h>
#include <QResource>
#include <vtkDataSetMapper.h>
#include <HalveType.h>
#include <QFile>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

#include <utility/ModelCache.h>
#include <utility/Thread.h>
#include <utility/VtkUtil.h>
#include "utility/FileUtils.h"

#include <QColor>
#include <vtkStringArray.h>

const char *TypesPointDataName = "types";
const char *PointsPointDataName = "points";
const char *ConnectivityPointDataName = "connectivity";
const char *ColorsPointDataName = "colors";
const char *ColorNamesPointDataName = "colorNames";
const char* PointIdScalarName = "ids";

CatheterMould::CatheterMould(const QString &meshName, const QList<quint16> &gap, QObject *parent)
    : QObject{parent}
{
    Q_ASSERT(Thread::currentlyOn(Thread::UI));
    m_grid = vtkSmartPointer<vtkUnstructuredGrid>::New();
    load(meshName, gap);
}


void CatheterMould::load(const QString &meshName, const QList<quint16> &gap) {
    m_meshName = meshName;
    m_gap = gap;
    reload();
}

void CatheterMould::reload() {
    Q_ASSERT(Thread::currentlyOn(Thread::UI));
    Q_ASSERT(m_grid != nullptr);
    if (m_meshName.isEmpty()) {
        makeDefultGrid(m_gap);
    } else {
        makeGrid(m_meshName);
    }
    emit changed();
}

vtkSmartPointer<vtkPoints> CatheterMould::makeLinnerPoints(const QList<quint16> &gap) {
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    points->InsertNextPoint(0, 0, 0);
    quint16 step = 0;
    for(quint16 v:gap) {
        step += v;
        points->InsertNextPoint(0, step, 0);
    }
    vtkNew<vtkPolyData> polyData;
    polyData->SetPoints(points);
    vtkNew<vtkTransform> transform;
    transform->Translate(0, -(step / 2), 0);
    vtkNew<vtkTransformPolyDataFilter> transformFilter;
    transformFilter->SetInputData(polyData);
    transformFilter->SetTransform(transform);
    transformFilter->Update();
    return transformFilter->GetOutput()->GetPoints();
}

void CatheterMould::makeDefultGrid(const QList<quint16> &gap) {
    Q_ASSERT(Thread::currentlyOn(Thread::UI));
    vtkNew<vtkPolyLine> polyLine;
    vtkSmartPointer<vtkIntArray> types = vtkSmartPointer<vtkIntArray>::New();
    types->SetName(TypesPointDataName);
    types->SetNumberOfComponents(1);
    vtkSmartPointer<vtkUnsignedCharArray> colors = vtkSmartPointer<vtkUnsignedCharArray>::New();
    colors->SetName(ColorsPointDataName);
    colors->SetNumberOfComponents(3);
    vtkSmartPointer<vtkStringArray> colorNames = vtkSmartPointer<vtkStringArray>::New();
    colorNames->SetName(ColorNamesPointDataName);

    vtkSmartPointer<vtkPoints> points = makeLinnerPoints(gap);
    vtkStdString msgColorName = ModelCache::instance()->colorName(Halve::CET_MAG);
    vtkColor3ub magColor = ModelCache::instance()->color3ub(msgColorName);
    for (qsizetype i = 0; i < points->GetNumberOfPoints(); ++i) {
        polyLine->GetPointIds()->InsertNextId(i);
        types->InsertNextTuple1(Halve::CET_MAG);
        colorNames->InsertNextValue(msgColorName);
        colors->InsertNextTypedTuple(magColor.GetData());
    }
    vtkStdString handleColorName = ModelCache::instance()->colorName(Halve::CET_HANDLE);
    vtkIdType pointId = points->GetNumberOfPoints() - 1;
    colors->SetTypedTuple(pointId, ModelCache::instance()->color3ub(handleColorName).GetData());
    colorNames->SetValue(pointId, handleColorName);

    vtkNew<vtkCellArray> cells;
    cells->InsertNextCell(polyLine);
    m_grid->SetPoints(points);
    m_grid->SetCells(VTK_POLY_LINE, cells);
    m_grid->GetPointData()->SetScalars(types);
    m_grid->GetPointData()->AddArray(colors);
    m_grid->GetPointData()->AddArray(colorNames);
}

QByteArray CatheterMould::loadMeshFile(const QString &meshName) {
    QString meshFile = file_utils::getCatheterMeshPath(meshName);
    QFile f(meshFile);
    if (f.open(QIODevice::ReadOnly)) {
        return f.readAll();
    }
    return QResource(QString(":/assets/model/%1").arg(meshName)).uncompressedData();
}

void CatheterMould::makeGrid(const QString &meshName) {
    QJsonDocument json = QJsonDocument::fromJson(meshName.toLatin1());
    if (json.isObject()) {
        fromJson(json.object());
    } else {
        QByteArray data = loadMeshFile(meshName);
        fromJson(QJsonDocument::fromJson(data.data()).object());
    }
}

void CatheterMould::toJson(QJsonObject &meshJson) const {
    Q_ASSERT(m_grid != nullptr);
    QJsonArray pointsJson;
    for(vtkIdType i = 0; i < m_grid->GetNumberOfPoints(); ++i) {
        QJsonArray pointJson;
        double p[3]{};
        m_grid->GetPoint(i, p);
        pointJson.append(p[0]);
        pointJson.append(p[1]);
        pointJson.append(p[2]);
        pointsJson.append(pointJson);
    }
    meshJson[PointsPointDataName] = pointsJson;

    QJsonArray connectivitysJson;
    for(vtkIdType i = 0; i < m_grid->GetNumberOfCells(); ++i) {
        vtkPolyLine *line = dynamic_cast<vtkPolyLine*>(m_grid->GetCell(i));
        QJsonArray connectivityJson;
        for(int j = 0; j < line->GetNumberOfPoints(); ++j) {
            connectivityJson.append(line->GetPointIds()->GetId(j));
        }
        connectivitysJson.append(connectivityJson);
    }
    meshJson[ConnectivityPointDataName] = connectivitysJson;

    auto types = m_grid->GetPointData()->GetArray(TypesPointDataName);
    vtkIdType numberPoints = types->GetNumberOfTuples();
    QJsonArray typesJson;
    for(vtkIdType i = 0; i < numberPoints; ++i) {
        typesJson.append(types->GetTuple1(i));
    }
    meshJson[TypesPointDataName] = typesJson;

    vtkStringArray *colorNames = dynamic_cast<vtkStringArray*>(m_grid->GetPointData()->GetAbstractArray(ColorNamesPointDataName));
    QJsonArray colorsNameJson;
    for(vtkIdType i = 0; i < colorNames->GetNumberOfTuples(); ++i) {
        colorsNameJson.append(colorNames->GetValue(i).c_str());
    }
    meshJson[ColorNamesPointDataName] = colorsNameJson;
}

void CatheterMould::fromJson(const QJsonObject &meshJson) {
    Q_ASSERT(Thread::currentlyOn(Thread::UI));
    Q_ASSERT(m_grid != nullptr);
    vtkNew<vtkPoints> points;
    if (meshJson.contains(PointsPointDataName)) {
        QJsonArray pointsJson = meshJson[PointsPointDataName].toArray();
        for(qsizetype i = 0; i < pointsJson.size(); ++i) {
            QJsonArray pointJson = pointsJson[i].toArray();
            points->InsertNextPoint(pointJson[0].toDouble(), pointJson[1].toDouble(), pointJson[2].toDouble());
        }
    }
    m_grid->SetPoints(points);

    vtkNew<vtkCellArray> cells;
    if (meshJson.contains(ConnectivityPointDataName)) {
        QJsonArray connectivitysJson = meshJson[ConnectivityPointDataName].toArray();
        for(qsizetype i = 0; i < connectivitysJson.size(); ++i) {
            vtkNew<vtkPolyLine> polyLine;
            QJsonArray connectivityJson = connectivitysJson[i].toArray();
            auto pointIds = polyLine->GetPointIds();
            for(qsizetype j  = 0; j < connectivityJson.size(); ++j) {
                pointIds->InsertNextId(connectivityJson[j].toInt());
            }
            cells->InsertNextCell(polyLine);
        }
    }
    m_grid->SetCells(VTK_POLY_LINE, cells);

    vtkSmartPointer<vtkIntArray> types = vtkSmartPointer<vtkIntArray>::New();
    types->SetName(TypesPointDataName);
    types->SetNumberOfComponents(1);
    types->SetNumberOfTuples(points->GetNumberOfPoints());
    for(qsizetype i = 0; i < points->GetNumberOfPoints(); ++i) {
        types->SetTuple1(i, Halve::CatheterElectrodeType::CET_MAG);
    }
    if (meshJson.contains(TypesPointDataName)) {
        QJsonArray scalarsJson = meshJson[TypesPointDataName].toArray();
        auto cnt = std::min(scalarsJson.size(), points->GetNumberOfPoints());
        for(qsizetype i = 0; i < cnt; ++i) {
            types->SetTuple1(i, scalarsJson[i].toInt());
        }
    }
    m_grid->GetPointData()->SetScalars(types);

    vtkSmartPointer<vtkStringArray> colorNames = vtkSmartPointer<vtkStringArray>::New();
    colorNames->SetName(ColorNamesPointDataName);
    vtkStdString msgColorName = ModelCache::instance()->colorName(Halve::CET_MAG);
    for (qsizetype i = 0; i < points->GetNumberOfPoints(); ++i) {
        colorNames->InsertNextValue(msgColorName);
    }
    if (meshJson.contains(ColorNamesPointDataName)) {
        QJsonArray colorNamesJson = meshJson[ColorNamesPointDataName].toArray();
        for(qsizetype i = 0; i < colorNamesJson.size(); ++i) {
            colorNames->SetValue(i, colorNamesJson[i].toString().toStdString());
        }
    }
    m_grid->GetPointData()->AddArray(colorNames);

    vtkSmartPointer<vtkUnsignedCharArray> colors = vtkSmartPointer<vtkUnsignedCharArray>::New();
    colors->SetName(ColorsPointDataName);
    colors->SetNumberOfComponents(3);
    for(qsizetype i = 0; i < points->GetNumberOfPoints(); ++i) {
        auto colorName =  ModelCache::instance()->colorName((Halve::CatheterElectrodeType)types->GetTuple1(i));
        colors->InsertNextTypedTuple(ModelCache::instance()->color3ub(colorName).GetData());
    }
    if (colorNames->GetNumberOfTuples() > 0) {
        auto cnt = std::min(colorNames->GetNumberOfTuples(), points->GetNumberOfPoints());
        for(qsizetype i = 0; i < cnt; ++i) {
            const vtkStdString &colorName = colorNames->GetValue(i);
            if (!colorName.empty()) {
                vtkColor3ub color3ub = vtkutil::qColorToVtkColor3ub(QColor::fromString(colorName));
                colors->SetTypedTuple(i, color3ub.GetData());
            }
        }
    }
    m_grid->GetPointData()->AddArray(colors);
}

vtkSmartPointer<vtkUnstructuredGrid> CatheterMould::grid(bool deepCopy) {
    Q_ASSERT(m_grid != nullptr);
    vtkSmartPointer<vtkUnstructuredGrid> grid = m_grid;
    if (deepCopy) {
        grid = vtkSmartPointer<vtkUnstructuredGrid>::New();
        grid->DeepCopy(m_grid);
    }
    return grid;
}
Halve::CatheterElectrodeType CatheterMould::getType(vtkIdType id) const {
    Q_ASSERT(m_grid != nullptr);
    Q_ASSERT(m_grid->GetNumberOfPoints() > id);
    return (Halve::CatheterElectrodeType)getTypeData()->GetTuple1(id);
}

vtkDataArray* CatheterMould::getTypeData() const {
    return m_grid->GetPointData()->GetArray(TypesPointDataName);
}
