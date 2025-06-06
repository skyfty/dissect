#include <QtMath>
#include <vtkNew.h>
#include <vtkPointData.h>
#include <vtkQuaternion.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <catheter/Catheter.h>
#include <catheter/ElectrodeNode.h>
#include "catheter/CatheterDb.h"
#include "catheter/CatheterMagnetism.h"
#include <channel/Channel.h>
#include <study/alarm/Alarm.h>
#include <vtkUnstructuredGrid.h>
#include "Combined.h"
#include "combined/CatheterTrack.h"
#include "profile/Profile.h"
#include "CatheterTrackPackage.h"
#include "combined/Bound.h"
#include "combined/BlendMagnetism.h"
#include "combined/BlendDint.h"
#include "HalveType.h"
#include "halitus/BreathOptions.h"
#include "CheckEnvironmentHelper.h"
#include "catheter/CatheterPerception.h"


Combined::Combined(QObject *parent)
    : QObject(parent)
{
}

Combined::~Combined() {
    delete m_electricMappingAlgorithm;
}

void Combined::setProfile(Profile* profile) {
    if (m_profile == profile) {
        return;
    }
    m_profile = profile;
    m_reproduceOptions = profile->reproduceOptions();
    m_catheterDb = profile->catheterDb();
    m_breathOptions = profile->breathOptions();

    prepareElectricalCatheter();

    emit profileChanged();
}

Profile *Combined::profile() const {
    return m_profile;
}
void Combined::prepareElectricalCatheter() {
    Q_ASSERT(m_catheterDb != nullptr);
    m_pantCatheter = m_catheterDb->getData(PantCatheterID);
    addBlendCatheter(m_catheterDb->getEmployDatas());
    QObject::connect(m_catheterDb, &CatheterDb::deleted, this, &Combined::onCatheterDeleted);
    QObject::connect(m_catheterDb, &CatheterDb::employChanged, this, &Combined::onCatheterEmployChanged);
    QObject::connect(m_catheterDb, &CatheterDb::imported, this, &Combined::onCatheterImported);

}
void Combined::onCatheterImported() {
    Q_ASSERT(m_catheterDb != nullptr);
    addBlendCatheter(m_catheterDb->getEmployDatas());
}

double Combined::bloodPoolImpedance() const
{
    return m_bloodPoolImpedance;
}

void Combined::setBloodPoolImpedance(double newBloodPoolImpedance)
{
    m_bloodPoolImpedance = newBloodPoolImpedance;
}

double Combined::displacement() const
{
    return m_displacement;
}

void Combined::setDisplacement(double newDisplacement)
{
    if (qFuzzyCompare(m_displacement, newDisplacement))
        return;
    m_displacement = newDisplacement;
    emit displacementChanged();
}

Channel *Combined::channel() const
{
    return m_channel;
}

void Combined::setChannel(Channel *newChannel)
{
    if (m_channel == newChannel)
        return;
    m_channel = newChannel;
    QObject::connect(m_channel, &Channel::trackData, this, &Combined::onChannelTrackData);
    QObject::connect(m_channel, &Channel::stateChanged, this, &Combined::stateChanged);
    QObject::connect(m_channel, &Channel::modeChanged, this, &Combined::modeChanged);

    emit channelChanged();
}

Halve::ChannelMode Combined::mode() const {
    return m_channel->mode();
}

Halve::TrackStatus Combined::reproductCatheterStatus() const
{
    return m_reproductCatheterStatus;
}

double  Combined::squaredDistance() {
    return std::sqrt(vtkMath::Distance2BetweenPoints(m_centerPolemicsPosition.GetData(), m_centerPoint.GetData()));
}

void Combined::setReproductCatheterStatus(Halve::TrackStatus newReproductCatheterStatus)
{
    if (m_reproductCatheterStatus == newReproductCatheterStatus)
        return;
    m_reproductCatheterStatus = newReproductCatheterStatus;
    emit reproductCatheterStatusChanged();
}

