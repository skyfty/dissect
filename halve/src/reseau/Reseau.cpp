#include <vtkImplicitPolyDataDistance.h>
#include <QJsonDocument>
#include <qjsonobject.h>
#include <QJsonArray>
#include <QDir>
#include <vtkIdList.h>
#include <utility/Thread.h>
#include "Reseau.h"
#include "vtkAbstractCellLocator.h"
#include "vtkAbstractPointLocator.h"
#include <vtkKdTree.h>
#include <vtkPolyData.h>
#include <vtkVector.h>
#include <QtConcurrent>
#include <vtkXMLPolyDataWriter.h>
#include <vtkXMLPolyDataReader.h>
#include <utility/VtkUtil.h>

using namespace std::placeholders;
Reseau::Reseau(const QString &path,QObject *parent)
    : QObject{parent},m_path(path)
{
}

void Reseau::fromJson(const QJsonObject &json) {
    if (json.contains("id"))
        m_id = json["id"].toInteger();
    if (json.contains("name"))
        m_name = json["name"].toString();
    if (json.contains("apparent"))
        m_apparent = json["apparent"].toBool();
    if (json.contains("buildin"))
        m_buildin = json["buildin"].toBool();
    if (json.contains("dyestuff"))
        m_dyestuff = QColor::fromString(json["dyestuff"].toString());
    if (json.contains("pointNumber"))
        m_pointNumber = json["pointNumber"].toInteger();

}

void Reseau::toJson(QJsonObject &json) const {
    json["id"] = m_id;
    json["name"] = m_name;
    json["apparent"] = m_apparent;
    json["buildin"] = m_buildin;
    json["dyestuff"] = m_dyestuff.name();
    json["pointNumber"] = m_pointNumber;
}

void Reseau::clean() {
    m_meshPointIds = nullptr;
    m_polyData = nullptr;
    m_kdTree = nullptr;
    m_implicitPolyDataDistance = nullptr;
    QFile::remove(idsFileName());
    QFile::remove(polyDataFilePath());
    m_mTime.Modified();
}

vtkSmartPointer<vtkIdList> Reseau::pointIds() {
    if (m_meshPointIds == nullptr) {
        loadIdsData();
    }
    return m_meshPointIds;
}
void Reseau::setPointIds(vtkSmartPointer<vtkIdList> ids) {
    m_meshPointIds = ids;
}

void Reseau::loadIdsData() {
    m_meshPointIds = vtkSmartPointer<vtkIdList>::New();
    QFile file(idsFileName());
    if (file.open(QIODevice::ReadOnly)) {
        auto numberOfIds = file.size() / sizeof(vtkIdType);
        file.read((char*)m_meshPointIds->Resize(numberOfIds), file.size());
        m_meshPointIds->SetNumberOfIds(numberOfIds);
    }
}

QString Reseau::idsFileName() const {
    return QString("%1/mesh_%2.ids").arg(path()).arg(id());
}
QString Reseau::polyDataFilePath() const {
    return QString("%1/mesh_%2.vtp").arg(path()).arg(id());
}
void Reseau::savePointIds() {
    savePointIds(m_meshPointIds);
}

void Reseau::savePointIds(vtkSmartPointer<vtkIdList> ids) {
    if (ids == nullptr) {
        QFile::remove(idsFileName());
    } else {
        QFile file(idsFileName());
        if (file.open(QIODevice::WriteOnly)) {
            auto bufferSize = ids->GetNumberOfIds() * sizeof(vtkIdType);
            file.write((const char*)ids->begin(), bufferSize);
        }
    }
}

vtkMTimeType Reseau::GetMTime()
{
    return m_mTime.GetMTime();
}

void Reseau::savePolyData(vtkPolyData* polyData) {
    if (polyData != nullptr) {
        vtkNew<vtkXMLPolyDataWriter> writer;
        writer->GlobalWarningDisplayOff(); // hide VTK errors
        writer->SetFileName(polyDataFilePath().toStdString().c_str());
        writer->SetInputData(polyData);
        writer->Update();
    }
}

void Reseau::assignPolyData(vtkSmartPointer<vtkPolyData> polyData) {
    m_kdTree = nullptr;
    m_polyData = polyData;
}

vtkKdTree* Reseau::kdTree() {
    if (m_polyData != nullptr && m_kdTree == nullptr) {
        vtkPoints* points = m_polyData->GetPoints();
        if (points != nullptr) {
            m_kdTree = vtkSmartPointer<vtkKdTree>::New();
            m_kdTree->BuildLocatorFromPoints(points);
        }
    }
    return m_kdTree;
}

qint32 Reseau::pointNumber() const
{
    return m_pointNumber;
}

void Reseau::setPointNumber(qint32 newPointNumber)
{
    if (m_pointNumber == newPointNumber)
        return;
    m_pointNumber = newPointNumber;
    emit pointNumberChanged();
}

bool Reseau::buildin() const
{
    return m_buildin;
}

