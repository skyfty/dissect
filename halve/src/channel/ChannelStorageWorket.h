#ifndef CHANNELWORKET_H
#define CHANNELWORKET_H

#include "channel/ChannelData.h"
#include <QObject>
#include <QPointer>

class QThread;
class ChannelDataDb;

class ChannelStorageWorket : public QObject
{
    Q_OBJECT
public:
    explicit ChannelStorageWorket(QObject *parent = nullptr);
    ~ChannelStorageWorket();
    void init(ChannelDataDb *channelDataDb);
    void exit();

public slots:
    void onSave(const QList<ChannelData> &datas);
signals:
    void save(const QList<ChannelData> &datas);

private:
    QPointer<QThread> m_thread;
    QPointer<ChannelDataDb> m_channelDataDb;
};

#endif // CHANNELWORKET_H