int Combined::interval() const
{
    return m_interval;
}

CatheterTrack Combined::createCatheterTrack(quint16 seat, Halve::CatheterElectrodeType type, Halve::TrackStatus status, const vtkVector3d &position, const vtkQuaterniond& quaternion, const QString &electrodeId) {
    CatheterTrack track;
    track.setSeat(seat);
    track.setPosition(position);
    track.setQuaternion(quaternion);
    track.setType(type);
    track.setStatus(status);
    track.setElectrodeId(electrodeId);
    return track;
}

extern const char *TypesPointDataName ;

vtkSmartPointer<vtkPoints> Combined::produceElectricityPoints(vtkPoints* points, vtkVector3d& position, const vtkQuaterniond& quaternion) {
    vtkNew<vtkPolyData> gridPolyData;
    gridPolyData->SetPoints(points);

    vtkNew<vtkTransform> transform;
    transform->Translate(position.GetData());
    double direction[4]{};
    direction[0] = vtkMath::DegreesFromRadians(quaternion.GetRotationAngleAndAxis(direction + 1));
    transform->RotateWXYZ(direction[0], direction[1], direction[2], direction[3]);

    vtkNew<vtkTransformPolyDataFilter> transformFilter;
    transformFilter->SetInputData(gridPolyData);
    transformFilter->SetTransform(transform);
    transformFilter->Update();
    return transformFilter->GetOutput()->GetPoints();
}

void Combined::appendElectrodeTrackData(const TrackData &trackData, Halve::TrackStatus status, Catheter *catheter,vtkVector3d& position, QList<CatheterTrack> &catheterTrackList) {
    Q_ASSERT(catheter != nullptr);
    CatheterMould* catheterMould = catheter->catheterMould();
    vtkSmartPointer<vtkUnstructuredGrid> mouldGrid = catheterMould->grid();
    vtkQuaterniond quaternion;
    trackData.getQuaternion(quaternion);

    vtkNew<vtkPoints> mouldPoints;
    for (vtkIdType pointId = 0; pointId < mouldGrid->GetNumberOfPoints(); ++pointId) {
        vtkSmartPointer<CatheterPerception> perception = catheterMould->getPerception(pointId);
        if (perception->mode() == 0) {
            mouldPoints->InsertNextPoint(mouldGrid->GetPoint(pointId));
        }
    }
    vtkSmartPointer<vtkPoints> targetPoints = produceElectricityPoints(mouldPoints, position, quaternion);
    vtkDataArray* types = mouldGrid->GetPointData()->GetArray(TypesPointDataName);

    for (vtkIdType pointId = 0; pointId < targetPoints->GetNumberOfPoints(); ++pointId) {
        Halve::CatheterElectrodeType type = (Halve::CatheterElectrodeType)types->GetTuple1(pointId);
        quint16 seat = catheter->bseat() + pointId;
        ElectrodeNode* electrodeNode = catheter->getElectrodeNode(seat);
        vtkVector3d position{};
        targetPoints->GetPoint(pointId, position.GetData());
        vtkQuaterniond quaternion{};
        catheterTrackList.append(createCatheterTrack(seat, type, status, position, quaternion, electrodeNode->id()));
    }

}

void Combined::inspectPantCatheterPort(const QSharedPointer<CatheterTrackPackage> &catheterTracks) {
    const QList<CatheterTrack>& tracks = catheterTracks->getTracks(m_pantCatheter);
    CheckEnvironmentHelper flags(m_environmentFlags);
    if (tracks.size() != 2) {
        flags(Halve::AN_REFERENCE_CATHETER_CONFIGURATION_ERROR, true);
    } else {
        flags(Halve::AN_BACK_REFERENCE_CATHETER_CONFIGURATION_ERROR, tracks[MagnetismPant0Port].status() != Halve::TrackStatus_Valid);
        flags(Halve::AN_PRETHORACIC_REFERENCE_CATHETER_CONFIGURATION_ERROR, tracks[MagnetismPant1Port].status() != Halve::TrackStatus_Valid);
    }
    setEnvironmentFlags(flags);
}

