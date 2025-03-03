
#include "catheter/Catheter.h"
#include "combined/TrackData.h"

#include <channel/Channel.h>
#include "catheter/CatheterDb.h"
#include "Combined.h"
#include <vtkMath.h>
#include "combined/BlendMagnetism.h"
#include "combined/BlendDint.h"

QList<TrackData> Combined::convertTrackData(const ChannelTrackData &dataBuffer) {
    QList<TrackData> trackDataList;
    switch(m_channel->mode()) {
    case Halve::CHANNELMODE_MAGNETIC: {
        trackDataList = convertMagneticTrackData(dataBuffer);
        break;
    }
    case Halve::CHANNELMODE_ELECTRICAL: {
        trackDataList = convertElectricalTrackData(dataBuffer);
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
    QList<TrackData> trackDataList;
    for (quint16 seat = 0; seat < ElectricalPortAmount; seat++) {
        Catheter *catheter = m_catheterDb->getDataAtSeat(seat);
        if (catheter == nullptr) {
            continue;
        }
        const ChannelTrackM &m = dataBuffer.m[seat];
        TrackData trackData(catheter, seat);
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
        trackDataList.append(channelBlend.second->process(dataBuffer, m_dnn));
    }
    if (m_blendsMagnetism.size() > 0 && m_blendsDint.size() > 0) {
        auto magnetism = m_blendsMagnetism.front();
        quint16 port = magnetism.first->port();
        if (TrackData::convertStatus(dataBuffer.n[port].status) == Halve::TrackStatus_Valid) {
            if (magnetism.second->trained()) {
                auto elecIdentify = magnetism.second->getElecIdentify();
                auto updater = magnetism.second->getUpdater();
                auto [consultSeat, targetSeat] = magnetism.second->getMagnetismSeat();
                qint32 port = magnetism.first->port();
                for (auto& channelBlend : m_blendsDint) {
                    trackDataList.append(channelBlend.second->process(elecIdentify, updater, port, consultSeat,targetSeat, dataBuffer, m_dnn));
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
