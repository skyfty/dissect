#include "ChannelHost.h"
#include "channel/ChannelDataDb.h"
#include "channel/ChannelTrackDb.h"
#include <utility/IOWorker.h>
#include "channel/ChannelTrack.h"
#include "channel/ChannelTrackData.h"

constexpr quint32 ChannelDataAmount = 50;

ChannelHost::ChannelHost(QObject *parent)
    : ChannelSimpleSource{parent}
{
    m_lookbackTimer = new QTimer(this);
    QObject::connect(m_lookbackTimer, &QTimer::timeout, this, &ChannelHost::onLookbackTimeout);
    m_keepliveTimer = new QTimer(this);
    QObject::connect(m_keepliveTimer, &QTimer::timeout, this, &ChannelHost::onKeepliveTimerEvent);
}

ChannelHost::~ChannelHost() {
}

void ChannelHost::init(const QString& profilePath, Halve::ChannelMode mode)
{
    m_mode = mode;
    m_profilePath = profilePath;
    m_channelDataDb = new ChannelDataDb(m_profilePath, this);
    m_channelTrackDb = new ChannelTrackDb(m_profilePath, this);
    m_channelIdCounter = QDateTime::currentSecsSinceEpoch();
}

void ChannelHost::connect(quint16 port) {
    if (m_receiverSocket != nullptr) {
        return;
    }
    m_lookbackTimer->stop();
    m_port = port;
    m_receiverSocket = new QUdpSocket(this);
    if (m_receiverSocket->bind(QHostAddress::Any, m_port)) {
        setState(State_Connected);
        QObject::connect(m_receiverSocket, &QUdpSocket::readyRead, this, &ChannelHost::onDataReady);
    } else {
        setState(State_ConnectFailed);
    }
}

void ChannelHost::disconnect() {
    if (m_receiverSocket != nullptr) {
        m_receiverSocket->close();
        m_receiverSocket->deleteLater();
    }
    m_lookbackTimer->stop();
    setState(State_Disconect);
}

void ChannelHost::onDataReady() {
    QByteArray buffer;
    buffer.resize(m_receiverSocket->pendingDatagramSize());
    m_receiverSocket->readDatagram(buffer.data(), buffer.size());
    if (buffer.size() == sizeof(ChannelDataPackage)) {
        setState(State_Tracking);
        publishData(reinterpret_cast<const ChannelDataPackage*>(buffer.data()));
    } else {
        setState(State_ConnectFailed);
    }
    if (m_lastKeepTime.isNull()) {
        m_keepliveTimer->start(std::chrono::seconds(10));
    }
    m_lastKeepTime = QDateTime::currentDateTime();
}

void ChannelHost::publishData(const ChannelDataPackage *dataBuffer) {
    auto nowTime = QDateTime::currentDateTime();
    publishChannelData(dataBuffer, nowTime);
    publishTrackData(dataBuffer, nowTime);
}

void ChannelHost::publishChannelData(const ChannelDataPackage *dataBuffer, const QDateTime &nowTime) {
    QList<ChannelData> channelDataList;
    channelDataList.reserve(ChannelDataAmount);
    for (size_t i = 0; i < ChannelDataAmount; ++i) {
        ChannelData channelData;
        channelData.m_id = m_channelIdCounter++;
        channelData.m_time = nowTime.toMSecsSinceEpoch();
        for (size_t j = 0; j < EcgPortAmount; j++) {
            channelData.eda[j] = static_cast<ChannelData::DataType>(dataBuffer->ecg[j][i]);
        }
        for (size_t j = 0; j < ElectricalPortAmount; j++) {
            channelData.da[j] = static_cast<ChannelData::DataType>(dataBuffer->da[j][i]);
        }
        if (i > EcgPortAmount - 1) {
            auto midx = i - EcgPortAmount;
            channelData.m_pos = vtkVector3f(dataBuffer->m[midx].pos[0],dataBuffer->m[midx].pos[1],dataBuffer->m[midx].pos[2]);
        }
        channelDataList.push_back(channelData);
    }
    testChannelImportSpeed(channelDataList);

    if (channelDataList.size() > 1) {
        IOWorker::run([this, channelDataList] {
            m_channelDataDb->add(channelDataList);
        });
        emit sampleChanged(channelDataList);
    }
}
void ChannelHost::publishTrackData(const ChannelDataPackage *dataBuffer, const QDateTime &nowTime) {
    auto channelTrackData = std::make_unique<ChannelTrackData>(m_channelIdCounter++, nowTime.toMSecsSinceEpoch(), dataBuffer);
    emit trackData(*channelTrackData);

    IOWorker::run([this, channelTrackData = std::move(channelTrackData)] {
        m_channelTrackDb->add(*channelTrackData);
    });
}

