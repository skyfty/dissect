
#include "catheter/Catheter.h"
#include "combined/TrackData.h"

#include "halitus/BreathOptions.h"
#include "profile/Profile.h"
#include <channel/Channel.h>
#include "catheter/CatheterDb.h"
#include <vtkMath.h>
#include "combined/BlendMagnetism.h"
#include "combined/BlendDint.h"
#include "Electric_field_define.h"
#include "Electric_field_mapping_algorithm.h"

#include "Combined.h"
QList<TrackData> Combined::convertTrackData(const ChannelTrackData &dataInput) {
    QList<TrackData> trackDataList;
    const int delay = 1;  // 电落后磁1个周期
    m_inputBuffer.push_back(dataInput);
    if (m_inputBuffer.size() > delay + 1)
        m_inputBuffer.pop_front();
    if (m_inputBuffer.size() < delay + 1)
        return trackDataList;
    const auto& d1 = m_inputBuffer.front();
    const auto& d2 = m_inputBuffer.back();

    // 重新拼接数据
    ChannelTrackData dataBuffer;
    dataBuffer.m_id = d2.m_id;
    dataBuffer.m_time = d2.m_time;
    std::memcpy(dataBuffer.m, d2.m, sizeof(d2.m));
    std::memcpy(dataBuffer.n, d1.n, sizeof(d1.n));

    switch(m_channel->mode()) {
    case Halve::CHANNELMODE_ELECTRICAL: {
        trackDataList = convertElectricalTrackData(dataBuffer);
        break;
    }
    case Halve::CHANNELMODE_MAGNETIC: {
        trackDataList = convertMagneticTrackData(dataBuffer);
        break;
    }
    case Halve::CHANNELMODE_BLEND: {
        if (m_training) {
            trackDataList = convertBlendTraningTrackData(dataBuffer);
        } else {
            trackDataList = convertBlendTrackData(dataBuffer);
        }
        break;
    }
    }
    return trackDataList;
}

QList<TrackData> Combined::convertMagneticTrackData(const ChannelTrackData &dataBuffer) {
    QList<TrackData> trackDataList;
    for (quint16 port = 0; port < MagnetismPortAmount; ++port) {
        QList<Catheter *> catheterList;
        if ((port == MagnetismPant0Port || port == MagnetismPant1Port)) {
            catheterList.append(m_pantCatheter);
        } else {
            catheterList = m_catheterDb->getDatasAtPort(port);
        }
        for(auto catheter : catheterList) {
            const ChannelTrackN& nPoint = dataBuffer.n[port];
            TrackData trackData(catheter, port);
            trackData.setPosition(nPoint.reversal());
            trackData.setQuaternion(nPoint.quaternion);
            trackData.setStatus(nPoint.status);
            trackDataList.push_back(trackData);
        }
    }
    return trackDataList;
}

