#pragma once

#include <QObject>
#include <vtkSmartPointer.h>
#include <vtkVector.h>
#include <vtkQuaternion.h>
#include <DynamicNearestNeighbor.h>
#include "TrackData.h"
#include "vtkBoundingBox.h"
#include "CatheterTrack.h"
#include "rep_channel_merged.h"

class Profile;
class Catheter;
class CatheterDb;
class vtkPoints;
class vtkQuaterniond;
class vtkTransform;
class Channel;
class Blend;
class BlendMagnetism;
class BlendDint;
class CatheterTrackPackage;
class vtkUnstructuredGrid;
class ReproduceOptions;
class Electric_field_mapping_algorithm;
class BreathOptions;
//class Electric_field_mapping_algorithm;

class Combined : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Profile *profile WRITE setProfile READ profile NOTIFY profileChanged);
    Q_PROPERTY(ChannelReplica::State state READ state NOTIFY stateChanged FINAL)
    Q_PROPERTY(QVector3D centerPoint READ centerPoint WRITE setCenterPoint NOTIFY centerPointChanged FINAL)
    Q_PROPERTY(Halve::TrackStatus reproductCatheterStatus READ reproductCatheterStatus WRITE setReproductCatheterStatus NOTIFY reproductCatheterStatusChanged FINAL)
    Q_PROPERTY(Channel *channel READ channel WRITE setChannel NOTIFY channelChanged FINAL)
    Q_PROPERTY(double displacement READ displacement WRITE setDisplacement NOTIFY displacementChanged FINAL)
    Q_PROPERTY(QVector3D electricCenterShifting READ electricCenterShifting WRITE setElectricCenterShifting NOTIFY electricCenterShiftingChanged FINAL)
    Q_PROPERTY(qint32 magnetismTrainRate READ magnetismTrainRate WRITE setMagnetismTrainRate NOTIFY magnetismTrainRateChanged FINAL)
    Q_PROPERTY(bool isEnvironmentValid READ isEnvironmentValid NOTIFY environmentFlagsChanged FINAL)
    Q_PROPERTY(QVector3D coefficient READ coefficient WRITE setCoefficient NOTIFY coefficientChanged FINAL)
    Q_PROPERTY(bool training READ training WRITE setTraining NOTIFY trainingChanged FINAL)
    Q_PROPERTY(double bloodPoolImpedance READ bloodPoolImpedance WRITE setBloodPoolImpedance NOTIFY bloodPoolImpedanceChanged FINAL)
    Q_PROPERTY(Halve::ChannelMode mode READ mode NOTIFY modeChanged FINAL CONSTANT)

public:
    explicit Combined(QObject *parent = nullptr);
    ~Combined();
    void setProfile(Profile* profile);
    Profile *profile() const;
    Q_INVOKABLE void resetCenterPoint();
    Q_INVOKABLE void resetCenterPoint(double x, double y, double z);
    void resetCenterPoint(double pos[3]);
    void resetCenterPoint(const vtkVector3d &pos);
    void setCenterPoint(const QVector3D &vec);   
    void checkEnvironment();
    void checkReproduceCatheter();
    void checkCatheterPort(Catheter *catheter);

    QVector3D centerPoint() const;
    vtkVector3d getCenterPoint();
    vtkVector3d getCenterPolemicsPoint() const;
    vtkVector3d getCenterPolemicsShifting() const;

    ChannelReplica::State state() const;
    TrackData::List getCurrentTrackDataList() const;

    int interval() const;
    void setInterval(int newInterval);

    Halve::TrackStatus reproductCatheterStatus() const;
    void setReproductCatheterStatus(Halve::TrackStatus newReproductCatheterStatus);

    Channel *channel() const;
    void setChannel(Channel *newChannel);

    Halve::ChannelMode mode() const;

    double displacement() const;
    void setDisplacement(double newDisplacement);

    QVector3D electricCenterShifting() const;
    void setElectricCenterShifting(const QVector3D &pos);

    qint32 magnetismTrainRate() const;
    void setMagnetismTrainRate(qint32 newMagnetismTrainRate);

    bool isEnvironmentValid() const;
    void setEnvironmentFlags(quint64 flags);
    quint64 environmentFlags() const;
    double squaredDistance();


    QVector3D coefficient() const;
    void setCoefficient(const QVector3D& newCoefficient);

    bool training() const;
    void setTraining(bool newTraining);

    double bloodPoolImpedance() const;
    void setBloodPoolImpedance(double newBloodPoolImpedance);