void Combined::abruptionTrackData(TrackData::List &currentTrackDataList) {
    vtkVector3d pant0Position{0.0,0.0,0.0};
    vtkQuaterniond pant0Quaternion{0.0,0.0,0.0,0.0};
    bool pantResult = m_profile->useBackReference() ? getPant0TrackData(currentTrackDataList, pant0Position, pant0Quaternion):true;
    QSharedPointer<CatheterTrackPackage> catheterTracks(new CatheterTrackPackage());
    for(TrackData &trackData : currentTrackDataList) {
        Catheter* catheter = trackData.catheter();
        QList<CatheterTrack> &catheterTrackList = catheterTracks->getTracks(catheter);
        vtkVector3d position{};
        vtkQuaterniond quaternion{};
        Halve::TrackStatus status = trackData.getStatus();
        if (status == Halve::TrackStatus_Valid) {
            trackData.getPosition(position);
            trackData.getQuaternion(quaternion);
        }

        switch(trackData.port()) {
        case MagnetismPant0Port: {
            catheterTrackList.append(createCatheterTrack(trackData.port(), Halve::CET_PANT, status, position, quaternion, Pant0ID));
            break;
        }
        case MagnetismPant1Port: {
            vtkMath::Subtract(position, pant0Position, position);
            catheterTrackList.append(createCatheterTrack(trackData.port(), Halve::CET_PANT, status, position, quaternion,Pant1ID));
            break;
        }
        default: {
            if (pantResult) {
                if (m_centerPoint[0] == -1) {
                    if (m_reproduceOptions->catheterId() == catheter->id()) {
                        setTrackCenterPoint(trackData, pant0Position, pant0Quaternion);
                    }
                    return;
                }

                if (catheter->employ()) {
                    vtkMath::Subtract(position, m_centerPolemicsPosition.GetData(), position);
                    appendElectrodeTrackData(trackData, status, catheter, position, catheterTrackList);
                }
            }
            break;
        }
        }
    }
    inspectPantCatheterPort(catheterTracks);
    inspectReproduceCatheter(catheterTracks);

    emit catheterTrackChanged(catheterTracks);
}


