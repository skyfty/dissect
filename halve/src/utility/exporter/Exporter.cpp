#include "Exporter.h"

Exporter::Exporter(QObject *parent)
    : QObject{parent}
{}

Exporter::~Exporter() {
    m_process = nullptr;
}
void Exporter::cancel() {

}

void Exporter::launch() {
    QProcess *process = m_process;
    m_process = nullptr;
    if (process != nullptr) {
        process->terminate();
        delete process;
    }
    m_remoteNode.reset();
    m_exporterReplica.reset();
    m_host = QString("local:exporter_%1").arg((quint64)this);
    QStringList arguments;
    arguments << "--pid" << QString("%1").arg(QCoreApplication::applicationPid())
              << "--host" << m_host
              << "--process-type" << "exporter";
    m_process = new QProcess(this);;
    QObject::connect(m_process, &QProcess::finished, this, &Exporter::onFinised);
    QObject::connect(m_process, &QProcess::started, this, &Exporter::onStarted);
    m_process->start(QCoreApplication::applicationFilePath(), arguments);
    m_process->waitForStarted();
}

void Exporter::onFinised() {
    m_process = nullptr;
}

void Exporter::onStarted() {
    m_remoteNode.reset(new QRemoteObjectNode()); // create remote object node
    m_remoteNode->connectToNode(QUrl(m_host)); // connect with remote host node
    m_remoteNode->setHeartbeatInterval(1000);
    m_exporterReplica.reset(m_remoteNode->acquire<ExporterReplica>()); // acquire replica of source from host node
    QObject::connect(m_exporterReplica.data(), &ExporterReplica::finished, this, &Exporter::onCopyFinised);
    QObject::connect(m_exporterReplica.data(), &ExporterReplica::stateChanged, this, &Exporter::onStateChanged);
}

void Exporter::copy(const QString &srcPath, const QString &destPath)
{
    if (m_process == nullptr) {
        launch();
    }
    m_list.append(std::make_pair(srcPath, destPath));

    if (m_exporterReplica!=nullptr && m_exporterReplica->state() == ExporterReplica::State_Ready) {
        m_exporterReplica->copy(srcPath, destPath);
    }
}

void Exporter::onStateChanged() {
    if (m_exporterReplica->state() == ExporterReplica::State_Ready) {
        for(const auto &item:m_list) {
            m_exporterReplica->copy(item.first, item.second);
        }
    }
}

void Exporter::onCopyFinised(const QString &srcPath, const QString &destPath) {
    m_list.removeOne(std::make_pair(srcPath, destPath));
    emit finished(srcPath, destPath);
}