private:
    vtkSmartPointer<vtkPoints> produceElectricityPoints(vtkUnstructuredGrid* grid, vtkVector3d &position, const vtkQuaterniond& quaternion);
    CatheterTrack createCatheterTrack(quint16 seat, Halve::CatheterElectrodeType type, Halve::TrackStatus status, const vtkVector3d &position, const vtkQuaterniond& quaternion, const QString &id);
    void appendElectrodeTrackData(const TrackData &trackData,
                                    Halve::TrackStatus status,
                                  Catheter* catheter,
                                  vtkVector3d &position,QList<CatheterTrack> &catheterTrackList);
    void adjuestTrackAngle(vtkVector3d &position, const vtkQuaterniond &pant10Quaternion);
    void abruptionTrackData(TrackData::List &currentTrackDataList);
    void electricalTrackData(TrackData::List &currentTrackDataList);
    void blendTrackData(const TrackData::List &currentTrackDataList);
    void blendTrackData(const TrackData::List &catheterTrackData, const vtkVector3d &pant10Position, const vtkQuaterniond &pant10Quaternion);
    void blendTrackData(const TrackData &trackData,const vtkVector3d &pant10Position, const vtkQuaterniond &pant10Quaternion,QList<CatheterTrack> &catheterTrackList);
    void blendTraningTrackData(const TrackData::List& currentTrackDataList);
    int electricCSMapping(ChannelTrackData& dataBuffer);

    bool getPant0TrackData(const TrackData::List &catheterTrackData, vtkVector3d &pant10Position, vtkQuaterniond &pant10Quaternion);
    void setTrackCenterPoint(const TrackData &trackData, const vtkVector3d &pant10Position, const vtkQuaterniond &pant10Quaternion);
    QList<TrackData> convertTrackData(const ChannelTrackData &dataBuffer);
    QList<TrackData> convertBlendTrackData(const ChannelTrackData &dataBuffer);
    QList<TrackData> convertBlendTraningTrackData(const ChannelTrackData& dataBuffer);
    QList<TrackData> convertElectricalTrackData(const ChannelTrackData &dataBuffer, int breathGateSync = 1);
    QList<TrackData> convertMagneticTrackData(const ChannelTrackData &dataBuffer);

    void addBlendCatheter(const QList<Catheter *> &catheters);
    void removeBlendCatheter(Catheter *catheter);
    bool isValidBlendCatheter(Catheter *catheter);
    void prepareElectricalCatheter();
    void inspectPantCatheterPort(const QSharedPointer<CatheterTrackPackage> &catheterTracks);
    void inspectReproduceCatheter(const QSharedPointer<CatheterTrackPackage> &catheterTracks);
    void inspectMagneticReproduceCatheter(const QSharedPointer<CatheterTrackPackage> &catheterTracks);
    void inspectElectricalReproduceCatheter(const QSharedPointer<CatheterTrackPackage> &catheterTracks);
    void inspectBlendReproduceCatheter(const QSharedPointer<CatheterTrackPackage> &catheterTracks);

    quint64 checkEnvironmentFlags(quint64 flags, bool s);
    void getCS1AndCS9TrackData(const TrackData::List &catheterTrackData, TrackData &cs4, TrackData &cs8);

    void blendUpdateBloodPoolImpedance(const ChannelTrackData &dataInput);
signals:
    void profileChanged();
    void stateChanged(ChannelReplica::State state);
    void detectorChanged();
    void catheterTrackChanged(const QSharedPointer<CatheterTrackPackage> &trackData);
    void intervalChanged();
    void patnEnableChanged();
    void centerPointChanged();
    void keepSaveChanged();
    void reproductCatheterStatusChanged();
    void channelChanged();
    void displacementChanged();
    void magnetismTrainRateChanged();
    void environmentFlagsChanged();
    void electricCenterShiftingChanged();

    void coefficientChanged();

    void trainingChanged();
    void modeChanged();

    void bloodPoolImpedanceChanged();

public slots:
    void onChannelTrackData(const ChannelTrackData &datas);
    void onCatheterDeleted(Catheter *catheter);
    void onCatheterEmployChanged(Catheter *catheter);
    void onCatheterImported();

private:
    QPointer<Profile> m_profile;
    QPointer<Channel> m_channel;
    QPointer<ReproduceOptions> m_reproduceOptions;
    QPointer<BreathOptions> m_breathOptions;

    QPointer<CatheterDb> m_catheterDb;

    int m_interval = 0;
    bool m_training = false;
    vtkVector3d m_coefficient{ 100, 100, 100 };
    TrackData::List m_currentTrackDataList;
    bool m_keepSave = false;
    Halve::TrackStatus m_reproductCatheterStatus = Halve::TrackStatus_Invalid;
    QPointer<Catheter> m_pantCatheter;
    vtkSmartPointer<vtkTransform> m_transform;
    vtkVector3d m_centerPoint{ -1,-1,-1 };
    vtkVector3d m_centerPolemicsPosition{0,0,0};
    vtkVector3d m_lastCenterPolemicsPosition{ -1,-1,-1 };
    double m_displacement = 20;
    vtkVector3d m_electricCenterShifting{0.0,0.0,0.0};
    qint32 m_magnetismTrainRate = 5;
    quint64 m_environmentFlags = 0;
    quint32 m_reproductCatheterMissedCount = 0;
    float m_bloodPoolImpedance = 0.0;

    QList<QPair<Catheter*, QSharedPointer<BlendMagnetism>>> m_blendsMagnetism;
    QList<QPair<Catheter*, QSharedPointer<BlendDint>>> m_blendsDint;
    Electric_field_mapping_algorithm * m_electricMappingAlgorithm = nullptr;

    QList<ChannelTrackData> m_inputBuffer;
    ys::DynamicNearestNeighbor m_dnn;
};
