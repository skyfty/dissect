#include <QDateTime>
#include "channel/ChannelData.h"
#include "OscillograpDataSource.h"
#include "channel/ChannelDataFilter.h"
#include "channel/Channel.h"
#include "notebook/NotebookSnapshot.h"
#include "oscillograp/PantCalculate.h"
#include <QChart>
#include <QtCharts/QXYSeries>
#include <QtCharts/QAreaSeries>
#include <QtQuick/QQuickView>
#include <QtQuick/QQuickItem>
#include <QtCore/QDebug>
#include <QtCore/QRandomGenerator>
#include <QtCore/QtMath>
#include <QBrush>
#include <QFile>
#include <QGraphicsLayout>
#include <QPen>
#include <catheter/ElectrodeNode.h>
#include <channel/ChannelDataCalculate.h>
#include <channel/ChannelDataFilter.h>
#include <profile/Profile.h>
#include <profile/ProfileSnapshot.h>
#include "utility/ConnectCheck.h"

QT_USE_NAMESPACE

OscillograpDataSource::OscillograpDataSource(QObject* parent)
    : QObject(parent) {
}


void OscillograpDataSource::setProfile(Profile* profile) {
    if (m_profile == profile) {
        return;
    }
    m_profile = profile;
    m_oscillograpElectrode = m_profile->oscillograpElectrode();
    QObject::connect(m_oscillograpElectrode, &OscillograpElectrode::deleted, this, &OscillograpDataSource::onElectrodeDeleted);
    emit profileChanged();
}

Profile *OscillograpDataSource::profile() const {
    return m_profile;
}
void OscillograpDataSource::onElectrodeDeleted(ElectrodeNode* node) {
    Q_ASSERT(node != nullptr);
    m_channelDataCache.remove(node);
}

int OscillograpDataSource::max() const
{
    return m_max;
}

void OscillograpDataSource::setMax(int newChartMax) {
    if (m_max == newChartMax)
        return;
    m_max = newChartMax;
    for (auto i = m_seriesData.cbegin(), end = m_seriesData.cend(); i != end; ++i) {
        resetSeriesSize(static_cast<QXYSeries*>(i.value().first));
    }
    emit maxChanged();
}

void OscillograpDataSource::setChannelDataFilter(ChannelDataFilter *channelDataFilter) {
    if (m_channelDataFilter == channelDataFilter)
        return;
    m_channelDataFilter = channelDataFilter;
}

bool OscillograpDataSource::paused() const
{
    return m_isPaused;
}

void OscillograpDataSource::setPaused(bool newIsPaused)
{
    if (m_isPaused == newIsPaused)
        return;
    m_isPaused = newIsPaused;
    if (m_channel != nullptr) {
        if (m_isPaused) {
            QObject::disconnect(m_channel, &Channel::sampleChanged, this, &OscillograpDataSource::onChannelSampleChanged);
        } else {
            QObject::connect(m_channel, &Channel::sampleChanged, this, &OscillograpDataSource::onChannelSampleChanged);
        }
    }
    emit pausedChanged();
}

int OscillograpDataSource::currentIndex() const {
    return m_index;
}

void OscillograpDataSource::onChannelSampleChanged(const QList<ChannelData> &channelDatas) {
    if (channelDatas.empty()) {
        resetSeriesData();
        if (m_mode == LOOP) {
            m_index = 0;
            emit currentIndexChanged(m_index);
        }
    } else {
        switch(m_mode) {
        case LOOP: {
            setLoopModeSeriesData(channelDatas);
            break;
        }
        case FORWARD: {
            setForwardModeSeriesData(channelDatas);
            break;
        }
        }
    }
}
void OscillograpDataSource::onShipChanged(const QList<ChannelData>& channelDatas)
{
    if (channelDatas.empty()) {
        return;
    }
    for (auto seriesData = m_seriesData.cbegin(); seriesData != m_seriesData.cend(); ++seriesData) {
        QXYSeries* series = static_cast<QXYSeries*>(seriesData.value().first);
        setBrowseModeSeriesData(series, seriesData.value().second, channelDatas);
    }
}