void Combined::electricalTrackData(TrackData::List &currentTrackDataList) {
    QSet<Catheter*> catheters;
    QSharedPointer<CatheterTrackPackage> catheterTracks(new CatheterTrackPackage());
    for(const TrackData &trackData:currentTrackDataList) {
        Catheter* catheter = trackData.catheter();
        if (!catheter->employ()) {
            continue;
        }
        Halve::TrackStatus status = trackData.getStatus();
        if (trackData.getFlags() & TrackData::ELECTRICAL_IMPEDANCE) {
            status = Halve::TrackStatus_Missing;
        }
        QList<CatheterTrack> &catheterTrackList = catheterTracks->getTracks(catheter);

        quint16 port = trackData.port();
        vtkVector3d position{};
        trackData.getPosition(position);
        quint16 seatIdx = port - catheter->bseat();
        if (trackData.catheter()->getType() == CSCatheterType && seatIdx == 5) {
            m_centerPolemicsPosition = position;
            if (m_centerPoint[0] == -1) {
                m_centerPoint = m_centerPolemicsPosition;
                emit centerPointChanged();
                return;
            }
        }
        vtkQuaterniond quaternion{};
        trackData.getQuaternion(quaternion);
        Halve::CatheterElectrodeType type = catheter->catheterMould()->getType(seatIdx);
        ElectrodeNode* electrodeNode = catheter->getElectrodeNode(port);
        vtkMath::Subtract(position, m_centerPoint.GetData(), position);
        catheterTrackList.append(createCatheterTrack(port, type, status, position, quaternion, electrodeNode->id()));
        catheters.insert(catheter);
    }

    //for (Catheter* catheter : catheters) {
    //    CatheterMould* catheterMould = catheter->catheterMould();
    //    vtkUnstructuredGrid* mouldGrid = catheterMould->grid();
    //    QList<CatheterTrack>& trackDatas = catheterTracks->getTracks(catheter);

    //    vtkNew<vtkPoints> sourcePoints, targetPoints;
    //    for (vtkIdType pointId = 0; pointId < mouldGrid->GetNumberOfPoints(); ++pointId) {
    //        vtkIdType splineValue = -1;
    //        vtkSmartPointer<CatheterPerception> perception = catheterMould->getPerception(pointId);
    //        if (perception->mode() != CatheterPerception::EXPLICIT || !perception->getSpline(splineValue)) {
    //            continue;
    //        }
    //        auto iter = std::find_if(std::begin(trackDatas), std::end(trackDatas), [&](const CatheterTrack& track) {
    //            quint16 seatIdx = track.seat() - catheter->bseat();
    //            return seatIdx == splineValue;
    //        });
    //        if (iter == std::end(trackDatas)) {
    //            continue;
    //        }
    //        targetPoints->InsertNextPoint(targetPosition.GetData());
    //        vtkVector3d sourcePosition;
    //        mouldGrid->GetPoint(pointId, sourcePosition.GetData());
    //        sourcePosition = vtkutil::randomUndulation(sourcePosition);
    //        sourcePoints->InsertNextPoint(sourcePosition.GetData());
    //    }

    //}   


    if (m_centerPoint[0] != -1) {
        QList<CatheterTrack>& pantCatheterTrackList = catheterTracks->getTracks(m_pantCatheter);
        TrackData cs1,cs9;
        getCS1AndCS9TrackData(currentTrackDataList, cs1, cs9);
        vtkVector3d position{};
        cs1.getPosition(position);
        vtkQuaterniond quaternion{};
        cs1.getQuaternion(quaternion);
        pantCatheterTrackList.append(createCatheterTrack(MagnetismPant0Port, Halve::CET_PANT, Halve::TrackStatus_Valid, position, quaternion, Pant0ID));
        cs9.getPosition(position);
        cs9.getQuaternion(quaternion);
        pantCatheterTrackList.append(createCatheterTrack(MagnetismPant1Port, Halve::CET_PANT, Halve::TrackStatus_Valid, position, quaternion, Pant1ID));
    }

    inspectReproduceCatheter(catheterTracks);

    emit catheterTrackChanged(catheterTracks);
}
void Combined::getCS1AndCS9TrackData(const TrackData::List &catheterTrackData, TrackData &cs4, TrackData &cs8) {
    for(const TrackData &trackData : catheterTrackData) {
        Catheter* catheter = trackData.catheter();
        if (!catheter->employ() || catheter->getType() != CSCatheterType) {
            continue;
        }
        quint16 port = trackData.port();
        quint16 seatIdx = port - catheter->bseat();
        if (seatIdx == 1) {
            cs4 = trackData;
        } else if (seatIdx == 9) {
            cs8 = trackData;
        }
    }
}

void Combined::blendUpdateBloodPoolImpedance(const ChannelTrackData &dataInput)
{
    Catheter* reproduceCatheter = m_catheterDb->getData(m_reproduceOptions->catheterId());
    if (reproduceCatheter == nullptr || !reproduceCatheter->employ() || reproduceCatheter->getAmount() < 2)
        return;
    int startSeat = reproduceCatheter->bseat();
    vtkVector3f diff;
    vtkMath::Subtract(dataInput.m[startSeat].pos.GetData(), dataInput.m[startSeat + 1].pos.GetData(), diff.GetData());
    double componentSum = vtkMath::Dot(diff.GetData(), diff.GetData());
    double impedance = std::sqrt(componentSum);
    setBloodPoolImpedance(impedance);
}

