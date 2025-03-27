#include "Profile.h"
#include "album/AlbumDb.h"
#include "casket/CasketDb.h"
#include "catheter/Catheter.h"
#include "catheter/CatheterDb.h"
#include "channel/ChannelDataDb.h"
#include "dissolve/DissolveDb.h"
#include "dissolve/DissolveOptions.h"
#include "halitus/BreathOptions.h"
#include "mapping/MappingPointGroupDb.h"
#include "mapping/MappingPointsDb.h"
#include "mapping/MappingPointGroup.h"
#include "melt/MeltChannelDb.h"
#include "melt/MeltDb.h"
#include "melt/MeltOptions.h"
#include "melt/MeltOptionsDb.h"
#include "notebook/NotebookDb.h"
#include "notebook/NotebookOptions.h"
#include "oscillograp/OscillograpOptions.h"
#include "mesh/ReproduceOptions.h"
#include "EcgOptions.h"
#include "oscillograp/SignalSourceOptions.h"
#include "study/StageSettings.h"
#include "study/alarm/AlarmDb.h"
#include "study/azimuth/AzimuthDb.h"
#include "study/scalar/ScalarOptions.h"
#include "ProfileSnapshot.h"
#include <QJsonDocument>
#include <qfile.h>
#include <qjsonobject.h>
#include <vtkPolyData.h>
#include <profile/FilterOptions.h>
#include <mesh/BlackboardDb.h>
#include <reseau/ReseauDb.h>
#include <QDir>
#include <QStandardPaths>
#include <vtkFloatArray.h>
#include <vtkMatrix4x4.h>
#include <vtkPLYReader.h>
#include <utility/Thread.h>
#include <study/scalar/ScalarDb.h>
#include <catheter/CatheterTempletDb.h>
#include <notebook/NotebookSnapshot.h>
#include <QtConcurrent>
#include "utility/FileUtils.h"

Profile::Profile(QObject *parent)
    : QObject(parent)
{
}

Profile::Profile(const QString &path,QObject *parent)
    : QObject(parent),
      m_path(path)
{
}

qint64 Profile::currentMappingPointId() const
{
    return m_currentMappingPointId;
}

void Profile::setCurrentMappingPointId(qint64 newCurrentMappingPointId)
{
    if (m_currentMappingPointId == newCurrentMappingPointId)
        return;
    m_currentMappingPointId = newCurrentMappingPointId;
    emit currentMappingPointIdChanged();
}


NotebookOptions *Profile::notebookOptions()
{
    if (m_notebookOptions == nullptr) {
        m_notebookOptions = new NotebookOptions(this);
    }
    return m_notebookOptions;
}

qint64 Profile::currentDissolvePointId() const
{
    return m_currentDissolvePointId;
}

void Profile::setCurrentDissolvePointId(qint64 newCurrentDissolvePointId)
{
    if (m_currentDissolvePointId == newCurrentDissolvePointId)
        return;
    m_currentDissolvePointId = newCurrentDissolvePointId;
    emit currentDissolvePointIdChanged();
}

QString Profile::name() const
{
    return m_name;
}

void Profile::setName(const QString &newName)
{
    if (m_name == newName)
        return;
    m_name = newName;
    emit nameChanged();
}

CatheterTempletDb *Profile::catheterTempletDb()
{
    if (m_catheterTempletDb == nullptr) {
        m_catheterTempletDb = new CatheterTempletDb(m_path, this);
    }
    return m_catheterTempletDb;
}

MeltChannelDb *Profile::meltChannelDb()
{
    if (m_meltChannelDb == nullptr) {
        m_meltChannelDb = new MeltChannelDb(m_path, this);
    }
    return m_meltChannelDb;
}

vtkMatrix4x4* Profile::orientaionMatrix() const
{
    return m_orientaionMatrix;
}

void Profile::setOrientaionMatrix(const vtkMatrix4x4* newOrientaionMatrix)
{
    m_orientaionMatrix = nullptr;
    if (newOrientaionMatrix != nullptr) {
        m_orientaionMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
        m_orientaionMatrix->DeepCopy(newOrientaionMatrix);
    }
    emit orientaionMatrixChanged();
}

QString Profile::pulseElectrodeId() const
{
    return m_pulseElectrodeId;
}

void Profile::setPulseElectrodeId(const QString &newPulseElectrodeId)
{
    if (m_pulseElectrodeId == newPulseElectrodeId)
        return;
    m_pulseElectrodeId = newPulseElectrodeId;
    emit pulseElectrodeIdChanged();
}

