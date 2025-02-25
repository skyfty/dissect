#ifndef MELTWORKER_H
#define MELTWORKER_H

#include "melt/MeltType.h"
#include "HalveType.h"
#include <QHash>
#include <QObject>
#include <QPointer>

class QThread;
class ElectrodeNode;
class QAbstractSeries;
class ChannelDataFilter;
class MeltSamplingElectrode;
class Profile;
class AppointElectrode;
class MeltOptions;
class MeltSetting;
class CatheterDb;
class Channel;

class MeltWorker : public QObject
{
    Q_OBJECT
public:
    explicit MeltWorker(QObject *parent = nullptr);
    ~MeltWorker();
    void init(CatheterDb *catheterDb);
    void exit();

    ElectrodeNodeHash filterChannelData(const QList<ChannelData> &channelDatas,  MeltOptions *meltOptions,  MeltSetting *meltSetting);
    ChannelData::List filterChannelData(const ElectrodeNode* electrodeNode, const QList<ChannelData> &channelDatas,  MeltOptions *meltOptions);
    void calculateMeltData(const QList<ChannelData>& channelDatas, qint32 consultDataIndex, const ElectrodeNodeHash& electrodeDatas,  MeltOptions *meltOptions,  MeltSetting *meltSetting);
    void calculateSamplingData();
    std::vector<qint32> calculateConsultDataIndex(const ChannelData::List &consultDatas, MeltOptions *meltOptions,  MeltSetting *meltSetting);
    void calculateSamplingData(const ElectrodeNodeHash &electrodeDatas, qint32 consultDataIndex, MeltOptions *meltOptions,  MeltSetting *meltSetting);
    void calculateAppointData(const ElectrodeNodeHash &electrodeDatas, qint32 consultDataIndex, MeltOptions *meltOptions,  MeltSetting *meltSetting);
    void setChannelDataFilter(ChannelDataFilter *channelDataFilter);

signals:
    void update(const QList<ChannelData>& channelDatas, qint32 consultDataIndex,  MeltOptions *meltOptions,  MeltSetting *meltSetting);
    void updateFinished(const ElectrodeNodeHash &electrodeDatas, MeltOptions *meltOptions,  MeltSetting *meltSetting);
    void consultDataFinished(const ChannelData::List &datas, qint32 consultDataIndex ,const std::vector<qint32> &consultDataIndexList,  MeltOptions *meltOptions,  MeltSetting *meltSetting);
    void sampleDataFinished(const ElectrodeNode* electrodeNode,const ChannelData::List &datas, qint32 consultDataIndex, const ElectrodeNodeFruit &lat,  MeltOptions *meltOptions,  MeltSetting *meltSetting);
    void appointDataFinished(const ElectrodeNode* electrodeNode,const ChannelData::List &datas,qint32 consultDataIndex,  MeltOptions *meltOptions,  MeltSetting *meltSetting);
public slots:
    void onUpdating(const QList<ChannelData>& channelDatas, qint32 consultDataIndex,  MeltOptions *meltOptions,  MeltSetting *meltSetting);

private:
    QPointer<QThread> m_thread;
    QPointer<ChannelDataFilter> m_channelDataFilter;
    QPointer<CatheterDb> m_catheterDb;

};

#endif // MELTWORKER_H