void OscillograpDataSource::setLoopModeSeriesData(const QList<ChannelData> &channelDatas) {
    if (channelDatas.empty()) {
        return;
    }
    int loopModeCnt = 0;
    for (auto seriesData = m_seriesData.cbegin(); seriesData != m_seriesData.cend(); ++seriesData) {
        QXYSeries* series = static_cast<QXYSeries*>(seriesData.value().first);
        loopModeCnt += setLoopModeSeriesData(series, seriesData.value().second, m_index, channelDatas);
    }
    if (loopModeCnt == 0) {
        return;
    }
    int currentIndex = m_index + channelDatas.size();
    if (currentIndex >= m_max) {
        currentIndex -= m_max;
    }
    if (currentIndex != m_index) {
        m_index = currentIndex;
        emit currentIndexChanged(m_index);
    }
}

int OscillograpDataSource::setLoopModeSeriesData(QXYSeries *series, const ElectrodeNode *electrodeNode, int currentIndex, const QList<ChannelData> &channelDatas) {
    Q_ASSERT(series != nullptr && electrodeNode != nullptr);
    if (electrodeNode->seat() == ElectrodeNode::UNSET) {
        return 0;
    }
    ChannelData::List dataList = filterChannelData(electrodeNode, channelDatas);
    if (!dataList.isEmpty()) {
        cacheChannelData(electrodeNode, dataList);

        QList<QPointF> pointSeries = series->points();
        for(int channelDataIdx = 0; channelDataIdx < dataList.size(); ++channelDataIdx) {
            pointSeries[currentIndex].setY(dataList[channelDataIdx]);
            if (++currentIndex >= m_max) {
                currentIndex = 0;
            }
        }
        series->replace(pointSeries);
    }
    return dataList.size();
}

void OscillograpDataSource::setForwardModeSeriesData(const QList<ChannelData> &channelDatas) {
    if (channelDatas.empty()) {
        return;
    }
    for (auto seriesData = m_seriesData.cbegin(); seriesData != m_seriesData.cend(); ++seriesData) {
        QXYSeries* series = static_cast<QXYSeries*>(seriesData.value().first);
        setForwardModeSeriesData(series, seriesData.value().second, channelDatas);
    }
}

void OscillograpDataSource::setForwardModeSeriesData(QXYSeries *series, const ElectrodeNode *electrodeNode,const QList<ChannelData> &channelDatas) {
    Q_ASSERT(series != nullptr && electrodeNode != nullptr);
    if (electrodeNode->seat() == ElectrodeNode::UNSET) {
        return;
    }
    ChannelData::List dataList = filterChannelData(electrodeNode, channelDatas);
    if (!dataList.isEmpty()) {
        cacheChannelData(electrodeNode, dataList);

        QList<QPointF> pointSeries = series->points();
        for(int j = 0; j < dataList.size(); ++j) {
            pointSeries.append(QPointF(0, dataList[j]));
        }
        pointSeries.remove(0, channelDatas.size());
        fillSeriesPoint(series, pointSeries);
    }
}


void OscillograpDataSource::setBrowseModeSeriesData(QXYSeries* series, const ElectrodeNode* electrodeNode, const QList<ChannelData> &channelDatas) {
    Q_ASSERT(series != nullptr && electrodeNode != nullptr);
    if (electrodeNode->seat() == ElectrodeNode::UNSET) {
        return;
    }
    ChannelData::List dataList = filterChannelData(electrodeNode, channelDatas);
    if (!dataList.isEmpty()) {
        QList<QPointF> pointSeries = series->points();
        for(int j = 0; j < pointSeries.size(); ++j) {
            qint32 ydata = j >= dataList.size()?0:dataList[j];
            pointSeries[j] =QPointF(0, ydata);
        }
        fillSeriesPoint(series, pointSeries);
    }
}


void OscillograpDataSource::resetSeriesData() {
    for (auto seriesData = m_seriesData.cbegin(); seriesData != m_seriesData.cend(); ++seriesData) {
        QXYSeries* series = static_cast<QXYSeries*>(seriesData.value().first);
        QList<QPointF> pointSeries = series->points();
        for(int j = 0; j < pointSeries.size(); ++j) {
            pointSeries[j] = QPointF(0, 0);
        }
        fillSeriesPoint(series, pointSeries);
    }
}

ChannelData::List OscillograpDataSource::filterChannelData(const ElectrodeNode *electrodeNode, const QList<ChannelData> &channelDatas)
{
    return filterChannelData(m_channelDataFilter, electrodeNode, channelDatas);
}