void Profile::setCenterPoint(const vtkVector3d &pos)
{
    if (pos == m_centerPoint) {
        return;
    }
    m_centerPoint = pos;
    emit centerPointChanged();
}

vtkVector3d Profile::getCenterPoint() const
{
    return m_centerPoint;
}

void Profile::save() {
}

QString Profile::getStoragePath(const QString &path) {
    return path + "/configure.json";
}

void Profile::backup(const QString &path)
{
    QString backupFile = getStoragePath(path);
    QFile::copy(getStoragePath(m_path), backupFile);
}

const QString& Profile::getId() const {
    return m_id;
}

FilterOptions *Profile::filterOptions() {
    if (m_filterOptions == nullptr) {
        Q_ASSERT(Thread::currentlyOn(Thread::UI));
        m_filterOptions = new FilterOptions(this);
    }
    return m_filterOptions;
}


SignalSourceOptions* Profile::signalSourceOptions() {
    if (m_signalSourceOptions == nullptr) {
        Q_ASSERT(Thread::currentlyOn(Thread::UI));
        m_signalSourceOptions = new SignalSourceOptions(this);
    }
    return m_signalSourceOptions;
}

EcgOptions* Profile::ecgOptions()
{
    if (m_ecgOptions == nullptr) {
        Q_ASSERT(Thread::currentlyOn(Thread::UI));
        m_ecgOptions = new EcgOptions(this);
    }
    return m_ecgOptions;
}

AzimuthDb *Profile::azimuthDb()
{
    if (m_azimuthDb == nullptr) {
        Q_ASSERT(Thread::currentlyOn(Thread::UI));
        m_azimuthDb = new AzimuthDb(m_path, this);
    }
    return m_azimuthDb;
}

Mapping* Profile::mapping() {
    if (m_mapping == nullptr) {
        Q_ASSERT(Thread::currentlyOn(Thread::UI));
        m_mapping = new Mapping(this);
    }
    return m_mapping;
}

MeltDb* Profile::meltDb() {
    if (m_meltDb == nullptr) {
        Q_ASSERT(Thread::currentlyOn(Thread::UI));
        m_meltDb = new MeltDb(m_path, this);
    }
    return m_meltDb;
}

BlackboardDb *Profile::blackboardDb() {
    if (m_blackboardDb == nullptr) {
        Q_ASSERT(Thread::currentlyOn(Thread::UI));
        m_blackboardDb = new BlackboardDb(this);
    }
    return m_blackboardDb;
}

ChannelDataDb *Profile::channelDataDb()
{
    if (m_channelDataDb == nullptr) {
        Q_ASSERT(Thread::currentlyOn(Thread::UI));
        m_channelDataDb = new ChannelDataDb(m_path, this);
    }
    return m_channelDataDb;
}

CatheterDb *Profile::catheterDb() {
    if (m_catheterDb == nullptr) {
        Q_ASSERT(Thread::currentlyOn(Thread::UI));
        m_catheterDb = new CatheterDb(m_path, this);
    }
    return m_catheterDb;
}

int Profile::getIncreaseMeltOrder()
{
    m_increaseMeltOrder++;
    return m_increaseMeltOrder;
}

AlbumDb *Profile::albumDb() {
    if (m_albumDb == nullptr) {
        Q_ASSERT(Thread::currentlyOn(Thread::UI));
        m_albumDb = new AlbumDb(m_path,this);
    }
    return m_albumDb;
}

QString Profile::albumSavePath()
{
    return AlbumDb::getPictureStoragePath(m_path);
}

Profile::State Profile::state() const {
    return m_state;
}

void Profile::setState(State newState) {
    if (m_state == newState)
        return;
    m_state = newState;
    emit stateChanged();
}

int Profile::modelRef() const
{
    return m_modelRef;
}

void Profile::increaseRef()
{
    Q_ASSERT(Thread::currentlyOn(Thread::UI));
    m_modelRef++;
    emit modelRefChanged(m_modelRef);
}

void Profile::decreasecRef()
{
    Q_ASSERT(Thread::currentlyOn(Thread::UI));
    m_modelRef--;
    emit modelRefChanged(m_modelRef);
}


