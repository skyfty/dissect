#include "CatheterElectrodeNodePointDb.h"
#include "catheter/CatheterMould.h"
#include <QDir>
#include <QFile>
#include <QFileSystemWatcher>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkUnstructuredGrid.h>
#include <QDesktopServices>
#include <QUrl>
#include "utility/FileUtils.h"
#include <QTimer>

CatheterElectrodeNodePointDb::CatheterElectrodeNodePointDb(QObject *parent)
    :QObject(parent){
}

void CatheterElectrodeNodePointDb::editMesh() {
    QString meshFileName = m_catheter->meshName();
    if (meshFileName.isEmpty()) {
        meshFileName = QString("%1.json").arg(QDateTime::currentSecsSinceEpoch());
    }
    QString meshPath = file_utils::getCatheterMeshPath(meshFileName);
    if (!QFile::exists(meshPath)) {
        QJsonObject json;
        m_catheter->catheterMould()->toJson(json);
        saveMeshFile(meshPath, json);
    }
    m_catheter->setMeshName(meshFileName);
    catheterDb->save();
    QDesktopServices::openUrl(QUrl::fromLocalFile(meshPath));
}

void CatheterElectrodeNodePointDb::resetMesh() {
    CatheterMould *mould = m_catheter->catheterMould();
    mould->makeDefultGrid(m_catheter->getGap());
    QString meshPath = file_utils::getCatheterMeshPath(m_catheter->meshName());
    if (!meshPath.isEmpty()) {
        QJsonObject json;
        mould->toJson(json);
        saveMeshFile(meshPath, json);
    }
}

void CatheterElectrodeNodePointDb::saveMeshFile(const QString &file, const QJsonObject &json) {
    QFile saveFile(file);
    if (saveFile.open(QIODevice::WriteOnly)) {
        saveFile.write(QJsonDocument(json).toJson());
    }
}

Catheter *CatheterElectrodeNodePointDb::catheter() const
{
    return m_catheter;
}

void CatheterElectrodeNodePointDb::setCatheter(Catheter *newCatheter)
{
    if (m_catheter == newCatheter)
        return;
    m_catheter = newCatheter;
    onCatheterMeshNameChanged();
    QObject::connect(m_catheter, &Catheter::meshNameChanged, this, &CatheterElectrodeNodePointDb::onCatheterMeshNameChanged);
    emit catheterChanged();
}

void CatheterElectrodeNodePointDb::watchFile(const QString &filePath) {
    m_watcher.reset(new QFileSystemWatcher());
    if(!filePath.isEmpty() && QFile::exists(filePath)) {
        m_watcher->addPath(filePath);
    }
    QObject::connect(m_watcher.get(), &QFileSystemWatcher::fileChanged,[this](){
        QTimer::singleShot(700, this, &CatheterElectrodeNodePointDb::onCatheterMeshChanged);
    });
}

void CatheterElectrodeNodePointDb::onCatheterMeshNameChanged() {
    QString meshPath = file_utils::getCatheterMeshPath(m_catheter->meshName());
    watchFile(meshPath);
}

void CatheterElectrodeNodePointDb::onCatheterMeshChanged() {
    m_catheter->catheterMould()->reload();
    emit meshChanged();
}

CatheterDb *CatheterElectrodeNodePointDb::getCatheterDb() const
{
    return catheterDb;
}

void CatheterElectrodeNodePointDb::setCatheterDb(CatheterDb *newCatheterDb)
{
    if (catheterDb == newCatheterDb)
        return;
    catheterDb = newCatheterDb;
    emit catheterDbChanged();
}

qint32 CatheterElectrodeNodePointDb::pointCount() const
{
    if (m_catheter == nullptr) {
        return 0;
    }
    auto mouldGrid = m_catheter->catheterMould()->grid();
    return mouldGrid->GetNumberOfPoints();
}

vtkSmartPointer<vtkUnstructuredGrid> CatheterElectrodeNodePointDb::mouldGrid() {
    if (m_catheter == nullptr) {
        return nullptr;
    }
    return m_catheter->catheterMould()->grid();
}

extern const char *TypesPointDataName ;

Halve::CatheterElectrodeType CatheterElectrodeNodePointDb::getType(int row) {
    if (m_catheter == nullptr) {
        return Halve::CET_TUBE;
    }
    auto types = m_catheter->catheterMould()->grid()->GetPointData()->GetArray(TypesPointDataName);
    return (Halve::CatheterElectrodeType)types->GetTuple1(row);
}