void ChannelHost::onKeepliveTimerEvent() {
    qint64 secondsElapsed = m_lastKeepTime.secsTo(QDateTime::currentDateTime());
    if (secondsElapsed > 10) {
        setState(State_Disconect);
    }
}

void ChannelHost::setLookbackSpeed(qint32 newLookbackSpeed) {
    ChannelSimpleSource::setLookbackSpeed(newLookbackSpeed);
    if (newLookbackSpeed == 0) {
        m_lookbackTimer->stop();
    } else {
        m_lookbackTimer->start(std::chrono::milliseconds(newLookbackSpeed));
    }
}

void ChannelHost::testChannelImportSpeed(const QList<ChannelData> &channelDatas) {
    static quint64 startTime = 0;
    static quint64 channelDatasImportAmount = 0;
    if (startTime == 0) {
        startTime = QDateTime::currentMSecsSinceEpoch();
    }
    channelDatasImportAmount += channelDatas.size();
    if (QDateTime::currentMSecsSinceEpoch() - startTime >= 1000) {
        startTime = 0;
        channelDatasImportAmount = 0;
    }
}
void ChannelHost::onLookbackTimeout() {
    if (!m_lookbackChannelDatas.isEmpty()) {
        QList<ChannelData> channelDatas = m_lookbackChannelDatas.mid(index(), ChannelDataAmount);
        if (channelDatas.isEmpty()) {
            setIndex(0);
        } else {
            setIndex(index() + channelDatas.size());
        }
        emit sampleChanged(channelDatas);
    }

    if (!m_lookbackTrackDatas.isEmpty()) {
        if (m_lookbackTrackDataIndex >= m_lookbackTrackDatas.size()) {
            m_lookbackTrackDataIndex = 0;
        }
        emit trackData(m_lookbackTrackDatas[m_lookbackTrackDataIndex++]);
    }
    setState(State_Tracking);
}

void ChannelHost::lookback(qint64 beginTime, qint64 endTime) {
    IOWorker::run([this, beginTime, endTime] {
        auto channelTrackData = m_channelTrackDb->select(where(between(&ChannelTrackData::m_time, beginTime, endTime)));
        auto channelData = m_channelDataDb->select(where(between(&ChannelData::m_time, beginTime, endTime)));
        return std::make_pair(channelData, channelTrackData);
    }).then(this, [this](const QPair<QList<ChannelData>,QList<ChannelTrackData>> &data){
        m_lookbackTrackDatas = data.second;
        m_lookbackTrackDataIndex = 0;
        m_lookbackChannelDatas = data.first;
        setIndex(0);
        setSize(m_lookbackChannelDatas.size());
        setState(State_Connected);
    });
    disconnect();
}

void ChannelHost::ship(qint64 idx, qint64 count) {
    if (m_lookbackChannelDatas.isEmpty()) {
        return;
    }
    QList<ChannelData> datas = m_lookbackChannelDatas.mid(idx, count);
    emit shipChanged(datas);
}

void ChannelHost::reconnect(){
    m_lookbackTrackDatas.clear();
    m_lookbackTrackDataIndex = 0;
    m_lookbackChannelDatas.clear();
    setSize(m_lookbackChannelDatas.size());
    setIndex(0);
    disconnect();
    connect(m_port);
}