ScalarOptions *Profile::scalarOptions()
{
    if (m_scalarOptions == nullptr) {
        Q_ASSERT(Thread::currentlyOn(Thread::UI));
        m_scalarOptions = new ScalarOptions(this);
    }
    return m_scalarOptions;
}


ScalarDb *Profile::scalarDb()
{
    if (m_scalarDb == nullptr) {
        Q_ASSERT(Thread::currentlyOn(Thread::UI));
        m_scalarDb = new ScalarDb(m_path,scalarOptions(), this);
    }
    return m_scalarDb;
}

BreathOptions *Profile::breathOptions()
{
    if (m_breathOptions == nullptr) {
        Q_ASSERT(Thread::currentlyOn(Thread::UI));
        m_breathOptions = new BreathOptions(this);
    }
    return m_breathOptions;
}

DissolveOptions *Profile::dissolveOptions()
{
    if (m_dissolveOptions == nullptr) {
        Q_ASSERT(Thread::currentlyOn(Thread::UI));
        m_dissolveOptions = new DissolveOptions(this);
    }
    return m_dissolveOptions;
}


qint64 Profile::mappingReseau() const
{
    return m_mappingReseauId;
}

void Profile::setMappingReseau(qint64  newMappingReseauId)
{
    if (m_mappingReseauId == newMappingReseauId)
        return;
    m_mappingReseauId = newMappingReseauId;
    emit mappingReseauChanged();
}

Halve::MeltState Profile::meltState() const
{
    return m_meltState;
}

void Profile::setMeltState(const Halve::MeltState& newMeltState)
{
    if (m_meltState == newMeltState)
        return;
    m_meltState = newMeltState;
    emit meltStateChanged();
}


bool Profile::pantSampling() const
{
    return m_pantSampling;
}

void Profile::setPantSampling(bool newPantSampling)
{
    if (m_pantSampling == newPantSampling)
        return;
    m_pantSampling = newPantSampling;
    emit pantSamplingChanged();
}

qint32 Profile::increaseReseauId()
{
    return ++m_reseauIdIndex;
}
qint32 Profile::increaseSnapshotId() {

    return ++m_snapshotIdIndex;
}

qint32 Profile::increaseMappingPointGroupId()
{
    return ++m_mappingPointGroupIdIndex;
}

QString Profile::layoutFile() const {
    return m_layoutFile;
}

void Profile::setLayoutFile(const QString &newFile) {
    if (m_layoutFile == newFile) {
        return;
    }
    m_layoutFile = newFile;
    emit layoutFileChanged();
}

qint64 Profile::currentReseauId() const
{
    return m_currentReseauId;
}

void Profile::setCurrentReseauId(qint64 newCurrentReseau) {
    if (m_currentReseauId == newCurrentReseau)
        return;
    m_currentReseauId = newCurrentReseau;
    emit currentReseauIdChanged();
}

Reseau *Profile::getCurrentReseau()
{
    return reseauDb()->getData(m_currentReseauId);
}

qint32 Profile::channelDatasImportSpeed() const
{
    return m_channelDatasImportSpeed;
}

void Profile::setChannelDatasImportSpeed(qint32 newChannelDatasImportSpeed)
{
    if (m_channelDatasImportSpeed == newChannelDatasImportSpeed)
        return;
    m_channelDatasImportSpeed = newChannelDatasImportSpeed;
    emit channelDatasImportSpeedChanged();
}

QString Profile::currentMappingPointGroup()
{
    if (m_currentMappingPointGroup.isEmpty()) {
        m_currentMappingPointGroup = mappingPointGroupDb()->getData(0)->id();
    }
    return m_currentMappingPointGroup;
}

void Profile::setCurrentMappingPointGroup(const QString &newCurrentMappingPointGroup)
{
    if (m_currentMappingPointGroup == newCurrentMappingPointGroup)
        return;
    m_currentMappingPointGroup = newCurrentMappingPointGroup;
    emit currentMappingPointGroupChanged();
}

MappingPointGroup *Profile::getCurrentMappingPointGroup()
{
    return mappingPointGroupDb()->getData(m_currentMappingPointGroup);
}

MappingPointGroupDb *Profile::mappingPointGroupDb()
{
    if (m_mappingPointGroupDb == nullptr) {
        Q_ASSERT(Thread::currentlyOn(Thread::UI));
        m_mappingPointGroupDb = new MappingPointGroupDb(m_path, this);
    }
    return m_mappingPointGroupDb;
}


