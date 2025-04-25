#include "Channel.h"
#include "catheter/CatheterDb.h"
#include "profile/Profile.h"

#include <vtkLineSource.h>
#include <vtkMath.h>
#include <vtkQuaternion.h>
#include <vtkTransformPolyDataFilter.h>
#include "catheter/CatheterMagnetism.h"
#include <catheter/Catheter.h>
#include "ChannelWorker.h"

Channel::Channel(QObject *parent)
    : QObject{parent}
{
    m_worker = new ChannelWorker();
    QObject::connect(m_worker, &ChannelWorker::sampleChanged, this, &Channel::sampleChanged);
    QObject::connect(m_worker, &ChannelWorker::trackData, this, &Channel::trackData);
    QObject::connect(m_worker, &ChannelWorker::shipChanged, this, &Channel::shipChanged);
    QObject::connect(m_worker, &ChannelWorker::stateChanged, this, &Channel::onStateChanged);
    QObject::connect(m_worker, &ChannelWorker::indexChanged, this, &Channel::indexChanged);
    QObject::connect(m_worker, &ChannelWorker::sizeChanged, this, &Channel::sizeChanged);
    m_worker->init();
}

Channel::~Channel() {
    m_worker->exit();
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

quint64 Channel::trackRate() const
{
    return m_trackRate;
}

void Channel::setTrackRate(quint64 rate)
{
    if (m_trackRate == rate)
        return;
    m_trackRate = rate;
    m_worker->setTrackRate(rate);
    emit trackRateChanged();
}

void Channel::connect() {
    if (m_worker->replica() == nullptr) {
        return;
    }
    emit m_worker->connectHost(m_port);
}

void Channel::disconnect()
{
    if (m_worker->replica() == nullptr) {
        return;
    }
    emit m_worker->disconnectHost();
}


qint32 Channel::lookbackSpeed() const
{
    return m_lookbackSpeed;
}

void Channel::setLookbackSpeed(qint32 newSpeed)
{
    if (m_lookbackSpeed == newSpeed) {
        return;
    }
    m_lookbackSpeed = newSpeed;
    emit  m_worker->setLookbackSpeed(m_lookbackSpeed);
    emit lookbackSpeedChanged();
}

qint32 Channel::index() const {
    return m_index;
}

void Channel::setIndex(qint32 newIndex) {
    if (m_index == newIndex) {
        return;
    }
    m_index = newIndex;
    emit m_worker->setIndex(m_index);
    emit indexChanged();
}

qint32 Channel::size() const {
    return m_size;
}

ChannelReplica::State Channel::state() const {
    return m_state;
}

void Channel::onStateChanged(ChannelReplica::State s) {
    m_state = s;
    emit stateChanged(s);
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
    if (m_worker->replica() == nullptr) {
        return;
    }
    emit m_worker->lookback(beginTime, endTime);
}

void Channel::reconnect() {
    if (m_worker->replica() == nullptr) {
        return;
    }
    emit m_worker->reconnect();
}
void Channel::ship(qint64 idx, qint64 count) {
    if (m_worker->replica() == nullptr) {
        return;
    }
    emit m_worker->ship(idx, count);
}

void Channel::launchWatch() {
    QStringList arguments;
    arguments << "--pid" << QString("%1").arg(QCoreApplication::applicationPid())
              << "--process-type" << "channel-watch";
    auto process = new QProcess(this);;
    process->start(QCoreApplication::applicationFilePath(), arguments);
}
