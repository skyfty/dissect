#ifndef CHANNELWORKER_H
#define CHANNELWORKER_H

#include <QObject>

#include <QFuture>
#include <QPointer>
#include <QPromise>
#include "rep_channel_merged.h"

class FrameRate;

class ChannelWorker : public QObject
{
    Q_OBJECT
public:
    explicit ChannelWorker(QObject *parent = nullptr);
    ~ChannelWorker();

    void exit();
    void init();
    ChannelReplica* replica() const { return m_channelReplica.data(); }
    void setTrackRate(quint64 lookbackSpeed);

signals:
    void connectHost(quint16 m_port);
    void disconnectHost();
    void setLookbackSpeed(qint32 newSpeed);
    void lookback(qint64 beginTime, qint64 endTime);
    void ship(qint64 idx, qint64 count);
    void reconnect();
    void setIndex(qint32 newSamplingRate);

    void portChanged();
    void stateChanged(ChannelReplica::State state);
    void sampleChanged(const QList<ChannelData> &datas);
    void trackData(const ChannelTrackData &datas);
    void shipChanged(const QList<ChannelData> &datas);

    void sizeChanged();
    void indexChanged();
    void lookbackSpeedChanged();

    void keepSaveChanged();
    void modeChanged();
    void trackRateChanged();

public slots:
    void onStarted();
    void onTrackData(const ChannelTrackData& datas);
    void onConnectHost(quint16 port);
    void onDisconnectHost();
    void onSetLookbackSpeed(qint32 newSpeed);
    void onLookback(qint64 beginTime, qint64 endTime);
    void onShip(qint64 idx, qint64 count);
    void onReconnect();
    void onSetIndex(qint32 newSamplingRate);
    void onStateChanged();

private:
    QPointer<QThread> m_thread;
    QPointer<FrameRate> m_frameRate;
    QScopedPointer<QRemoteObjectNode> m_remoteNode;
    QScopedPointer<ChannelReplica> m_channelReplica;
};

#endif // CHANNELWORKER_H