int Profile::latPerimeter() const
{
    return m_latPerimeter;
}

void Profile::setLatPerimeter(int newLatPerimeter)
{
    if (m_latPerimeter == newLatPerimeter)
        return;
    m_latPerimeter = newLatPerimeter;
    emit latPerimeterChanged();
}

int Profile::pulse() const
{
    return m_pulse;
}

void Profile::setPulse(int newPulse)
{
    if (m_pulse == newPulse)
        return;
    m_pulse = newPulse;
    emit pulseChanged();
}



CasketDb *Profile::casketDb()
{
    if (m_casketDb == nullptr) {
        Q_ASSERT(Thread::currentlyOn(Thread::UI));
        m_casketDb = new CasketDb(m_path, catheterDb(), this);
    }
    return m_casketDb;
}

ReseauDb *Profile::reseauDb()
{
    if (m_reseauDb == nullptr) {
        Q_ASSERT(Thread::currentlyOn(Thread::UI));
        m_reseauDb = new ReseauDb(m_path, this);
    }
    return m_reseauDb;
}

Halve::MappingType Profile::mappingType() const
{
    return m_mappingType;
}

void Profile::setMappingType(Halve::MappingType newMappingType)
{
    if (m_mappingType == newMappingType)
        return;
    m_mappingType = newMappingType;
    emit mappingTypeChanged();
}

OscillograpElectrode *Profile::oscillograpElectrode()
{
    if (m_oscillograpElectrode == nullptr) {
        Q_ASSERT(Thread::currentlyOn(Thread::UI));
        m_oscillograpElectrode = new OscillograpElectrode(catheterDb(),this);
    }
    return m_oscillograpElectrode;
}

MappingPointsDb *Profile::mappingPointsDb()
{
    if (m_mappingPointsDb == nullptr) {
        Q_ASSERT(Thread::currentlyOn(Thread::UI));
        m_mappingPointsDb = new MappingPointsDb(m_path, this);
    }
    return m_mappingPointsDb;
}

DissolveDb *Profile::dissolveDb()
{
    if (m_dissolveDb == nullptr) {
        Q_ASSERT(Thread::currentlyOn(Thread::UI));
        m_dissolveDb = new DissolveDb(m_path, this);
    }
    return m_dissolveDb;
}
NotebookDb *Profile::notebookDb() {
    if (m_notebookDb == nullptr) {
        Q_ASSERT(Thread::currentlyOn(Thread::UI));
        m_notebookDb = new NotebookDb(m_path, albumDb(), this);
    }
    return m_notebookDb;
}

bool Profile::pointValidCheck() const
{
    return m_pointValidCheck;
}

void Profile::setPointValidCheck(bool newPointValidCheck)
{
    if (m_pointValidCheck == newPointValidCheck)
        return;
    m_pointValidCheck = newPointValidCheck;
    emit pointValidCheckChanged();
}

AlarmDb *Profile::alarmDb()
{
    if (m_alarmDb == nullptr) {
        Q_ASSERT(Thread::currentlyOn(Thread::UI));
        m_alarmDb = new AlarmDb(this);
    }
    return m_alarmDb;
}

Halve::ChannelMode Profile::channelMode() const
{
    return m_channelMode;
}

void Profile::setChannelMode(const Halve::ChannelMode &newChannelMode)
{
    if (m_channelMode == newChannelMode)
        return;
    m_channelMode = newChannelMode;
    emit channelModeChanged();
}

Halve::StorageState Profile::storageState() const
{
    return m_storageState;
}

void Profile::setStorageState(const Halve::StorageState &newStorageState)
{
    if (m_storageState == newStorageState)
        return;
    m_storageState = newStorageState;
    emit storageStateChanged();
}

ReproduceOptions *Profile::reproduceOptions()
{
    if (m_reproduceOptons == nullptr) {
        Q_ASSERT(Thread::currentlyOn(Thread::UI));
        m_reproduceOptons = new ReproduceOptions(this);
    }
    return m_reproduceOptons;
}

OscillograpOptions *Profile::oscillograpOptions()
{
    if (m_oscillograpOptions == nullptr) {
        Q_ASSERT(Thread::currentlyOn(Thread::UI));
        m_oscillograpOptions = new OscillograpOptions(this);
    }
    return m_oscillograpOptions;
}

MeltOptions *Profile::meltOptions()
{
    return meltOptionsDb()->getData(m_currentMeltOptionsId);
}

