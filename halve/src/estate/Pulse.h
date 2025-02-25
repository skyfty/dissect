#ifndef PULSE_H
#define PULSE_H


#include <QFuture>
#include <QObject>
#include <QPointer>
#include <channel/ChannelData.h>

namespace ys {
class HeartRateFilter;
}

class Profile;
class Channel;
class ElectrodeNode;
class ChannelDataFilter;
class CatheterDb;
class FilterOptions;

class Pulse : public QObject {
  Q_OBJECT
    Q_PROPERTY(Profile* profile WRITE setProfile READ profile NOTIFY profileChanged);
  Q_PROPERTY(quint16 samplingRate READ samplingRate WRITE setSamplingRate NOTIFY samplingRateChanged FINAL)
  Q_PROPERTY(Channel *channel WRITE setChannel FINAL)

public:
    explicit Pulse(QObject *parent = nullptr);
    ~Pulse();
    Q_INVOKABLE void update();

    void setProfile(Profile *newProfile);
    Profile *profile() const;

    quint16 samplingRate() const;
    void setSamplingRate(quint16 newSamplingRate);

    void setChannel(Channel *newChannel);
signals:
    void profileChanged();
    void valueChanged();

    void samplingRateChanged();
public slots:
    void onChannelSampleChanged(const QList<ChannelData> &channelDatas);
    void onPulseElectrodeIdChanged();

private:
    void createDefaultFilter();
    void processChannelData(const QList<ChannelData> &channelDatas);

private:
    QPointer<Profile> m_profile;
    quint16 m_samplingRate = 2048;
    QPointer<Channel> m_channel;
    QList<ChannelData> m_channelDataCache;
    QPointer<CatheterDb> m_catheterDb;
    QPointer<ChannelDataFilter> m_channelDataFilter;
    QPointer<FilterOptions> m_filterOptions;
    QPointer<ElectrodeNode> m_electrodeNode;
    std::unique_ptr<ys::HeartRateFilter> m_heartRateFilter;
    QFuture<void> m_loadDataFuture;
};

#endif // PULSE_H
