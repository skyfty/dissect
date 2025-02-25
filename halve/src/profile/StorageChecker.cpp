#include "StorageChecker.h"

#include "Profile.h"

#include <QCoreApplication>
#include <QProcess>
#include <QRemoteObjectNode>

StorageChecker::StorageChecker(QObject *parent)
    : QObject{parent}
{
    m_remoteNode.reset(new QRemoteObjectNode()); // create remote object node
    m_remoteNode->connectToNode(QUrl(QStringLiteral("local:storagechecker"))); // connect with remote host node
    m_remoteNode->setHeartbeatInterval(1000);
    m_storageCheckerReplica.reset(m_remoteNode->acquire<StorageCheckerReplica>()); // acquire replica of source from host node
    QObject::connect(m_storageCheckerReplica.data(), &StorageCheckerReplica::alarm, this, &StorageChecker::alarm);
    QObject::connect(m_storageCheckerReplica.data(), &StorageCheckerReplica::stateChanged, this, &StorageChecker::stateChanged);
}

StorageChecker::~StorageChecker()
{
    m_process = nullptr;
}

void StorageChecker::launch() {
    QProcess *process = m_process;
    m_process = nullptr;
    if (process != nullptr) {
        process->terminate();
        delete process;
    }
    QStringList arguments;
    arguments << "--pid" << QString("%1").arg(QCoreApplication::applicationPid())
              << "--process-type" << "storagechecker"
              << "--threshold" << QString::number(m_threshold)
              << "--profile" << m_profile->path();
    m_process = new QProcess(this);;
    QObject::connect(m_process, &QProcess::finished, this, &StorageChecker::onFinised);
    m_process->start(QCoreApplication::applicationFilePath(), arguments);
}

void StorageChecker::onFinised() {
    if (m_process != nullptr) {
        launch();
    }
}

Halve::StorageState StorageChecker::state() const
{
    return m_storageCheckerReplica->state();
}


void StorageChecker::setProfile(Profile* profile) {
    if (m_profile == profile) {
        return;
    }
    m_profile = profile;
    emit profileChanged();
}

Profile *StorageChecker::profile() const {
    return m_profile;
}
qint32 StorageChecker::threshold() const
{
    return m_threshold;
}

void StorageChecker::setThreshold(qint32 newThreshold)
{
    if (m_threshold == newThreshold)
        return;
    m_threshold = newThreshold;
    if (m_storageCheckerReplica != nullptr) {
        m_storageCheckerReplica->setThreshold(m_threshold);
    }
    emit thresholdChanged();
}
