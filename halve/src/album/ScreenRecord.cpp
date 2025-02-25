#include "ScreenRecord.h"
#include <Windows.h>
#include <QProcess>
#include <QDate>
#include <QCoreApplication>
#include <QStandardPaths>
#include <QTimer>

ScreenRecord::ScreenRecord(QObject *parent)
    : QObject{parent}
{}

ScreenRecord::~ScreenRecord() {
    if (m_process != nullptr) {
        terminalProcess();
    }
}
void ScreenRecord::terminalProcess() {
    m_process->kill();
    m_process->waitForFinished();
    CloseHandle(m_event);
    m_event = nullptr;
}

void ScreenRecord::stop()
{
    if (m_process != nullptr) {
        SetEvent(m_event);
        m_process->waitForFinished();
    }
}
void ScreenRecord::start(const QString &name, std::chrono::milliseconds value) {
    start(name);
    QTimer::singleShot(value, this, [&]{
        stop();
    });
}

void ScreenRecord::start(const QString &name) {
    if (m_process!= nullptr) {
        terminalProcess();
        delete m_process;
    }
    m_event = CreateEventA(nullptr, true, false, name.toStdString().c_str());
    QString fileName = QString("%1.mp4").arg(QDateTime::currentMSecsSinceEpoch());
    QString savePath = m_savePath;
    if (savePath.isEmpty()) {
        savePath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
        savePath += QString("\\%1").arg(QDateTime::currentSecsSinceEpoch());
    }
    savePath +=  "\\" + fileName;

    m_process = new QProcess(this);
    QStringList arguments;
    arguments << "--pid" << QString("%1").arg(QCoreApplication::applicationPid())
              << "--process-type" << "screengrab"
              << "--out-file" << savePath << "--wait-event" << name;
    QObject::connect(m_process, &QProcess::finished, this, [this, savePath, fileName](){
        onScreenRecordFinised(savePath, fileName);
    });
    m_process->start(QCoreApplication::applicationFilePath(), arguments);
    m_process->waitForStarted();
}

void ScreenRecord::onScreenRecordFinised(const QString &savePath, const QString &filename) {
    if (m_process != nullptr) {
        CloseHandle(m_event);
        m_event = nullptr;
        m_process->deleteLater();
    }
    emit finished(savePath, filename);
}

QString ScreenRecord::savePath() const
{
    return m_savePath;
}

void ScreenRecord::setSavePath(const QString &newSavePath)
{
    if (m_savePath == newSavePath)
        return;
    m_savePath = newSavePath;
    emit savePathChanged();
}