ChannelData::List OscillograpDataSource::filterChannelData(ChannelDataFilter *filter, const ElectrodeNode* electrodeNode, const QList<ChannelData> &channelDatas) {
    Q_ASSERT(filter != nullptr);
    ChannelData::List dataList(channelDatas.size());
    if (electrodeNode->type() == Halve::PANT) {
        if(electrodeNode->id() == Pant1ID) {
            dataList.fill(m_breathSurvey->gurgitation());
        } else if(electrodeNode->id() == Pant2ID) {
            dataList.fill(m_breathSurvey->gating());
        }
        return dataList;
    } else {
        ChannelDataCalculate calc(electrodeNode);
        std::transform(channelDatas.begin(), channelDatas.end(), dataList.begin(), calc);
        return filter->pass(dataList, electrodeNode);
    }
}


void OscillograpDataSource::resetSeriesSize(QXYSeries *xyseries) {
    QList<QPointF> pointSeries = xyseries->points();
    if (pointSeries.size() > m_max) {
        m_index = 0;
    }
    pointSeries.resize(m_max);
    fillSeriesPoint(xyseries, pointSeries);
}

void OscillograpDataSource::setChannel(Channel *newChannel) {
    if (m_channel == newChannel) {
        return;
    }
    m_channel = newChannel;

    if (m_profile->type() == Halve::PROFILE_SNAPSHOT) {
        QObject::connect(m_channel, &Channel::shipChanged, this, &OscillograpDataSource::onShipChanged);
    }
    emit channelChanged();
}

Channel* OscillograpDataSource::channel() const {
    return m_channel;
}

BreathSurvey *OscillograpDataSource::breathSurvey() const
{
    return m_breathSurvey;
}

void OscillograpDataSource::setBreathSurvey(BreathSurvey *newBreathSurvey)
{
    if (m_breathSurvey == newBreathSurvey)
        return;
    m_breathSurvey = newBreathSurvey;
    emit breathSurveyChanged();
}

OscillograpDataSource::Mode OscillograpDataSource::mode() const
{
    return m_mode;
}

void OscillograpDataSource::setMode(const OscillograpDataSource::Mode &newMode)
{
    if (m_mode == newMode)
        return;
    m_mode = newMode;
    m_index = m_max-1;
    emit modeChanged();
}

void OscillograpDataSource::setSeries(const QString &id, const ElectrodeNode *node, QAbstractSeries *series) {
    if (series == nullptr) {
        m_seriesData.remove(id);
    } else {
        QXYSeries *xyseries = static_cast<QXYSeries*>(series);
        xyseries->chart()->layout()->setContentsMargins(0, 0, 0, 0);
        m_seriesData[id] = std::make_pair(xyseries, node);
        fillCacheData(xyseries, node);
    }
}

void OscillograpDataSource::fillCacheData(QXYSeries *xyseries, const ElectrodeNode *electrodeNode) {
    QList<QPointF> pointSeries = xyseries->points();
    pointSeries.resize(m_max);
    if (m_channelDataCache.contains(electrodeNode)) {
        ChannelData::List dataList = m_channelDataCache[electrodeNode];
        if (m_mode == LOOP)  {
            int idx = m_index;
            auto iter = dataList.rbegin();
            for(;idx >=0 && iter != dataList.rend();idx--, ++iter) {
                pointSeries[idx].setY(*iter);
            }
            idx = pointSeries.size() - 1;
            for(;iter != dataList.rend() && idx >m_index;idx--,  ++iter) {
                pointSeries[idx].setY(*iter);
            }
        } else {
            for(int j = 0; j < dataList.size(); ++j) {
                pointSeries[0].setY(dataList[j]);
            }
        }
    }
    fillSeriesPoint(xyseries, pointSeries);
}

void OscillograpDataSource::fillSeriesPoint(QXYSeries *series, QList<QPointF> &pointSeries)
{
    for (int j = 0; j < pointSeries.size(); ++j) {
        pointSeries[j].setX(j);
    }
    series->replace(pointSeries);
}

void OscillograpDataSource::cacheChannelData(const ElectrodeNode *electrodeNode, const ChannelData::List &dataList) {
    ChannelData::List &electrodeChannelData = m_channelDataCache[electrodeNode];
    electrodeChannelData.append(dataList);

    int spmax = electrodeChannelData.size() - m_max;
    if (spmax > 0) {
        electrodeChannelData.remove(0, spmax);
    }
}