bool Combined::getPant0TrackData(const TrackData::List &catheterTrackData, vtkVector3d &pant10Position, vtkQuaterniond &pant10Quaternion) {
    auto iter = std::find_if(catheterTrackData.begin(), catheterTrackData.end(), [](const TrackData &trackData){
        return trackData.port() == MagnetismPant0Port;
    });
    if (iter != catheterTrackData.end()) {
        iter->getPosition(pant10Position);
        //iter->getQuaternion(pant10Quaternion);
        return true;
    }
    return false;
}
void Combined::setTrackCenterPoint(const TrackData &trackData, const vtkVector3d &pant0Position, const vtkQuaterniond &pant0Quaternion)  {
    trackData.getPosition(m_centerPoint);
    m_centerPolemicsPosition = m_centerPoint;
    m_lastCenterPolemicsPosition = m_centerPolemicsPosition;
    emit centerPointChanged();
}

void Combined::blendTrackData(const TrackData::List &currentTrackDataList) {
    vtkVector3d pant0Position{0.0,0.0,0.0};
    vtkQuaterniond pant0Quaternion{0.0,0.0,0.0,0.0};
    bool pantResult = m_profile->useBackReference() ? getPant0TrackData(currentTrackDataList, pant0Position, pant0Quaternion):true;
    if (pantResult) {
        blendTrackData(currentTrackDataList, pant0Position, pant0Quaternion);
    }
}
void Combined::blendTrackData(const TrackData &trackData,const vtkVector3d &pant10Position, const vtkQuaterniond &pant10Quaternion,QList<CatheterTrack> &catheterTrackList) {
    Catheter* catheter = trackData.catheter();
    vtkVector3d position{};
    trackData.getPosition(position);
    vtkMath::Subtract(position, m_centerPolemicsPosition.GetData(), position);
    vtkQuaterniond quaternion{};
    trackData.getQuaternion(quaternion);
    quint16 port = trackData.port();
    quint16 seatIdx = port - catheter->bseat();
    ElectrodeNode* electrodeNode = catheter->getElectrodeNode(port);
    Halve::CatheterElectrodeType type = catheter->catheterMould()->getType(seatIdx);
    catheterTrackList.append(createCatheterTrack(port, type, trackData.getStatus(), position, quaternion, electrodeNode->id()));
}

void Combined::blendTrackData(const TrackData::List &currentTrackDataList, const vtkVector3d & pant0Position, const vtkQuaterniond &pant0Quaternion) {
    QSharedPointer<CatheterTrackPackage> catheterTracks(new CatheterTrackPackage());
    for(const TrackData &trackData : currentTrackDataList) {
        Catheter* catheter = trackData.catheter();
        Q_ASSERT(catheter != nullptr);
        QList<CatheterTrack> &catheterTrackList = catheterTracks->getTracks(catheter);
        quint16 port = trackData.port();
        vtkVector3d position{};
        trackData.getPosition(position);
        vtkQuaterniond quaternion{};
        trackData.getQuaternion(quaternion);
        if (catheter == m_pantCatheter) {
            switch(port) {
            case MagnetismPant0Port: {
                catheterTrackList.append(createCatheterTrack(port,Halve::CET_PANT, trackData.getStatus(), position, quaternion,Pant0ID));
                break;
            }
            case MagnetismPant1Port: {
                vtkMath::Subtract(position, pant0Position, position);
                catheterTrackList.append(createCatheterTrack(port, Halve::CET_PANT, trackData.getStatus(), position, quaternion, Pant1ID));
                break;
            }
            }
        } else {
            if (m_centerPoint[0] == -1) {
                if (m_reproduceOptions->catheterId() == catheter->id()) {
                    setTrackCenterPoint(trackData, pant0Position, pant0Quaternion);
                }
                return;
            }
            if (catheter->employ()) {
                blendTrackData(trackData, pant0Position, pant0Quaternion, catheterTrackList);
            }
        }
    }
    inspectReproduceCatheter(catheterTracks);
    inspectPantCatheterPort(catheterTracks);

    emit catheterTrackChanged(catheterTracks);
}

