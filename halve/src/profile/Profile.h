#pragma once

#include <QObject>
#include <QSharedPointer>
#include <QMap>
#include <QDate>
#include <QPointer>
#include <vtkVector.h>
#include <vtkMatrix4x4.h>
#include "HalveType.h"
#include <profile/FilterOptions.h>
#include "utility/VtkDataWarp.h"
#include "Mapping.h"
#include <mapping/MappingPointGroupDb.h>
#include "mesh/ReproduceOptions.h"
#include <study/StageSettings.h>
#include <reseau/Reseau.h>
#include <study/alarm/AlarmDb.h>
#include "melt/MeltSamplingElectrode.h"
#include "oscillograp/OscillograpElectrode.h"

class AlbumDb;
class AlarmDb;
class User;
class TreeModel;
class BodyOptions;
class MeltDb;
class Catheter;
class ProfileData;
class BlackboardDb;
class ElectrodeNode;
class PegPointDb;
class DissolveDb;
class CasketDb;
class ReproduceOptions;
class ChannelDataDb;
class MappingPointsDb;
class EcgOptions;
class MappingPointGroupDb;
class ReseauDb;
class Reseau;
class PantElectrode;
class BreathOptions;
class DissolveOptions;
class ScalarDb;
class ScalarOptions;
class MeltOptions;
class OscillograpOptions;
class AzimuthDb;
class MeltOptionsDb;
class MeltSetting;
class MappingSetting;
class StageSettings;
class StageOptions;
class CameraInfo;
class vtkMatrix4x4;
class MeltChannelDb;
class CatheterTempletDb;
class MappingPointGroup;
class SignalSourceOptions;
class NotebookDb;
class NotebookOptions;
class ProfileSnapshot;
class NotebookSnapshot;

