#ifndef CHANNELHOST_H
#define CHANNELHOST_H

#include <QObject>
#include <QUdpSocket>
#include "HalveType.h"
#include "rep_channel_merged.h"

struct ChannelTrack;
class ChannelTrackDb;
class CatheterMagnetism;
class ChannelTrackData;

class ChannelHost : public ChannelSimpleSource {
  Q_OBJECT
public:
    explicit ChannelHost(QObject *parent = nullptr);
    ~ChannelHost();
    void init(const QString& profile, Halve::ChannelMode mode);
    void connect(quint16 host) override;
    void reconnect() override;
    void disconnect() override;
    void lookback(qint64 beginTime, qint64 endTime) override;
    void ship(qint64 idx, qint64 count) override;
    void setLookbackSpeed(qint32 lookbackSpeed) override;

private:
    void publishData(const ChannelDataPackage *buffer);
    void testChannelImportSpeed(const QList<ChannelData> &channelDatas);
    void publishTrackData(const ChannelDataPackage *dataBuffer, const QDateTime &nowTime);
    void publishChannelData(const ChannelDataPackage *dataBuffer, const QDateTime &nowTime);

public slots:
    void onDataReady();
    void onLookbackTimeout();
    void onKeepliveTimerEvent();

private:
    QString m_profilePath;
    quint16 m_port = 0;
    quint64 m_channelIdCounter = 0;
    QPointer<ChannelDataDb> m_channelDataDb;
    QPointer<ChannelTrackDb> m_channelTrackDb;
    QPointer<QUdpSocket> m_receiverSocket;
    QHash<quint8, QByteArray> m_receiveData;
    QPointer<QTimer> m_lookbackTimer = nullptr;
    QPointer<QTimer> m_keepliveTimer = nullptr;
    QList<ChannelData>  m_lookbackChannelDatas;
    QList<ChannelTrackData> m_lookbackTrackDatas;
    quint32 m_lookbackTrackDataIndex = 0;
    Halve::ChannelMode m_mode;
    QDateTime m_lastKeepTime;
};


#endif // CHANNELHOST_H