QList<TrackData> Combined::convertElectricalTrackData(const ChannelTrackData &dataBuffer) {
    std::vector<ChannelTrackM> mdata(dataBuffer.m, dataBuffer.m + ElectricalPortAmount);
    int breathGateSync = 1;
    QList<Catheter*> csCatheters  = m_catheterDb->getCSDatas();
    if (csCatheters.size() > 0 && m_reproductCatheterStatus == Halve::TrackStatus_Valid) {
        float bloodPoolImpedance;
        if (m_electricMappingAlgorithm == nullptr) {
            m_electricMappingAlgorithm = new Electric_field_mapping_algorithm();
        }
        Catheter* csCatheter = csCatheters[0];
        Catheter* reproduceCatheter = m_catheterDb->getData(m_reproduceOptions->catheterId());

        float position_zero_out[2]{};
        m_electricMappingAlgorithm->Electric_field_mapping_algorithm_all(
            m_profile->state() == Profile::Reproduce ? OPERATION_STATE::MODELING : OPERATION_STATE::MAPPING,
            reproduceCatheter->bseat(),
            reproduceCatheter->getAmount(),
            csCatheter->bseat(),
            csCatheter->getAmount(),
            static_cast<RESPIRATORY_MODE>(m_breathOptions->respiratoryMode()),
            const_cast<float*>(dataBuffer.m[0].pos.GetData()),
            &breathGateSync,
            reinterpret_cast<float*>(mdata[0].pos.GetData()),
            &bloodPoolImpedance,
            position_zero_out);
        setBloodPoolImpedance(bloodPoolImpedance);
    }

    QList<TrackData> trackDataList;
    for (quint16 seat = 0; seat < ElectricalPortAmount; seat++) {
        Catheter *catheter = m_catheterDb->getDataAtSeat(seat);
        if (catheter == nullptr) {
            continue;
        }
        const ChannelTrackM &m = mdata[seat];
        TrackData trackData(catheter, seat);
        trackData.setFlags(breathGateSync == 0 ? TrackData::ELECTRICAL_IMPEDANCE : 0);
        trackData.setStatus(m.valid()?Halve::TrackStatus_Valid:Halve::TrackStatus_Invalid);
        vtkVector3<float> mpos = m.reversal();
        mpos[0] /= m_coefficient[0];
        mpos[1] /= m_coefficient[1];
        mpos[2] /= m_coefficient[2];
        trackData.setPosition(mpos);
        trackDataList.push_back(trackData);
    }
    return trackDataList;
}
QList<TrackData> Combined::convertBlendTraningTrackData(const ChannelTrackData& dataBuffer) {
    QList<TrackData> trackDataList;
    for (auto& channelBlend : m_blendsMagnetism) {
        quint16 port = channelBlend.first->port();
        if (TrackData::convertStatus(dataBuffer.n[port].status) != Halve::TrackStatus_Valid) {
            continue;
        }
        channelBlend.second->appendTrainData(dataBuffer);
        trackDataList.append(convertElectricalTrackData(dataBuffer));
    }
    return trackDataList;
}

QList<TrackData> Combined::convertBlendTrackData(const ChannelTrackData &dataBuffer) {
    QList<TrackData> trackDataList;
    for(quint16 port : {MagnetismPant0Port,MagnetismPant1Port}) {
        const ChannelTrackN &n = dataBuffer.n[port];
        TrackData trackData(m_pantCatheter, port);
        trackData.setPosition(n.reversal());
        trackData.setQuaternion(n.quaternion);
        trackData.setStatus(n.status);
        trackDataList.push_back(trackData);
    }

    for(auto &channelBlend : m_blendsMagnetism) {
        quint16 port = channelBlend.first->port();
        if (TrackData::convertStatus(dataBuffer.n[port].status) != Halve::TrackStatus_Valid) {
            continue;
        }
        trackDataList.append(channelBlend.second->process(dataBuffer, &m_dnn));
    }
    if (m_blendsMagnetism.size() > 0 && m_blendsDint.size() > 0) {
        auto magnetism = m_blendsMagnetism.front();
        quint16 port = magnetism.first->port();
        if (TrackData::convertStatus(dataBuffer.n[port].status) == Halve::TrackStatus_Valid) {
            if (magnetism.second->trained()) {
                auto elecIdentify = magnetism.second->getElecIdentify();
                auto updater = magnetism.second->getUpdater();
                auto [consultSeat, targetSeat] = magnetism.second->getMagnetismSeat();
                auto mag = magnetism.second->getMagnetism();
                qint32 port = magnetism.first->port();
                for (auto& channelBlend : m_blendsDint) {
                    trackDataList.append(channelBlend.second->process(elecIdentify, updater, port, consultSeat,targetSeat, dataBuffer, &m_dnn));
                }
            }
        }
    }
    return trackDataList;
}

bool Combined::training() const
{
    return m_training;
}

void Combined::setTraining(bool newTraining)
{
    if (m_training == newTraining)
        return;
    m_training = newTraining;
    for(auto &channelBlend : m_blendsMagnetism) {
        if (newTraining) {
            channelBlend.second->startTrain();
        } else {
            channelBlend.second->finishTrain();
        }
    }
    resetCenterPoint();
    emit trainingChanged();
}
