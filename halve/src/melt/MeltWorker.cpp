#include <QDateTime>
#include <QThread>
#include "catheter/CatheterDb.h"
#include "channel/Channel.h"
#include "channel/ChannelDataCalculate.h"
#include "channel/ChannelDataFilter.h"
#include <catheter/ElectrodeNode.h>
#include "melt/AppointElectrode.h"
#include "melt/MeltSamplingElectrode.h"
#include "melt/MeltOptions.h"
#include "melt/MeltSetting.h"
#include "melt/MeltSurvey.h"
#include "MeltWorker.h"

MeltWorker::MeltWorker(QObject* parent)
    : QObject{ parent } {
    m_thread = new QThread();
    moveToThread(m_thread);
    connect(m_thread, &QThread::finished, this, &QObject::deleteLater);
    QObject::connect(this, &MeltWorker::update, this, &MeltWorker::onUpdating);
}

MeltWorker::~MeltWorker() {
    m_thread->deleteLater();
}

void MeltWorker::exit() {
    m_thread->quit();
    m_thread->wait();
}

void MeltWorker::init(CatheterDb *catheterDb) {
    m_catheterDb = catheterDb;
    m_thread->start();
}
void MeltWorker::onUpdating(const QList<ChannelData>& channelDatas, qint32 consultDataIndex, MeltOptions *meltOptions,  MeltSetting *meltSetting)
{
    ElectrodeNodeHash electrodeDatas = filterChannelData(channelDatas, meltOptions, meltSetting);
    ElectrodeNode *electrode = m_catheterDb->getElectrodeNode(meltSetting->getConsultElectrodeId());
    if (electrode != nullptr && electrodeDatas.contains(electrode)) {
      calculateMeltData(channelDatas,consultDataIndex, electrodeDatas, meltOptions, meltSetting);
    }
    emit updateFinished(electrodeDatas,meltOptions, meltSetting);
}

void MeltWorker::calculateMeltData(const QList<ChannelData> &channelDatas, qint32 consultDataIndex, const ElectrodeNodeHash &electrodeDatas, MeltOptions *meltOptions,  MeltSetting *meltSetting) {
    MeltSamplingElectrode *meltSamplingElectrode = meltOptions->meltSamplingElectrode();
    ElectrodeNode *consultElectrode = m_catheterDb->getElectrodeNode(meltSetting->getConsultElectrodeId());
    auto consultDatas = electrodeDatas[consultElectrode];
    if (consultDatas.isEmpty()) {
      return;
    }
    MeltSurvey consultSurvey(consultDatas);
    auto consultDataIndexList = consultSurvey(meltSetting->consultPerception());
    if (consultDataIndex == -1) {
        consultDataIndex = MeltSurvey::germValue(consultDataIndexList, MeltSurvey::MIDDLE);
    }
    if (consultDataIndex >= 0 && consultDataIndex < channelDatas.size()) {
        emit consultDataFinished(consultDatas, consultDataIndex, consultDataIndexList,meltOptions, meltSetting);
        calculateAppointData(electrodeDatas, consultDataIndex, meltOptions, meltSetting);
        calculateSamplingData(electrodeDatas, consultDataIndex, meltOptions, meltSetting);
    }
}

void MeltWorker::calculateAppointData(const ElectrodeNodeHash &electrodeDatas, qint32 consultDataIndex, MeltOptions *meltOptions,  MeltSetting *meltSetting) {
    AppointElectrode *appointElectrode = meltOptions->appointElectrode();
    for(const ElectrodeNode* electrodeNode:appointElectrode->getDatas()) {
        ChannelData::List appointDatas = electrodeDatas[electrodeNode];
        if (appointDatas.empty()) {
            continue;
        }
        emit appointDataFinished(electrodeNode, appointDatas, consultDataIndex,meltOptions, meltSetting);
    }
}

void MeltWorker::calculateSamplingData(const ElectrodeNodeHash &electrodeDatas, qint32 consultDataIndex, MeltOptions *meltOptions,  MeltSetting *meltSetting) {
    MeltSamplingElectrode *meltSamplingElectrode = meltOptions->meltSamplingElectrode();
    auto perc = meltSamplingElectrode->samplingPerception();
    for(const ElectrodeNode* electrodeNode:meltSamplingElectrode->getDatas()) {
        ChannelData::List samplingDatas = electrodeDatas[electrodeNode];
        if (samplingDatas.empty()) {
            continue;
        }
        MeltSurvey meltSurvey(samplingDatas);
        auto nodeLatData = meltSurvey.calculateSamplingData(electrodeNode, consultDataIndex, meltSetting, perc);
        emit sampleDataFinished(electrodeNode, samplingDatas, consultDataIndex, nodeLatData, meltOptions, meltSetting);
    }
}

std::vector<qint32> MeltWorker::calculateConsultDataIndex(const ChannelData::List &consultDatas, MeltOptions *meltOptions,  MeltSetting *meltSetting) {
    MeltSamplingElectrode *meltSamplingElectrode = meltOptions->meltSamplingElectrode();
    auto avalideSize = meltSetting->max() / 2;
    auto datasMiddleIndex = consultDatas.size() / 2;
    int bIndex = datasMiddleIndex - avalideSize;
    MeltSurvey consultSurvey(consultDatas);
    return consultSurvey(meltSetting->consultPerception(),bIndex, datasMiddleIndex + avalideSize);
}

ElectrodeNodeHash MeltWorker::filterChannelData(const QList<ChannelData> &channelDatas, MeltOptions *meltOptions,  MeltSetting *meltSetting) {
    ElectrodeNodeHash electrodeDatas;
    ElectrodeNode *electrodeNode = m_catheterDb->getElectrodeNode(meltSetting->getConsultElectrodeId());;
    if (electrodeNode != nullptr && electrodeNode->seat() != ElectrodeNode::UNSET) {
        electrodeDatas[electrodeNode] = filterChannelData(electrodeNode,channelDatas,meltOptions);;
    }
    for(const ElectrodeNode* electrodeNode:meltOptions->meltSamplingElectrode()->getDatas()) {
        if (electrodeNode->seat() != ElectrodeNode::UNSET) {
            electrodeDatas[electrodeNode] = filterChannelData(electrodeNode,channelDatas,meltOptions);
        }
    }
    for(const ElectrodeNode* electrodeNode:meltOptions->appointElectrode()->getDatas()) {
        if (electrodeNode->seat() != ElectrodeNode::UNSET) {
            electrodeDatas[electrodeNode] = filterChannelData(electrodeNode,channelDatas,meltOptions);
        }
    }
    return electrodeDatas;
}

ChannelData::List MeltWorker::filterChannelData(const ElectrodeNode* electrodeNode, const QList<ChannelData> &channelDatas,  MeltOptions *meltOptions) {
    Q_ASSERT(m_channelDataFilter != nullptr);
    Q_ASSERT(electrodeNode != nullptr);
    ChannelDataCalculate channelCalc(electrodeNode);
    ChannelData::List dataList(channelDatas.size());
    std::transform(channelDatas.begin(), channelDatas.end(), dataList.begin(), channelCalc);
    //return meltOptions->id() == 0?m_channelDataFilter->pass(dataList, electrodeNode):m_channelDataFilter->passNoState(dataList, electrodeNode);
    return m_channelDataFilter->passNoState(dataList, electrodeNode);
}

void MeltWorker::setChannelDataFilter(ChannelDataFilter *channelDataFilter) {
    Q_ASSERT(channelDataFilter != nullptr);
    m_channelDataFilter = channelDataFilter;
}
