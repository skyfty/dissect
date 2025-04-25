#include "ChannelWorker.h"
#include "utility/FrameRate.h"

ChannelWorker::ChannelWorker(QObject* parent)
    : QObject{ parent } {
    m_thread = new QThread();
    moveToThread(m_thread);
    connect(m_thread, &QThread::finished, this, &QObject::deleteLater);
    connect(m_thread, &QThread::started, this, &ChannelWorker::onStarted);
    connect(this, &ChannelWorker::connectHost, this, &ChannelWorker::onConnectHost);
    connect(this, &ChannelWorker::disconnectHost, this, &ChannelWorker::onDisconnectHost);
    connect(this, &ChannelWorker::setLookbackSpeed, this, &ChannelWorker::onSetLookbackSpeed);
    connect(this, &ChannelWorker::lookback, this, &ChannelWorker::onLookback);
    connect(this, &ChannelWorker::ship, this, &ChannelWorker::onShip);
    connect(this, &ChannelWorker::reconnect, this, &ChannelWorker::onReconnect);

    m_frameRate = new FrameRate(this);
}

ChannelWorker::~ChannelWorker() {
    m_thread->deleteLater();
}
void ChannelWorker::onStarted() {
    m_remoteNode.reset(new QRemoteObjectNode()); // create remote object node
    m_remoteNode->connectToNode(QUrl(QStringLiteral("local:channel"))); // connect with remote host node
    m_remoteNode->setHeartbeatInterval(1000);
    m_channelReplica.reset(m_remoteNode->acquire<ChannelReplica>()); // acquire replica of source from host node

    QObject::connect(m_channelReplica.data(), &ChannelReplica::sampleChanged, this, &ChannelWorker::sampleChanged);
    QObject::connect(m_channelReplica.data(), &ChannelReplica::trackData, this, &ChannelWorker::onTrackData);
    QObject::connect(m_channelReplica.data(), &ChannelReplica::shipChanged, this, &ChannelWorker::shipChanged);
    QObject::connect(m_channelReplica.data(), &ChannelReplica::stateChanged, this, &ChannelWorker::onStateChanged);
    QObject::connect(m_channelReplica.data(), &ChannelReplica::indexChanged, this, &ChannelWorker::indexChanged);
    QObject::connect(m_channelReplica.data(), &ChannelReplica::sizeChanged, this, &ChannelWorker::sizeChanged);
}

void ChannelWorker::exit() {
    m_thread->quit();
    m_thread->wait();
}

void ChannelWorker::init() {
    m_thread->start();
}
void ChannelWorker::onTrackData(const ChannelTrackData& datas) {
    if (m_frameRate->charge()) {
        return;
    }
    emit trackData(datas);
}

void ChannelWorker::setTrackRate(quint64 newRate) {
    m_frameRate->setRate(newRate);
}
void ChannelWorker::onConnectHost(quint16 m_port) {
    Q_ASSERT(m_channelReplica != nullptr);
    m_channelReplica->connect(m_port);
}
void ChannelWorker::onDisconnectHost() {
    Q_ASSERT(m_channelReplica != nullptr);
    m_channelReplica->disconnect();
}

void  ChannelWorker::onSetLookbackSpeed(qint32 newSpeed) {
    Q_ASSERT(m_channelReplica != nullptr);
    m_channelReplica->setLookbackSpeed(newSpeed);
}
void ChannelWorker::onLookback(qint64 beginTime, qint64 endTime) {
    Q_ASSERT(m_channelReplica != nullptr);
    m_channelReplica->lookback(beginTime, endTime);
}
void ChannelWorker::onShip(qint64 idx, qint64 count) {
    Q_ASSERT(m_channelReplica != nullptr);
    m_channelReplica->ship(idx, count);
}    

void ChannelWorker::onReconnect() {
    Q_ASSERT(m_channelReplica != nullptr);
    m_channelReplica->reconnect();
}
void ChannelWorker::onSetIndex(qint32 newSamplingRate) {

    Q_ASSERT(m_channelReplica != nullptr);
    m_channelReplica->setIndex(newSamplingRate);
}
void ChannelWorker::onStateChanged() {
    Q_ASSERT(m_channelReplica != nullptr);
    emit stateChanged(m_channelReplica->state());
}
