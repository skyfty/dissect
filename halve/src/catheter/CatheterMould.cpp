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
#include <vtkPLYReader.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

#include <utility/ModelCache.h>
#include <utility/Thread.h>
#include <utility/VtkUtil.h>
#include "utility/FileUtils.h"
#include <vtkCellData.h>

#include <QColor>
#include <vtkStringArray.h>
#include <vtkDoubleArray.h>
#include "catheter/CatheterPerception.h"

const char *TypesPointDataName = "types";
const char *PointsPointDataName = "points";
const char *ConnectivityPointDataName = "connectivity";
const char *ColorsPointDataName = "colors";
const char *ColorNamesPointDataName = "colorNames";
const char* PointIdScalarName = "ids";
const char* PerceptionsPointDataName = "perception";
const char* MeshFileName = "file";
const char* SplinePointId = "SplinePointId";
const char* SplineDu = "SplineDu";
const char* DefaultMeshFile = "catheter_node.ply";
const char* NodeMeshName = "nodeMeshName";
const char* NodeMeshIndexName = "nodeMeshIndex";
const char* ConnectivityRadiusName = "connectivityRadius";
const char* TubeSidesName = "sides";
const char* FlexibilityName = "flexibility";
const char* ResolutionName = "resolution";
const char* PointDirectionName = "Direction";

CatheterMould::CatheterMould(const QString &meshName, const QList<quint16> &gap, double flexibility, QObject *parent)
    : QObject{parent}
{
    Q_ASSERT(Thread::currentlyOn(Thread::UI));
    load(meshName, gap, flexibility);
}


void CatheterMould::load(const QString &meshName, const QList<quint16> &gap, double flexibility) {
    m_meshName = meshName;
    m_gap = gap;
    m_flexibility = flexibility;
    reload();
}

