#include "Channel.h"
#include "catheter/CatheterDb.h"
#include "profile/Profile.h"

#include <vtkLineSource.h>
#include <vtkMath.h>
#include <vtkQuaternion.h>
#include <vtkTransformPolyDataFilter.h>
#include "catheter/CatheterMagnetism.h"
#include <catheter/Catheter.h>

Channel::Channel(QObject *parent)
    : QObject{parent}
{
    m_remoteNode.reset(new QRemoteObjectNode()); // create remote object node
    m_remoteNode->connectToNode(QUrl(QStringLiteral("local:channel"))); // connect with remote host node
    m_remoteNode->setHeartbeatInterval(1000);
    m_channelReplica.reset(m_remoteNode->acquire<ChannelReplica>()); // acquire replica of source from host node
    QObject::connect(m_channelReplica.data(), &ChannelReplica::sampleChanged, this, &Channel::sampleChanged);
    QObject::connect(m_channelReplica.data(), &ChannelReplica::trackData, this, &Channel::trackData);
    QObject::connect(m_channelReplica.data(), &ChannelReplica::shipChanged, this, &Channel::shipChanged);
    QObject::connect(m_channelReplica.data(), &ChannelReplica::stateChanged, this, &Channel::stateChanged);
    QObject::connect(m_channelReplica.data(), &ChannelReplica::indexChanged, this, &Channel::indexChanged);
    QObject::connect(m_channelReplica.data(), &ChannelReplica::sizeChanged, this, &Channel::sizeChanged);
}

Channel::~Channel() {
    m_process = nullptr;
}

void Channel::launch() {
    QProcess *process = m_process;
    m_process = nullptr;
    if (process != nullptr) {
        process->terminate();
        delete process;
    }
    QStringList arguments;
    arguments << "--pid" << QString("%1").arg(QCoreApplication::applicationPid())
              << "--process-type" << "channel"
              << "--keep-save" << (m_keepSave ? "1" : "0")
              << "--channel-mode" << QString("%1").arg(m_mode)
              << "--profile" << m_profilePath;
    m_process = new QProcess(this);;
    QObject::connect(m_process, &QProcess::finished, this, &Channel::onFinised);
    m_process->start(QCoreApplication::applicationFilePath(), arguments);
}

void Channel::onFinised() {
    if (m_process != nullptr) {
        launch();
    }
}

QString Channel::profilePath() const
{
    return m_profilePath;
}

void Channel::setProfilePath(const QString &newProfilePath)
{
    if (m_profilePath == newProfilePath)
        return;
    m_profilePath = newProfilePath;
    emit profilePathChanged();
}


Halve::ChannelMode Channel::mode() const
{
    return m_mode;
}

void Channel::setMode(const Halve::ChannelMode &newMode)
{
    if (m_mode == newMode)
        return;
    m_mode = newMode;
    emit modeChanged();
}

bool Channel::keepSave() const
{
    return m_keepSave;
}

void Channel::setKeepSave(bool newKeepSave)
{
    if (m_keepSave == newKeepSave)
        return;
    m_keepSave = newKeepSave;
    m_channelReplica->setKeepSave(newKeepSave);
    emit keepSaveChanged();
}

void Channel::connect() {
    m_channelReplica->connect(m_port);
}

void Channel::disconnect()
{
    m_channelReplica->disconnect();
}


qint32 Channel::lookbackSpeed() const
{
    return m_channelReplica->lookbackSpeed();
}

void Channel::setLookbackSpeed(qint32 newSpeed)
{
   return m_channelReplica->setLookbackSpeed(newSpeed);
}

qint32 Channel::index() const {
    return m_channelReplica->index();
}
void Channel::setIndex(qint32 newSamplingRate) {
    m_channelReplica->setIndex(newSamplingRate);
}

qint32 Channel::size() const {
    return m_channelReplica->size();
}

ChannelReplica::State Channel::state() const {
    return m_channelReplica == nullptr?ChannelReplica::State_Shutdown:m_channelReplica->state();
}

quint16 Channel::port() const {
    return m_port;
}

void Channel::setPort(quint16 newPort) {
    if (m_port == newPort)
        return;
    m_port = newPort;
    emit portChanged();
}

void Channel::lookback(qint64 beginTime, qint64 endTime) {
    m_channelReplica->lookback(beginTime, endTime);
}

void Channel::reconnect() {
    m_channelReplica->reconnect();
}
void Channel::ship(qint64 idx, qint64 count) {
    m_channelReplica->ship(idx, count);
}

void Channel::launchWatch() {
    QStringList arguments;
    arguments << "--pid" << QString("%1").arg(QCoreApplication::applicationPid())
              << "--process-type" << "channel-watch";
    auto process = new QProcess(this);;
    process->start(QCoreApplication::applicationFilePath(), arguments);
}
