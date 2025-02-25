#include "ProfileStorageWorker.h"

#include <QThread>

ProfileStorageWorker *profileStorageWorker = nullptr;

ProfileStorageWorker::ProfileStorageWorker(QObject *parent)
    : QObject{parent}
{
    m_thread = new QThread();
    moveToThread(m_thread);
    connect(m_thread, &QThread::finished, this, &QObject::deleteLater);
    profileStorageWorker = this;
}

ProfileStorageWorker::~ProfileStorageWorker() {
    m_thread->deleteLater();
    profileStorageWorker = nullptr;
}

void ProfileStorageWorker::init() {
    m_thread->start();
}

void ProfileStorageWorker::exit() {
    m_thread->quit();
    m_thread->wait();
}