class Profile : public QObject {
  Q_OBJECT;
  Q_PROPERTY(State state READ state WRITE setState NOTIFY stateChanged FINAL)
  Q_PROPERTY(QString id READ getId CONSTANT);
  Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
  Q_PROPERTY(QString path READ path CONSTANT);
  Q_PROPERTY(int modelRef READ modelRef NOTIFY modelRefChanged CONSTANT FINAL)
  Q_PROPERTY(FilterOptions *filterOptions READ filterOptions CONSTANT FINAL);
  Q_PROPERTY(Mapping *mapping READ mapping CONSTANT FINAL)
  Q_PROPERTY(ReproduceOptions *reproduceOptions READ reproduceOptions CONSTANT FINAL)
  Q_PROPERTY(MappingPointsDb *mappingPointsDb READ mappingPointsDb CONSTANT FINAL)
  Q_PROPERTY(OscillograpElectrode *oscillograpElectrode READ oscillograpElectrode CONSTANT FINAL)
  Q_PROPERTY(Halve::MappingType mappingType READ mappingType WRITE setMappingType NOTIFY mappingTypeChanged FINAL)
  Q_PROPERTY(CatheterDb *catheterDb READ catheterDb CONSTANT FINAL)
  Q_PROPERTY(int pulse READ pulse WRITE setPulse NOTIFY pulseChanged FINAL)
  Q_PROPERTY(int latPerimeter READ latPerimeter WRITE setLatPerimeter NOTIFY latPerimeterChanged FINAL)
  Q_PROPERTY(QString currentMappingPointGroup READ currentMappingPointGroup WRITE setCurrentMappingPointGroup NOTIFY currentMappingPointGroupChanged FINAL)
  Q_PROPERTY(qint32 channelDatasImportSpeed READ channelDatasImportSpeed WRITE setChannelDatasImportSpeed NOTIFY channelDatasImportSpeedChanged FINAL)
  Q_PROPERTY(qint64 currentReseauId READ currentReseauId WRITE setCurrentReseauId NOTIFY currentReseauIdChanged FINAL)
  Q_PROPERTY(Reseau* currentReseau READ getCurrentReseau NOTIFY currentReseauIdChanged FINAL)
  Q_PROPERTY(QString layoutFile READ layoutFile CONSTANT FINAL)
  Q_PROPERTY(bool pantSampling READ pantSampling WRITE setPantSampling NOTIFY pantSamplingChanged FINAL)
  Q_PROPERTY(Halve::MeltState meltState READ meltState WRITE setMeltState NOTIFY meltStateChanged FINAL)
  Q_PROPERTY(qint64 mappingReseau READ mappingReseau WRITE setMappingReseau NOTIFY mappingReseauChanged FINAL)
  Q_PROPERTY(DissolveOptions *dissolveOptions READ dissolveOptions  NOTIFY dissolveOptionsChanged FINAL)
  Q_PROPERTY(BreathOptions *breathOptions READ breathOptions CONSTANT FINAL)
  Q_PROPERTY(bool renovating READ renovating WRITE setRenovating NOTIFY renovatingChanged FINAL)
  Q_PROPERTY(MeltOptions *meltOptions READ meltOptions  NOTIFY meltOptionsChanged FINAL)
  Q_PROPERTY(OscillograpOptions *oscillograpOptions READ oscillograpOptions CONSTANT FINAL)
  Q_PROPERTY(QString pulseElectrodeId READ pulseElectrodeId WRITE setPulseElectrodeId NOTIFY pulseElectrodeIdChanged FINAL)
  Q_PROPERTY(MeltOptionsDb *meltOptionsDb READ meltOptionsDb NOTIFY meltOptionsDbChanged FINAL)
  Q_PROPERTY(MeltSetting *meltSetting READ meltSetting NOTIFY meltSettingChanged FINAL)
  Q_PROPERTY(MappingSetting *mappingSetting READ mappingSetting NOTIFY mappingSettingChanged FINAL)
  Q_PROPERTY(MappingPointGroupDb *mappingPointGroupDb READ mappingPointGroupDb CONSTANT FINAL)
  Q_PROPERTY(StageSettings *stageSettings READ stageSettings CONSTANT FINAL)
  Q_PROPERTY(vtkMatrix4x4 *orientaionMatrix READ orientaionMatrix NOTIFY orientaionMatrixChanged FINAL)
  Q_PROPERTY(CatheterTempletDb *catheterTempletDb READ catheterTempletDb CONSTANT FINAL)
  Q_PROPERTY(qint64 currentDissolvePointId READ currentDissolvePointId WRITE setCurrentDissolvePointId NOTIFY currentDissolvePointIdChanged FINAL)
  Q_PROPERTY(qint64 currentMappingPointId READ currentMappingPointId WRITE setCurrentMappingPointId NOTIFY currentMappingPointIdChanged FINAL)
  Q_PROPERTY(bool catheterAlined READ catheterAlined NOTIFY catheterAlinedChanged FINAL)
  Q_PROPERTY(Halve::ProfileType type READ type CONSTANT FINAL)
  Q_PROPERTY(NotebookOptions *notebookOptions READ notebookOptions CONSTANT FINAL)
  Q_PROPERTY(QString albumSavePath READ albumSavePath CONSTANT FINAL)
  Q_PROPERTY(bool pointValidCheck READ pointValidCheck WRITE setPointValidCheck NOTIFY pointValidCheckChanged FINAL)
  Q_PROPERTY(AlarmDb *alarmDb READ alarmDb CONSTANT FINAL)
  Q_PROPERTY(Halve::ChannelMode channelMode READ channelMode WRITE setChannelMode NOTIFY channelModeChanged FINAL)
  Q_PROPERTY(Halve::StorageState storageState READ storageState WRITE setStorageState NOTIFY storageStateChanged FINAL)
  Q_PROPERTY(bool useBackReference READ useBackReference WRITE setUseBackReference NOTIFY useBackReferenceChanged FINAL)

public:
    enum State{
        Init = Qt::UserRole + 1,
        Reproduce,
        Melt
    };
    Q_ENUM(State)

signals:
    void surgicalDateChanged();
    void stateChanged();
    void modelRefChanged(int cnt);
    void channelDataChanged();
    void restrikMappingPointGroupData(const QString &group, const QList<qint64> &ids);

    void mappingTypeChanged();
    void scrollToMappingPoint(qint64 id);

    void pulseChanged();
    void dissolved();

    void useBackReferenceChanged();

    void latPerimeterChanged();
    void currentMappingPointGroupChanged();
    void channelDatasImportSpeedChanged();
    void currentReseauIdChanged();
    void layoutFileChanged();


    void pantSamplingChanged();
    void pantChanged();

    void meltStateChanged();
    void mappingPointAdded(qint64 rowId);
    void mappingPointDeleted(qint64 rowId);

    void renovatingChanged();
    void mappingReseauChanged();

    void dissolveOptionsChanged();

    void scalarOptionsChanged();


    void pulseElectrodeIdChanged();
    void meltOptionsChanged();
    void meltOptionsDbChanged();
    void meltSettingChanged();
    void mappingSettingChanged();
    void centerPointChanged();
    void orientaionMatrixChanged();


