#include "IOWorker.h"

#include <QRunnable>
#include <QThreadPool>

IOWorker *ioWorker = nullptr;
extern Qt::HANDLE IoThreadHandle;

IOWorker::IOWorker(QObject *parent)
    : QObject{parent}
{
    ioWorker = this;
    m_thread = new QThread();
    moveToThread(m_thread);
    connect(m_thread, &QThread::finished, this, &QObject::deleteLater);
    connect(m_thread, &QThread::started, this, [] {
        IoThreadHandle = QThread::currentThreadId();
    });
    QObject::connect(this, &IOWorker::start, this, [](QRunnable* runner) {
        runner->run();
    });
}

IOWorker::~IOWorker() {
    m_thread->deleteLater();
    ioWorker = nullptr;
}

void IOWorker::init() {
    m_thread->start();
    m_thread->wait(200);
}

void IOWorker::exit() {
    m_thread->quit();
    m_thread->wait();
}