void Reseau::setBuildin(bool newBuildin)
{
    if (m_buildin == newBuildin)
        return;
    m_buildin = newBuildin;
    emit buildinChanged();
}
void Reseau::setName(const QString &v) {
    if (m_name != v) {
        m_name = v.trimmed();
        emit nameChanged();
        m_mTime.Modified();
    }
}
QString Reseau::getName() const {
    return m_name;
}
bool Reseau::apparent() const
{
    return m_apparent;
}

void Reseau::setApparent(bool newApparent)
{
    if (m_apparent == newApparent)
        return;
    m_apparent = newApparent;
    emit apparentChanged();
    m_mTime.Modified();
}


qint32 Reseau::addPointIds(vtkSmartPointer<vtkIdList> ids) {
    Q_ASSERT(ids != nullptr);

    qint32 idAddedCount = 0;
    vtkSmartPointer<vtkIdList> meshIds = pointIds();
    for (int i = 0; i < ids->GetNumberOfIds(); ++i) {
        vtkIdType id = ids->GetId(i);
        if (meshIds->IsId(id) != -1) {
            continue;
        }
        meshIds->InsertNextId(id);
        idAddedCount++;
    };

    if (idAddedCount > 0) {
        emit pointIdsAdded(IdListWarp::Ptr::create(ids));
        m_mTime.Modified();
    }
    return idAddedCount;
}

void Reseau::removePointIds(vtkSmartPointer<vtkIdList> ids) {
    Q_ASSERT(ids != nullptr);
    vtkSmartPointer<vtkIdList> meshIds = pointIds();
    for (int i = 0; i < ids->GetNumberOfIds(); ++i) {
        meshIds->DeleteId(ids->GetId(i));
    }
    emit pointIdsRemoved(IdListWarp::Ptr::create(ids));
    m_mTime.Modified();
}

QString Reseau::path() const
{
    return m_path;
}

QColor Reseau::dyestuff() const
{
    return m_dyestuff;
}

vtkColor3d Reseau::getDyestuff3d() const {
    return vtkutil::qColorToVtkColor3d(m_dyestuff);
}
void Reseau::setDyestuff(const QColor &newDyestuff)
{
    if (m_dyestuff == newDyestuff)
        return;
    m_dyestuff = newDyestuff;
    emit dyestuffChanged();
    m_mTime.Modified();
}

void Reseau::setPolyData(vtkSmartPointer<vtkPolyData> polyData) {
    assignPolyData(polyData);
    setPointNumber(polyData == nullptr?0:polyData->GetNumberOfPoints());
    emit changed();
    m_mTime.Modified();
}

vtkSmartPointer<vtkPolyData> Reseau::polyData() {
    if (m_polyData == nullptr) {
        vtkNew<vtkXMLPolyDataReader> reader;
        reader->GlobalWarningDisplayOff(); // hide VTK errors
        reader->SetFileName(polyDataFilePath().toStdString().c_str());
        reader->Update();
        assignPolyData(reader->GetOutput());
        m_pointNumber = m_polyData->GetNumberOfPoints();
    }
    return m_polyData;
}

double Reseau::checkPoint(const double point[3]) const {
    return checkPoint(point[0], point[1], point[2]);
}

double Reseau::checkPoint(const vtkVector3d &pos) const {
    return checkPoint(pos.GetX(), pos.GetY(), pos.GetZ());
}

double Reseau::checkPoint(double x, double y, double z)  const{
    if (m_polyData == nullptr || m_polyData->GetNumberOfPoints() == 0) {
        return -1;
    }
    if (m_implicitPolyDataDistance == nullptr) {
        m_implicitPolyDataDistance = vtkSmartPointer<vtkImplicitPolyDataDistance>::New();
        m_implicitPolyDataDistance->SetInput(m_polyData);
    }
    return m_implicitPolyDataDistance->EvaluateFunction(x, y, z);
}

vtkSmartPointer<vtkIdList> Reseau::getPointsWithinRadius(const double point[3], double radius) {
    vtkSmartPointer<vtkIdList> result = vtkSmartPointer<vtkIdList>::New();
    if (kdTree() != nullptr) {
        m_kdTree->FindPointsWithinRadius(radius, point, result);
    }
    return result;
}

vtkSmartPointer<vtkIdList> Reseau::getPointsWithinRadius(const vtkVector3d &point, double radius) {
    return getPointsWithinRadius(point.GetData(), radius);
}

std::pair<vtkIdType, double> Reseau::getClosestPointWithinRadius(const vtkVector3d &point, double radius) {
    return getClosestPointWithinRadius(point.GetData(), radius);
}

std::pair<vtkIdType, double> Reseau::getClosestPointWithinRadius(const double point[3], double radius)
{
    if (kdTree() == nullptr) {
        return std::make_pair(-1, 0);
    }
    double dist2 = 0;
    vtkIdType pointId = m_kdTree->FindClosestPointWithinRadius(radius, point, dist2);
    return std::make_pair(pointId, dist2);
}

void Reseau::getPoint(vtkIdType id, double point[3]) {
    if (id != -1 && m_polyData != nullptr) {
        m_polyData->GetPoint(id, point);
    }
}
void Reseau::getPoint(vtkIdType id, vtkVector3d &point) {
    getPoint(id, point.GetData());
}