MeltOptionsDb *Profile::meltOptionsDb()
{
    if (m_meltOptionsDb == nullptr) {
        Q_ASSERT(Thread::currentlyOn(Thread::UI));
        m_meltOptionsDb = new MeltOptionsDb(m_path,catheterDb(), this);
    }
    return m_meltOptionsDb;
}

void Profile::setCurrentMeltOptions(qint64 id)
{
    if (m_currentMeltOptionsId == id) {
        return;
    }
    m_currentMeltOptionsId = id;
    emit meltOptionsChanged();
}

qint64 Profile::currentMeltOptions() const {
    return m_currentMeltOptionsId;
}

MeltSetting *Profile::meltSetting() {
    return mappingPointGroupDb()->getData(currentMappingPointGroup())->meltSetting();
}


MappingSetting *Profile::mappingSetting() {
    return mappingPointGroupDb()->getData(currentMappingPointGroup())->mappingSetting();
}
StageSettings *Profile::stageSettings()
{
    if (m_stageSettings == nullptr) {
        Q_ASSERT(Thread::currentlyOn(Thread::UI));
        m_stageSettings = new StageSettings(this);
    }
    return m_stageSettings;
}

QString Profile::path() const
{
    return m_path;
}

bool Profile::renovating() const
{
    return m_renovating;
}

void Profile::setRenovating(bool newRenovating)
{
    if (m_renovating == newRenovating)
        return;
    m_renovating = newRenovating;
    emit renovatingChanged();
}


bool Profile::useBackReference() const
{
    return m_useBackReference;
}

void Profile::setUseBackReference(bool newUseBackReference)
{
    if (m_useBackReference == newUseBackReference)
        return;
    m_useBackReference = newUseBackReference;
    emit useBackReferenceChanged();
}
bool Profile::catheterAlined()
{
    auto catheterId = reproduceOptions()->catheterId();
    if (catheterId.isEmpty()) {
        return false;
    }
    Catheter* catheter = m_catheterDb->getData(catheterId);
    if (catheter == nullptr) {
        return false;
    }
    return catheter->employ();
}

ProfileSnapshot* Profile::loadSnapshot(NotebookSnapshot* snapshot) {
    QString snapshotProfileId = QString::number(QDateTime::currentMSecsSinceEpoch());
    QString srcPath = snapshot->snapshotPath();
    ProfileSnapshot *currentSnapshot = new ProfileSnapshot(snapshotProfileId, srcPath, snapshot, this);
    return currentSnapshot;
}

void Profile::freeSnapshot(ProfileSnapshot* snapshot) {
    if (snapshot != nullptr) {
        snapshot->deleteLater();
    }
}

QJsonValue Profile::toJson() {
    QJsonObject json;
    json["filter"] = filterOptions()->toJson();
    json["signalSource"] = signalSourceOptions()->toJson();
    json["ecg"] = ecgOptions()->toJson();
    json["mapping"] = mapping()->toJson();
    json["reproduce"] = reproduceOptions()->toJson();
    json["dissolve"] = dissolveOptions()->toJson();
    json["scalar"] = scalarOptions()->toJson();
    json["stageSettings"] = stageSettings()->toJson();
    json["increaseMeltOrder"] = m_increaseMeltOrder;
    json["oscillograpElectrode"] = oscillograpElectrode()->toJson();
    json["currentMappingPointGroup"] = m_currentMappingPointGroup;
    json["latPerimeter"] = m_latPerimeter;
    json["pulse"] = m_pulse;
    json["mappingType"] = m_mappingType;
    json["currentReseauId"] = m_currentReseauId;
    json["layoutFile"] = m_layoutFile;
    json["mappingPointGroupIdIndex"] = m_mappingPointGroupIdIndex;
    json["reseauIdIndex"] = m_reseauIdIndex;
    json["snapshotIdIndex"] = m_snapshotIdIndex;
    json["pantSampling"] = m_pantSampling;
    json["mappingReseau"] = m_mappingReseauId;
    json["pulseElectrode"] = m_pulseElectrodeId;
    json["breath"] = breathOptions()->toJson();
    json["oscillograp"] = oscillograpOptions()->toJson();
    json["stageSettings"] = stageSettings()->toJson();
    json["notebookOptions"] = notebookOptions()->toJson();
    json["channelMode"] = m_channelMode;
    json["useBackReference"] = m_useBackReference;

    QJsonObject enterPoint;
    enterPoint["x"] = m_centerPoint.GetX();
    enterPoint["y"] = m_centerPoint.GetY();
    enterPoint["z"] = m_centerPoint.GetZ();
    json["centerPoint"] = enterPoint;

    if (m_orientaionMatrix) {
        QByteArray byteArray;
        QDataStream stream(&byteArray, QIODevice::WriteOnly);
        stream << *m_orientaionMatrix;
        json["orientaionMatrix"] = QString::fromLatin1(byteArray);
    }

    return json;
}