void Combined::blendTraningTrackData(const TrackData::List& currentTrackDataList) {
    QSharedPointer<CatheterTrackPackage> catheterTracks(new CatheterTrackPackage());
    for (const TrackData& trackData : currentTrackDataList) {
        Halve::TrackStatus status = trackData.getStatus();  
        Catheter* catheter = trackData.catheter();
        if (status != Halve::TrackStatus_Valid || !catheter->employ()) {
            continue;
        }
        vtkVector3d position{};
        trackData.getPosition(position);
        if (m_centerPoint[0] == -1) {
            m_centerPoint = position;
            emit centerPointChanged();
        }
        vtkMath::Add(position, m_electricCenterShifting, position);
        vtkQuaterniond quaternion{};
        trackData.getQuaternion(quaternion);
        QList<CatheterTrack>& catheterTrackList = catheterTracks->getTracks(catheter);
        quint16 port = trackData.port();
        quint16 seatIdx = port - catheter->bseat();
        Halve::CatheterElectrodeType type = catheter->catheterMould()->getType(seatIdx);
        ElectrodeNode* electrodeNode = catheter->getElectrodeNode(port);
        vtkMath::Subtract(position, m_centerPoint.GetData(), position);
        catheterTrackList.append(createCatheterTrack(port, type, status, position, quaternion, electrodeNode->id()));
    }

    if (m_centerPoint[0] != -1) {
        QList<CatheterTrack>& pantCatheterTrackList = catheterTracks->getTracks(m_pantCatheter);
        vtkVector3d position{};
        vtkMath::Subtract(m_centerPoint, m_centerPoint.GetData(), position);
        vtkQuaterniond quaternion{};
        pantCatheterTrackList.append(createCatheterTrack(MagnetismPant0Port, Halve::CET_PANT, Halve::TrackStatus_Valid, position, quaternion, Pant0ID));
        pantCatheterTrackList.append(createCatheterTrack(MagnetismPant1Port, Halve::CET_PANT, Halve::TrackStatus_Valid, position, quaternion, Pant1ID));
    }
    setEnvironmentFlags(0);
    setReproductCatheterStatus(Halve::TrackStatus_Valid);
    emit catheterTrackChanged(catheterTracks);
}

void Combined::checkEnvironment() {
    CheckEnvironmentHelper flags(m_environmentFlags);
    if (m_channel->mode() == Halve::CHANNELMODE_ELECTRICAL) {
        flags(Halve::AN_THE_CS_CATHETER_CARTRIDGE_IS_NOT_CONFIGURED, m_catheterDb->getCSDatas().isEmpty());
        flags(Halve::AN_REFERENCE_CATHETER_CONFIGURATION_ERROR, false);
    } else {
        flags(Halve::AN_ELECTRIC_REFERENCE_POSITION_IS_TOO_OFFSET|Halve::AN_THE_CS_CATHETER_CARTRIDGE_IS_NOT_CONFIGURED, false);
    }

    setEnvironmentFlags(flags);
}

void Combined::checkCatheterPort(Catheter *inCatheter) {
    CheckEnvironmentHelper flags(m_environmentFlags);
    QList<Catheter*> catheters = m_catheterDb->getDatasAtPort(inCatheter->port());
    flags(Halve::AN_CATHETER_PORT_REUSED_ERROR, catheters.size() > 1);
    setEnvironmentFlags(flags);
}

void Combined::checkReproduceCatheter() {
    CheckEnvironmentHelper flags(m_environmentFlags);
    QString catheterId = m_reproduceOptions->catheterId();
    if (m_catheterDb->size() != 0 && !catheterId.isEmpty()) {
        Catheter* reproduceCatheter = m_catheterDb->getData(catheterId);
        if (reproduceCatheter != nullptr) {
            if (m_channel->mode() == Halve::CHANNELMODE_BLEND) {
                flags(Halve::AN_REPRODUCE_CATHETER_CONSULT_TARGET_CONFIG_ERROR, !isValidBlendCatheter(reproduceCatheter));
            } else {
                flags(Halve::AN_REPRODUCE_CATHETER_ERROR, false);
            }
            flags(Halve::AN_REPRODUCE_CATHETER_NOT_IN_CASKET_ERROR, reproduceCatheter->bseat() == CatheterNoSeta);
        } else {
            flags(Halve::AN_REPRODUCE_CATHETER_CONFIGURATION_ERROR, true);
        }
    } else {
        flags(Halve::AN_REPRODUCE_CATHETER_ERROR, false);
    }
    setEnvironmentFlags(flags);
}

