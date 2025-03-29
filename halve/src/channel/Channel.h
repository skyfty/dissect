#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>
#include <vtkBoundingBox.h>
#include <vtkTransform.h>

#include <halitus/BreathSurvey.h>
#include "HalveType.h"
#include "rep_channel_merged.h"

class Profile;
class Channel;
class BreathSurvey;

class Channel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ChannelReplica::State state READ state NOTIFY stateChanged FINAL)
    Q_PROPERTY(quint16 port READ port WRITE setPort NOTIFY portChanged FINAL)
    Q_PROPERTY(qint32 index READ index  WRITE setIndex  NOTIFY indexChanged FINAL)
    Q_PROPERTY(qint32 size READ size  NOTIFY sizeChanged FINAL)
    Q_PROPERTY(qint32 lookbackSpeed READ lookbackSpeed WRITE setLookbackSpeed NOTIFY lookbackSpeedChanged FINAL)
    Q_PROPERTY(bool keepSave READ keepSave WRITE setKeepSave NOTIFY keepSaveChanged FINAL)
    Q_PROPERTY(Halve::ChannelMode mode READ mode WRITE setMode NOTIFY modeChanged FINAL)
    Q_PROPERTY(QString profilePath READ profilePath WRITE setProfilePath NOTIFY profilePathChanged FINAL)
    Q_PROPERTY(quint64 rate READ rate WRITE setRate NOTIFY rateChanged FINAL)


public:
    explicit Channel(QObject *parent = nullptr);
    ~Channel();
    quint16 port() const;
    void setPort(quint16 newPort);
    qint32 index() const;
    void setIndex(qint32 newSamplingRate);
    qint32 size() const;
    qint32 lookbackSpeed() const;
    void setLookbackSpeed(qint32 newSpeed);
    bool keepSave() const;
    void setKeepSave(bool newKeepSave);
    quint64 rate() const;
    void setRate(quint64 newRate);

    Halve::ChannelMode mode() const;
    void setMode(const Halve::ChannelMode &newMode);

    QString profilePath() const;
    void setProfilePath(const QString &newProfilePath);

    Q_INVOKABLE void launch();
    Q_INVOKABLE void connect();
    Q_INVOKABLE void reconnect();
    Q_INVOKABLE void disconnect();
    Q_INVOKABLE void lookback(qint64 beginTime, qint64 endTime);
    Q_INVOKABLE void ship(qint64 idx, qint64 count);
    Q_INVOKABLE void launchWatch();


    ChannelReplica::State state() const;

signals:
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
    void rateChanged();


    void profilePathChanged();

public slots:
    void onFinised();

private:
    bool m_keepSave = false;
    quint16 m_port;
    QScopedPointer<QRemoteObjectNode> m_remoteNode;
    QScopedPointer<ChannelReplica> m_channelReplica;
    QProcess *m_process = nullptr;
    Halve::ChannelMode m_mode = Halve::CHANNELMODE_MAGNETIC;
    QString m_profilePath;
    quint64 m_rate = 8;
};

#endif // CHANNEL_H
