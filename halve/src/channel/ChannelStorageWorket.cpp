#include "ChannelDataDb.h"
#include "ChannelStorageWorket.h"

#include <QThread>

ChannelStorageWorket::ChannelStorageWorket(QObject *parent)
    : QObject{parent}
{
    m_thread = new QThread();
    moveToThread(m_thread);
    connect(m_thread, &QThread::finished, this, &QObject::deleteLater);
    QObject::connect(this, &ChannelStorageWorket::save, this, &ChannelStorageWorket::onSave);
}

ChannelStorageWorket::~ChannelStorageWorket() {
    m_thread->deleteLater();
}

void ChannelStorageWorket::onSave(const QList<ChannelData> &datas) {
    Q_ASSERT(m_channelDataDb != nullptr);
    m_channelDataDb->add(datas);
}

void ChannelStorageWorket::init(ChannelDataDb *channelDataDb) {
    Q_ASSERT(channelDataDb != nullptr);
    m_channelDataDb = channelDataDb;
    m_thread->start();
}

void ChannelStorageWorket::exit() {
    m_thread->quit();
    m_thread->wait();
}