quint64 Combined::environmentFlags() const {
    return m_environmentFlags;
}

bool Combined::isEnvironmentValid() const
{
    return m_environmentFlags == 0;
}

void Combined::setEnvironmentFlags(quint64 flags)
{
    if (m_environmentFlags == flags)
        return;
    m_environmentFlags = flags;
    emit environmentFlagsChanged();
}

void Combined::onCatheterEmployChanged(Catheter *catheter) {
    Q_ASSERT(catheter != nullptr);
    if (catheter->employ()) {
        addBlendCatheter({catheter});
    } else {
        removeBlendCatheter(catheter);
    }
}


QVector3D Combined::coefficient() const
{
    QVector3D pos;
    if (m_coefficient[0] != -1) {
        pos.setX(m_coefficient[0]);
        pos.setY(m_coefficient[1]);
        pos.setZ(m_coefficient[2]);
    }
    return pos;
}

void Combined::setCoefficient(const QVector3D& vec)
{
    m_coefficient.Set(vec.x(), vec.y(), vec.z());
}


bool Combined::isValidBlendCatheter(Catheter *catheter) {
    Q_ASSERT(catheter != nullptr);
    auto magnetism = catheter->catheterMagnetism();;
    return catheter->getAmount() > 0 &&
           magnetism->consult() != -1 &&
           magnetism->target() != -1 &&
           magnetism->consult() != magnetism->target();
}

void Combined::addBlendCatheter(const QList<Catheter *> &catheters) {
    for(Catheter *catheter:catheters) {
        if (catheter->magnetism()) {
            if (isValidBlendCatheter(catheter)) {
                auto magnetismBlend = QSharedPointer<BlendMagnetism>::create(m_profile, catheter);
                magnetismBlend->startTrainTimer(std::chrono::seconds(m_magnetismTrainRate));
                m_blendsMagnetism.append(std::make_pair(catheter,magnetismBlend));
            }
        } else {
            m_blendsDint.append(std::make_pair(catheter, QSharedPointer<BlendDint>::create(m_profile, catheter)));
        }
    }
}
void Combined::removeBlendCatheter(Catheter *catheter) {
    Q_ASSERT(catheter != nullptr);
    auto iter = std::find_if(m_blendsMagnetism.begin(), m_blendsMagnetism.end(),[catheter](const auto &item){
        return item.first == catheter;
    });
    if (iter != m_blendsMagnetism.end()) {
        m_blendsMagnetism.erase(iter);
    } else {
        auto iter2 = std::find_if(m_blendsDint.begin(), m_blendsDint.end(),[catheter](const auto &item){
            return item.first == catheter;
        });
        if (iter2 != m_blendsDint.end()) {
            m_blendsDint.erase(iter2);
        }
    }
}

void Combined::onCatheterDeleted(Catheter *catheter) {
    removeBlendCatheter(catheter);
}

qint32 Combined::magnetismTrainRate() const
{
    return m_magnetismTrainRate;
}

void Combined::setMagnetismTrainRate(qint32 newMagnetismTrainRate)
{
    if (m_magnetismTrainRate == newMagnetismTrainRate)
        return;
    m_magnetismTrainRate = newMagnetismTrainRate;
    for(auto blend:m_blendsMagnetism) {
        blend.second->startTrainTimer(std::chrono::seconds(m_magnetismTrainRate));
    }
    emit magnetismTrainRateChanged();
}