    void catheterTempletDbChanged();

    void currentDissolvePointIdChanged();

    void catheterAlinedChanged();
    void currentMappingPointIdChanged();

    void minChanelChanged();


    void notebookOptionsChanged();

    void playVideo(QString fileName);
    void viewPicture(QString fileName);
    void activeDockWidget(const QString &name);
    void openSnapshot(const NotebookSnapshot* snapshot);
    void close();


    void pointValidCheckChanged();


    void channelModeChanged();

    void currentReseauValidChanged();

    void storageStateChanged();
    void action(const QString &name);

    void nameChanged();

public:
    explicit Profile(QObject *parent);
    Profile(const QString &path,QObject *parent);
    ~Profile() = default;
    Q_INVOKABLE virtual void save();
    void backup(const QString &path);
    QString getStoragePath(const QString &path);

    Q_INVOKABLE ProfileSnapshot* loadSnapshot(NotebookSnapshot* snapshot);
    Q_INVOKABLE void freeSnapshot(ProfileSnapshot* snapshot);


    QJsonValue toJson();
    void fromJson(const QJsonObject &json);
    CatheterDb* catheterDb();
    const QString& getId() const;
    FilterOptions* filterOptions();

    SignalSourceOptions* signalSourceOptions();
    EcgOptions* ecgOptions();

    bool pantSampling() const;
    void setPantSampling(bool newPantSampling);

    Mapping* mapping();


    bool catheterAlined();

    MeltDb* meltDb();
    BlackboardDb* blackboardDb();
    ChannelDataDb* channelDataDb();

    int getIncreaseMeltOrder();

    AlbumDb *albumDb();
    QString albumSavePath();

    State state() const;
    void setState(State newState);

    int modelRef() const;
    Q_INVOKABLE void increaseRef();
    Q_INVOKABLE void decreasecRef();

    QString path() const;

    ReproduceOptions *reproduceOptions();

    bool renovating() const;
    void setRenovating(bool newRenovating);

    MappingPointsDb *mappingPointsDb();
    DissolveDb *dissolveDb();

    OscillograpElectrode *oscillograpElectrode();

    Halve::MappingType mappingType() const;
    void setMappingType(Halve::MappingType newMappingType);

    ReseauDb *reseauDb();

    CasketDb *casketDb();
    int pulse() const;
    void setPulse(int newPulse);

    int latPerimeter() const;
    void setLatPerimeter(int newLatPerimeter);

    MappingPointGroupDb *mappingPointGroupDb();

    QString currentMappingPointGroup();
    void setCurrentMappingPointGroup(const QString &newCurrentMappingPointGroup);
    MappingPointGroup* getCurrentMappingPointGroup();

    qint32 channelDatasImportSpeed() const;
    void setChannelDatasImportSpeed(qint32 newChannelDatasImportSpeed);

    qint64 currentReseauId() const;
    void setCurrentReseauId(qint64 newCurrentReseau);
    Reseau* getCurrentReseau();

    QString layoutFile() const;
    void setLayoutFile(const QString &newFile);

    qint32 increaseMappingPointGroupId();

    qint32 increaseReseauId();
    qint32 increaseSnapshotId();

    bool placeDissolvePoint() const;
    void setPlaceDissolvePoint(bool newPlaceDissolvePoint);

    Halve::MeltState meltState() const;
    void setMeltState(const Halve::MeltState& newMeltState);

    qint64 mappingReseau() const;
    void setMappingReseau(qint64 newMappingReseauId);

    DissolveOptions *dissolveOptions();

    BreathOptions *breathOptions();

    ScalarDb *scalarDb();

    ScalarOptions *scalarOptions();

    MeltOptions *meltOptions();
    MeltOptionsDb *meltOptionsDb();

    void setCurrentMeltOptions(qint64 id);
    qint64 currentMeltOptions() const;
    MeltSetting *meltSetting();
    MappingSetting *mappingSetting();

    OscillograpOptions *oscillograpOptions();

    AzimuthDb *azimuthDb();


    QString pulseElectrodeId() const;
    void setPulseElectrodeId(const QString &newPulseElectrodeId);

    void setCenterPoint(const vtkVector3d &pos);
    vtkVector3d getCenterPoint() const;


    bool useBackReference() const;
    void setUseBackReference(bool newUseBackReference);

    StageSettings *stageSettings();

