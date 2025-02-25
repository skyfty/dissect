#ifndef DATASOURCE_H
#define DATASOURCE_H

#include "channel/ChannelData.h"
#include <QHash>
#include <QMap>
#include <QPointer>

class QAbstractSeries;
class Profile;
class QXYSeries;
class ChannelDataFilter;
class Channel;
class ElectrodeNode;
class OscillograpElectrode;
class BreathSurvey;

class OscillograpDataSource : public QObject
{
    Q_OBJECT;
    Q_PROPERTY(Profile* profile WRITE setProfile READ profile NOTIFY profileChanged);
    Q_PROPERTY(int max READ max WRITE setMax NOTIFY maxChanged FINAL)
    Q_PROPERTY(Channel *channel WRITE setChannel FINAL)
    Q_PROPERTY(bool paused READ paused WRITE setPaused NOTIFY pausedChanged FINAL)
    Q_PROPERTY(int currentIndex READ currentIndex NOTIFY currentIndexChanged FINAL)
    Q_PROPERTY(Mode mode READ mode WRITE setMode NOTIFY modeChanged FINAL)
    Q_PROPERTY(ChannelDataFilter *channelDataFilter WRITE setChannelDataFilter FINAL)
    Q_PROPERTY(BreathSurvey *breathSurvey READ breathSurvey WRITE setBreathSurvey NOTIFY breathSurveyChanged FINAL)

public:
    enum Mode {
        LOOP,
        FORWARD
    };
    Q_ENUM(Mode);

public:
    explicit OscillograpDataSource(QObject* parent = nullptr);
    ~OscillograpDataSource() override = default;

    Q_INVOKABLE void setSeries(const QString &id, const ElectrodeNode *node, QAbstractSeries *series);

    void setProfile(Profile* profile);
    Profile *profile() const;

    BreathSurvey *breathSurvey() const;
    void setBreathSurvey(BreathSurvey *newBreathSurvey);

    int max() const;
    void setMax(int newChartMax);

    void setChannelDataFilter(ChannelDataFilter *channelDataFilter);

    bool paused() const;
    void setPaused(bool newIsPaused);

    int currentIndex() const;

    Mode mode() const;
    void setMode(const Mode &newMode);

    void setChannel(Channel *newChannel);
    Channel* channel() const;

signals:
    void profileChanged();
    void maxChanged();
    void packAmountChanged();
    void pausedChanged();
    void currentIndexChanged(int index);
    void countChanged();
    void modeChanged();
    void channelChanged();
    void breathSurveyChanged();


public slots:
    void onChannelSampleChanged(const QList<ChannelData> &channelDatas);
    void onShipChanged(const QList<ChannelData>& channelDatas);
    void onElectrodeDeleted(ElectrodeNode* node);

private:
    void setLoopModeSeriesData(const QList<ChannelData> &channelDatas);
    int setLoopModeSeriesData(QXYSeries* series, const ElectrodeNode* electrodeNode,int currentIndex, const QList<ChannelData> &channelDatas);

    void setForwardModeSeriesData(const QList<ChannelData> &channelDatas);
    void setForwardModeSeriesData(QXYSeries* series, const ElectrodeNode* electrodeNode, const QList<ChannelData> &channelDatas);

    void setBrowseModeSeriesData(QXYSeries* series, const ElectrodeNode* electrodeNode, const QList<ChannelData> &channelDatas);

    ChannelData::List filterChannelData(const ElectrodeNode* electrodeNode, const QList<ChannelData> &channelDatas);
    ChannelData::List filterChannelData(ChannelDataFilter *filter, const ElectrodeNode* electrodeNode, const QList<ChannelData> &channelDatas);

    void resetSeriesData();
    void resetSeriesSize(QXYSeries *xyseries);
    void fillCacheData(QXYSeries* series, const ElectrodeNode* electrodeNode);
    void fillSeriesPoint(QXYSeries* series, QList<QPointF> &pointSeries);
    void cacheChannelData(const ElectrodeNode* electrodeNode,const ChannelData::List &dataList);
private:
    QPointer<Profile> m_profile;
    QPointer<Channel> m_channel;
    QPointer<OscillograpElectrode> m_oscillograpElectrode;
    QPointer<ChannelDataFilter> m_channelDataFilter;
    QPointer<BreathSurvey> m_breathSurvey;

    int m_max = 0;
    bool m_isPaused = true;
    Mode m_mode = LOOP;
    int m_index =0;
    quint16 m_channelsNumber = 8;
    QHash< const ElectrodeNode*, ChannelData::List> m_channelDataCache;
    QHash<QString, std::pair<QAbstractSeries*, const ElectrodeNode*>> m_seriesData;
};

#endif // DATASOURCE_H