void Combined::onChannelTrackData(const ChannelTrackData &dataInput) {
    if (m_profile->useBackReference())
    {
        if (m_channel->mode() == Halve::CHANNELMODE_MAGNETIC || m_channel->mode() == Halve::CHANNELMODE_BLEND)
        {
            auto& p0 = dataInput.n[MagnetismPant0Port];
            for (int i=0; i<MagnetismPortAmount; ++i) {
                if (i == MagnetismPant0Port || i == MagnetismPant1Port)
                    continue;
                auto& pi = dataInput.n[i];
                vtkMath::Subtract(pi.pos.GetData(), p0.pos.GetData(), (float*)pi.pos.GetData());
            }
        }
    }


    m_currentTrackDataList = convertTrackData(dataInput);
    switch(m_channel->mode()) {
    case Halve::CHANNELMODE_MAGNETIC: {
        abruptionTrackData(m_currentTrackDataList);
        break;
    }
    case Halve::CHANNELMODE_ELECTRICAL: {
        electricalTrackData(m_currentTrackDataList);
        break;
    }
    case Halve::CHANNELMODE_BLEND: {
        if (m_training) {
            blendTraningTrackData(m_currentTrackDataList);
        } else {
            blendTrackData(m_currentTrackDataList);
        }
        break;
    }
    }
}


ChannelReplica::State Combined::state() const {
    if (m_channel == nullptr) {
        return ChannelReplica::State_Shutdown;
    }
    return m_channel->state();

}

TrackData::List Combined::getCurrentTrackDataList() const {
    return m_currentTrackDataList;
}

const static vtkVector3d OrgCenterPoint{-1,-1,-1};
void Combined::resetCenterPoint() {
    m_centerPoint = OrgCenterPoint;
    m_lastCenterPolemicsPosition = OrgCenterPoint;
    m_centerPolemicsPosition = OrgCenterPoint;
    emit centerPointChanged();
}

void Combined::resetCenterPoint(double x, double y, double z) {
    m_centerPoint.Set(x,y, z);
    m_centerPolemicsPosition = m_centerPoint;
    m_lastCenterPolemicsPosition = m_centerPolemicsPosition;
}

void Combined::resetCenterPoint(double pos[3])
{
    resetCenterPoint(pos[0], pos[1], pos[2]);
}

void Combined::resetCenterPoint(const vtkVector3d &pos)
{
    resetCenterPoint(pos.GetX(), pos.GetY(), pos.GetZ());

}

void Combined::setCenterPoint(const QVector3D &vec)
{
    resetCenterPoint(vec.x(), vec.y(), vec.z());
}

QVector3D Combined::centerPoint() const
{
    QVector3D pos;
    if (m_centerPoint[0] != -1) {
        pos.setX(m_centerPoint[0]);
        pos.setY(m_centerPoint[1]);
        pos.setZ(m_centerPoint[2]);
    }
    return pos;
}

vtkVector3d Combined::getCenterPoint()
{
    return m_centerPoint;
}

vtkVector3d Combined::getCenterPolemicsPoint() const
{
    return m_centerPolemicsPosition;

}

QVector3D Combined::electricCenterShifting() const
{
    QVector3D pos;
    if (m_electricCenterShifting[0] != -1) {
        pos.setX(m_electricCenterShifting[0]);
        pos.setY(m_electricCenterShifting[1]);
        pos.setZ(m_electricCenterShifting[2]);
    }
    return pos;
}
void Combined::setElectricCenterShifting(const QVector3D &vec) {
    m_electricCenterShifting.Set(vec.x(), vec.y(), vec.z());
}

vtkVector3d Combined::getCenterPolemicsShifting() const {
    vtkVector3d result{};
    vtkMath::Subtract(m_lastCenterPolemicsPosition.GetData(), m_centerPolemicsPosition, result.GetData());
    return result;
}

void Combined::setInterval(int newInterval) {
    if (m_interval == newInterval)
        return;
    m_interval = newInterval;
    emit intervalChanged();
}