void CatheterMould::reload() {
    Q_ASSERT(Thread::currentlyOn(Thread::UI));
    m_meshPolyDatas.clear();
    m_perceptions.clear();
    m_grid = vtkSmartPointer<vtkUnstructuredGrid>::New();

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
vtkIdType CatheterMould::addNodeMesh(const QString& meshPath) {
    std::string content = loadMeshFile(meshPath).toStdString();
    if (content.empty()) {
        return -1;
    }
    vtkNew<vtkPLYReader> reader;
    reader->SetInputString(content);
    reader->SetReadFromInputString(true);
    reader->Update();
    auto polyData = reader->GetOutput();
    if (polyData->GetNumberOfCells() == 0) {
        return -1;
    }
    m_meshPolyDatas.append(std::make_pair(meshPath, polyData));
	return m_meshPolyDatas.size() - 1;
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
    vtkSmartPointer<vtkDoubleArray> nodeFlexibility = vtkSmartPointer<vtkDoubleArray>::New();
    nodeFlexibility->SetName(FlexibilityName);
    nodeFlexibility->SetNumberOfComponents(1);

    vtkSmartPointer<vtkPoints> points = makeLinnerPoints(gap);
    vtkStdString msgColorName = ModelCache::instance()->colorName(Halve::CET_MAG);
    vtkColor3ub magColor = ModelCache::instance()->color3ub(msgColorName);
    for (qsizetype i = 0; i < points->GetNumberOfPoints(); ++i) {
        polyLine->GetPointIds()->InsertNextId(i);
        types->InsertNextTuple1(Halve::CET_MAG);
        nodeFlexibility->InsertNextTuple1(m_flexibility);
        colorNames->InsertNextValue(msgColorName);
        colors->InsertNextTypedTuple(magColor.GetData());
    }
    vtkSmartPointer<vtkIntArray> nodeMesh = vtkSmartPointer<vtkIntArray>::New();
    nodeMesh->SetName(NodeMeshIndexName);
    nodeMesh->SetNumberOfComponents(1);
    nodeMesh->SetNumberOfTuples(points->GetNumberOfPoints());
    nodeMesh->FillTypedComponent(0, addNodeMesh(DefaultMeshFile));

    vtkSmartPointer<vtkIntArray> perceptions = vtkSmartPointer<vtkIntArray>::New();
    perceptions->SetName(PerceptionsPointDataName);
    perceptions->SetNumberOfComponents(1);
    m_perceptions.resize(points->GetNumberOfPoints());
    for (qsizetype i = 0; i < m_perceptions.size(); ++i) {
        vtkSmartPointer<CatheterPerception> perc = vtkSmartPointer<CatheterPerception>::New();
        perc->addSpline(i);
		m_perceptions[i] = perc;
        perceptions->InsertNextTuple1(i);
    }
    vtkStdString handleColorName = ModelCache::instance()->colorName(Halve::CET_HANDLE);
    vtkIdType pointId = points->GetNumberOfPoints() - 1;
    colors->SetTypedTuple(pointId, ModelCache::instance()->color3ub(handleColorName).GetData());
    colorNames->SetValue(pointId, handleColorName);

    vtkNew<vtkCellArray> cells;
    cells->InsertNextCell(polyLine);
    m_grid->SetCells(VTK_POLY_LINE, cells);
    vtkSmartPointer<vtkDoubleArray> connectivityRadius = vtkSmartPointer<vtkDoubleArray>::New();
    connectivityRadius->SetName(ConnectivityRadiusName);
    connectivityRadius->SetNumberOfComponents(1);
    connectivityRadius->InsertNextTuple1(CatheterTubeRadius);
    m_grid->GetCellData()->AddArray(connectivityRadius);

    vtkNew<vtkIntArray> tubeSides;
    tubeSides->SetName(TubeSidesName);
    tubeSides->SetNumberOfComponents(1);
    tubeSides->InsertNextTuple1(DefaultTubeSide);
    m_grid->GetCellData()->AddArray(tubeSides);

    vtkNew<vtkIntArray> tubeResolution;
    tubeResolution->SetName(ResolutionName);
    tubeResolution->SetNumberOfComponents(1);
    tubeResolution->InsertNextTuple1(DefaultTubeResolution);
    m_grid->GetCellData()->AddArray(tubeResolution);

    m_grid->SetPoints(points);
    m_grid->GetPointData()->SetScalars(types);
    m_grid->GetPointData()->AddArray(colors);
    m_grid->GetPointData()->AddArray(colorNames);
    m_grid->GetPointData()->AddArray(perceptions);
    m_grid->GetPointData()->AddArray(nodeMesh);
    m_grid->GetPointData()->AddArray(nodeFlexibility);
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
QList<vtkSmartPointer<vtkPolyData>> CatheterMould::getNodePolyDatas() const {
    QList<vtkSmartPointer<vtkPolyData>> polyDatas;
    for (const auto& meshPolyData : m_meshPolyDatas) {
        polyDatas.append(meshPolyData.second);
    }
    return polyDatas;
}
void CatheterMould::toJson(QJsonObject &meshJson) const {
    Q_ASSERT(m_grid != nullptr);
    
    QJsonArray meshPolyDatasJson;
	for (const auto& pair : m_meshPolyDatas) {
		meshPolyDatasJson.append(pair.first);
	}
    meshJson[NodeMeshName] = meshPolyDatasJson;
    
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
    QJsonArray connectivitysRadiusJson;
    vtkSmartPointer<vtkDoubleArray> connectivityRadius = vtkDoubleArray::SafeDownCast(m_grid->GetCellData()->GetArray(ConnectivityRadiusName));
    for(vtkIdType i = 0; i < m_grid->GetNumberOfCells(); ++i) {
        vtkPolyLine *line = dynamic_cast<vtkPolyLine*>(m_grid->GetCell(i));
        QJsonArray connectivityJson;
        for(int j = 0; j < line->GetNumberOfPoints(); ++j) {
            connectivityJson.append(line->GetPointIds()->GetId(j));
        }
        connectivitysRadiusJson.append(connectivityRadius->GetTuple1(i));
        connectivitysJson.append(connectivityJson);
    }
    meshJson[ConnectivityPointDataName] = connectivitysJson;
    meshJson[ConnectivityRadiusName] = connectivitysRadiusJson;

    vtkSmartPointer<vtkIntArray> tubeSides = vtkIntArray::SafeDownCast(m_grid->GetCellData()->GetArray(TubeSidesName));
    if (tubeSides->GetNumberOfValues() == 1) {
        meshJson[TubeSidesName] = tubeSides->GetValue(0);
    } else {
        QJsonArray tubeSidesJson;
        for (vtkIdType i = 0; i < tubeSides->GetNumberOfValues(); ++i) {
            tubeSidesJson.append(tubeSides->GetValue(i));
        }
        meshJson[TubeSidesName] = tubeSidesJson;
    }

    vtkSmartPointer<vtkIntArray> tubeResolutions = vtkIntArray::SafeDownCast(m_grid->GetCellData()->GetArray(ResolutionName));
    if (tubeResolutions->GetNumberOfValues() == 1) {
        meshJson[ResolutionName] = tubeResolutions->GetValue(0);
    } else {
        QJsonArray tubeResolutionsJson;
        for (vtkIdType i = 0; i < tubeResolutions->GetNumberOfValues(); ++i) {
            tubeResolutionsJson.append(tubeResolutions->GetValue(i));
        }
        meshJson[ResolutionName] = tubeResolutionsJson;
    }

    auto nodeMeshIndex = m_grid->GetPointData()->GetArray(NodeMeshIndexName);
    QJsonArray nodeMeshIndexJson;
    for (vtkIdType i = 0; i < nodeMeshIndex->GetNumberOfTuples(); ++i) {
        nodeMeshIndexJson.append(nodeMeshIndex->GetTuple1(i));
    }
    meshJson[NodeMeshIndexName] = nodeMeshIndexJson;

    auto nodeFlexibility = m_grid->GetPointData()->GetArray(FlexibilityName);
    QJsonArray nodeFlexibilityJson;
    for (vtkIdType i = 0; i < nodeFlexibility->GetNumberOfTuples(); ++i) {
        nodeFlexibilityJson.append(nodeFlexibility->GetTuple1(i));
    }
    meshJson[FlexibilityName] = nodeFlexibilityJson;

    auto types = m_grid->GetPointData()->GetArray(TypesPointDataName);
    vtkIdType numberPoints = types->GetNumberOfTuples();
    QJsonArray typesJson;
    for(vtkIdType i = 0; i < numberPoints; ++i) {
        typesJson.append(types->GetTuple1(i));
    }
    meshJson[TypesPointDataName] = typesJson;

    QJsonArray perceptionsJson;
    for (size_t i = 0; i < m_perceptions.size(); i++) {
		if (m_perceptions[i] != nullptr) {
			QJsonObject perceptionJson;
			m_perceptions[i]->ToJson(perceptionJson);
			perceptionsJson.append(perceptionJson);
		}
    }
    meshJson[PerceptionsPointDataName] = perceptionsJson;

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

    if (meshJson.contains(NodeMeshName)) {
        QJsonArray meshPolyDatasJson = meshJson[NodeMeshName].toArray();
        for (qsizetype i = 0; i < meshPolyDatasJson.size(); ++i) {
            addNodeMesh(meshPolyDatasJson[i].toString());
        }
    } else {
        addNodeMesh(DefaultMeshFile);
    }

    vtkNew<vtkPoints> points;
    if (meshJson.contains(PointsPointDataName)) {
        QJsonArray pointsJson = meshJson[PointsPointDataName].toArray();
        for(qsizetype i = 0; i < pointsJson.size(); ++i) {
            QJsonArray pointJson = pointsJson[i].toArray();
            vtkVector3d pos(pointJson[0].toDouble(), pointJson[1].toDouble(), pointJson[2].toDouble());
            points->InsertNextPoint(pos.GetData());
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

    vtkNew<vtkDoubleArray> connectivityRadius;
    connectivityRadius->SetName(ConnectivityRadiusName);
    connectivityRadius->SetNumberOfComponents(1);
    connectivityRadius->SetNumberOfTuples(cells->GetNumberOfCells());
    for (qsizetype i = 0; i < cells->GetNumberOfCells(); ++i) {
        connectivityRadius->SetTuple1(i, CatheterTubeRadius);
    }
    if (meshJson.contains(ConnectivityRadiusName)) {
        QJsonArray connectivitysJson = meshJson[ConnectivityRadiusName].toArray();
        for (qsizetype i = 0; i < connectivitysJson.size(); ++i) {
            connectivityRadius->SetTuple1(i, connectivitysJson[i].toDouble());
        }
    }
    m_grid->GetCellData()->AddArray(connectivityRadius);


    vtkNew<vtkIntArray> tubeSides;
    tubeSides->SetName(TubeSidesName);
    tubeSides->SetNumberOfComponents(1);
    tubeSides->SetNumberOfTuples(cells->GetNumberOfCells());
    for (qsizetype i = 0; i < cells->GetNumberOfCells(); ++i) {
        tubeSides->SetTuple1(i, DefaultTubeSide);
    }
    if (meshJson.contains(TubeSidesName)) {
        if (meshJson[TubeSidesName].isDouble()) {
            int tubeSidesValue = meshJson[TubeSidesName].toInt();
            for (qsizetype i = 0; i < cells->GetNumberOfCells(); ++i) {
                tubeSides->SetTuple1(i, tubeSidesValue);
            }
        } else {
            QJsonArray tubeSidesJson = meshJson[TubeSidesName].toArray();
            for (qsizetype i = 0; i < tubeSidesJson.size(); ++i) {
                tubeSides->SetTuple1(i, tubeSidesJson[i].toDouble());
            }
        }
    }
    m_grid->GetCellData()->AddArray(tubeSides);


    vtkNew<vtkIntArray> resolution;
    resolution->SetName(ResolutionName);
    resolution->SetNumberOfComponents(1);
    resolution->SetNumberOfTuples(cells->GetNumberOfCells());
    for (qsizetype i = 0; i < cells->GetNumberOfCells(); ++i) {
        resolution->SetTuple1(i, DefaultTubeResolution);
    }
    if (meshJson.contains(ResolutionName)) {
        if (meshJson[ResolutionName].isDouble()) {
            int resolutionValue = meshJson[ResolutionName].toInt();
            for (qsizetype i = 0; i < cells->GetNumberOfCells(); ++i) {
                resolution->SetTuple1(i, resolutionValue);
            }
        } else {
            QJsonArray resolutionJson = meshJson[ResolutionName].toArray();
            for (qsizetype i = 0; i < resolutionJson.size(); ++i) {
                resolution->SetTuple1(i, resolutionJson[i].toDouble());
            }
        }
    }
    m_grid->GetCellData()->AddArray(resolution);

    vtkSmartPointer<vtkIntArray> nodeMeshIndex = vtkSmartPointer<vtkIntArray>::New();
    nodeMeshIndex->SetName(NodeMeshIndexName);
    nodeMeshIndex->SetNumberOfComponents(1);
    nodeMeshIndex->SetNumberOfTuples(points->GetNumberOfPoints());
    for (qsizetype i = 0; i < points->GetNumberOfPoints(); ++i) {
        nodeMeshIndex->SetTuple1(i, 0);
    }
    if (meshJson.contains(NodeMeshIndexName)) {
        QJsonArray nodeMeshIndexJson = meshJson[NodeMeshIndexName].toArray();
        for (qsizetype i = 0; i < points->GetNumberOfPoints(); ++i) {
            nodeMeshIndex->SetTuple1(i, nodeMeshIndexJson[i].toInt());
        }
    }
	m_grid->GetPointData()->AddArray(nodeMeshIndex);

    vtkSmartPointer<vtkDoubleArray> nodeFlexibility = vtkSmartPointer<vtkDoubleArray>::New();
    nodeFlexibility->SetName(FlexibilityName);
    nodeFlexibility->SetNumberOfComponents(1);
    nodeFlexibility->SetNumberOfTuples(points->GetNumberOfPoints());
    for (qsizetype i = 0; i < points->GetNumberOfPoints(); ++i) {
        nodeFlexibility->SetTuple1(i, m_flexibility);
    }
    if (meshJson.contains(FlexibilityName)) {
        QJsonArray nodeFlexibilityJson = meshJson[FlexibilityName].toArray();
        for (qsizetype i = 0; i < points->GetNumberOfPoints(); ++i) {
            nodeFlexibility->SetTuple1(i, nodeFlexibilityJson[i].toDouble());
        }
    }
    m_grid->GetPointData()->AddArray(nodeFlexibility);

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

    vtkSmartPointer<vtkIntArray> perceptionsIdx = vtkSmartPointer<vtkIntArray>::New();
    perceptionsIdx->SetName(PerceptionsPointDataName);
    perceptionsIdx->SetNumberOfComponents(1);
    perceptionsIdx->SetNumberOfTuples(points->GetNumberOfPoints());
    m_perceptions.resize(points->GetNumberOfPoints());
    for (qsizetype i = 0; i < points->GetNumberOfPoints(); ++i) {
        m_perceptions[i] = vtkSmartPointer<CatheterPerception>::New();
		m_perceptions[i]->addSpline(i);
        perceptionsIdx->SetTuple1(i, 0);
    }
    if (meshJson.contains(PerceptionsPointDataName)) {
        QJsonArray scalarsJson = meshJson[PerceptionsPointDataName].toArray();
        auto cnt = std::min(scalarsJson.size(), points->GetNumberOfPoints());
        for (qsizetype i = 0; i < cnt; ++i) {
			perceptionsIdx->SetTuple1(i, i);
            m_perceptions[i]->FromJson(scalarsJson[i].toObject());
        }
    }
    m_grid->GetPointData()->AddArray(perceptionsIdx);

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

vtkSmartPointer<CatheterPerception> CatheterMould::getPerception(vtkIdType id) {
	if (id >= 0 && id < m_perceptions.size()) {
		return m_perceptions[id];
	}
    return nullptr;
}