void Profile::fromJson(const QJsonObject &json) {
    if (json.contains("filter")) {
        filterOptions()->fromJson(json["filter"].toObject());
    }
    if (json.contains("signalSource")) {
        signalSourceOptions()->fromJson(json["signalSource"].toArray());
    }
    if (json.contains("notebookOptions")) {
        notebookOptions()->fromJson(json["notebookOptions"].toObject());
    }
    if (json.contains("dissolve")) {
        dissolveOptions()->fromJson(json["dissolve"].toObject());
    }
    if (json.contains("ecg")) {
        ecgOptions()->fromJson(json["ecg"].toObject());
    }
    if (json.contains("mapping")) {
        mapping()->fromJson(json["mapping"].toObject());
    }
    if (json.contains("reproduce")) {
        reproduceOptions()->fromJson(json["reproduce"].toObject());
    }
    if (json.contains("scalar")) {
        scalarOptions()->fromJson(json["scalar"].toObject());
    }
    if (json.contains("stageSettings")) {
        stageSettings()->fromJson(json["stageSettings"].toArray());
    }
    if (json.contains("increaseMeltOrder")) {
        m_increaseMeltOrder = json["increaseMeltOrder"].toInt();
    }
    if (json.contains("oscillograpElectrode")) {
        oscillograpElectrode()->fromJson(json["oscillograpElectrode"].toArray());
    }
    if (json.contains("currentMappingPointGroup")) {
        m_currentMappingPointGroup = json["currentMappingPointGroup"].toString();
    }
    if (json.contains("latPerimeter")) {
        m_latPerimeter = json["latPerimeter"].toInt();
    }
    if (json.contains("channelMode")) {
        m_channelMode =(Halve::ChannelMode)json["channelMode"].toInt();
    }
    if (json.contains("pulse")) {
        m_pulse = json["pulse"].toInt();
    }
    if (json.contains("mappingType")) {
        m_mappingType = (Halve::MappingType)json["mappingType"].toInt();
    }

    if (json.contains("currentReseauId")) {
        m_currentReseauId = json["currentReseauId"].toInteger();
    }
    if (json.contains("pulseElectrode")) {
        m_pulseElectrodeId = json["pulseElectrode"].toString();
    }
    if (json.contains("layoutFile")) {
        m_layoutFile = json["layoutFile"].toString();
    }
    if (json.contains("mappingPointGroupIdIndex")) {
        m_mappingPointGroupIdIndex = json["mappingPointGroupIdIndex"].toInt();
    }
    if (json.contains("reseauIdIndex")) {
        m_reseauIdIndex = json["reseauIdIndex"].toInt();
    }
    if (json.contains("snapshotIdIndex")) {
        m_snapshotIdIndex = json["snapshotIdIndex"].toInt();
    }
    if (json.contains("pantSampling")) {
        m_pantSampling = json["pantSampling"].toBool();
    }
    if (json.contains("mappingReseau")) {
        m_mappingReseauId = json["mappingReseau"].toInteger();
    }

    if (json.contains("breath")) {
        breathOptions()->fromJson(json["breath"].toObject());
    }

    if (json.contains("oscillograp")) {
        oscillograpOptions()->fromJson(json["oscillograp"].toObject());
    }
    if (json.contains("centerPoint")) {
        auto enterPoint = json["centerPoint"].toObject();
        m_centerPoint.Set(enterPoint["x"].toDouble(), enterPoint["y"].toDouble(), enterPoint["z"].toDouble());
    }
    if (json.contains("useBackReference")) {
        m_useBackReference = json["useBackReference"].toBool();
    }

    if (json.contains("orientaionMatrix")) {
        QByteArray byteArray = json["orientaionMatrix"].toString().toLatin1();
        m_orientaionMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
        QDataStream stream(&byteArray, QIODevice::ReadOnly);
        stream >> *m_orientaionMatrix;
    }
}