    vtkMatrix4x4* orientaionMatrix() const;
    void setOrientaionMatrix(const vtkMatrix4x4 *newOrientaionMatrix);

    MeltChannelDb *meltChannelDb();


    CatheterTempletDb *catheterTempletDb();

    qint64 currentDissolvePointId() const;
    qint64 currentMappingPointId() const;

    virtual Halve::ProfileType type() const = 0;

    NotebookOptions *notebookOptions();
    NotebookDb *notebookDb();

    bool pointValidCheck() const;
    void setPointValidCheck(bool newPointValidCheck);

    AlarmDb *alarmDb();

    Halve::ChannelMode channelMode() const;
    void setChannelMode(const Halve::ChannelMode &newChannelMode);

    Halve::StorageState storageState() const;
    void setStorageState(const Halve::StorageState &newStorageState);
    QString name() const;
    void setName(const QString &newName);

public slots:
    void setCurrentMappingPointId(qint64 newCurrentMappingPointId);
    void setCurrentDissolvePointId(qint64 newCurrentDissolvePointId);

protected:
    qint32 m_modelRef =0;
    QString m_path;
    QString m_id;
    QString m_name;
    State m_state = State::Init;
    Halve::StorageState m_storageState;
    QPointer<User> m_user;
    QPointer<CatheterDb> m_catheterDb;
    QPointer<FilterOptions> m_filterOptions;
    QPointer<SignalSourceOptions> m_signalSourceOptions;
    QPointer<EcgOptions> m_ecgOptions;
    QPointer<BlackboardDb> m_blackboardDb;
    QPointer<ChannelDataDb> m_channelDataDb;
    QPointer<NotebookDb> m_notebookDb;

    QPointer<MappingPointsDb> m_mappingPointsDb;
    QPointer<Mapping> m_mapping;
    QPointer<ReproduceOptions> m_reproduceOptons;
    QPointer<MeltDb> m_meltDb;
    QPointer<AlbumDb> m_albumDb;
    qint32 m_increaseMeltOrder = 1;
    QPointer<PegPointDb> m_pegPointDb;
    QPointer<DissolveDb> m_dissolveDb;
    QPointer<CasketDb> m_casketDb;
    QPointer<DissolveOptions> m_dissolveOptions;
    QPointer<ScalarOptions> m_scalarOptions;
    QPointer<MeltOptionsDb> m_meltOptionsDb;

    QPointer<OscillograpOptions> m_oscillograpOptions;
    QPointer<ReseauDb> m_reseauDb;
    qint64 m_currentReseauId = 0;
    bool m_useBackReference = true;

    QPointer<OscillograpElectrode> m_oscillograpElectrode;
    QPointer<MappingPointGroupDb> m_mappingPointGroupDb;
    QString m_currentMappingPointGroup;
    QPointer<MeltChannelDb> m_meltChannelDb;

    qint32 m_pulse = 0;
    qint32 m_latPerimeter = 0;
    qint32 m_channelDatasImportSpeed = 0;
    qint64 m_currentMeltOptionsId = 0;
    bool m_renovating = false;

    qint32 m_mappingPointGroupIdIndex = 0;;
    qint32 m_reseauIdIndex = 0;;
    qint32 m_snapshotIdIndex = 0;;
    bool m_pointValidCheck = true;
    bool m_pantSampling = false;
    QPointer<BreathOptions> m_breathOptions;
    QPointer<ScalarDb> m_scalarDb;
    QPointer<AzimuthDb> m_azimuthDb;
    QPointer<StageSettings> m_stageSettings;
    QPointer<CatheterTempletDb> m_catheterTempletDb;
    QPointer<MeltSetting> m_meltSetting;

    QPointer<NotebookOptions> m_notebookOptions;
    QPointer<AlarmDb> m_alarmDb;

    QString m_layoutFile = "layout.json";

    Halve::MappingType m_mappingType = Halve::Lat;
    Halve::MeltState m_meltState = Halve::INIT;
    qint64 m_mappingReseauId = 0;
    qint64 m_currentDissolvePointId = -1;
    qint64 m_currentMappingPointId = -1;

    QString m_pulseElectrodeId = "bodysurface.bodysurface.ecg2";
    Halve::ChannelMode m_channelMode = Halve::CHANNELMODE_BLEND;

    vtkVector3d m_centerPoint{-1,-1,-1};
    vtkSmartPointer<vtkMatrix4x4> m_orientaionMatrix;

    friend class User;
    friend class ProfileManager;
private:
};
