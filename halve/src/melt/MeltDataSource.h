#ifndef MELTDATASOURCE_H
#define MELTDATASOURCE_H

#include <QFuture>
#include <QHash>
#include <vtkBoundingBox.h>
#include <vtkVector.h>
#include "combined/CatheterTrack.h"
#include "melt/MeltOptions.h"
#include "melt/MeltSetting.h"
#include "melt/MeltType.h"
#include "melt/MeltSurvey.h"

class QAbstractSeries;
class ElectrodeNode;
class CatheterTrackPackage;
class QXYSeries;
class MeltSamplingElectrode;
class Channel;
class MeltSampleLine;
class CatheterDb;
class Catheter;
class AppointElectrode;
class MappingPointsDb;
class ChannelDataFilter;
class MappingPoint;
class Profile;
class Combined;
class MeltWorker;
class Mapping;
class MeltOptions;
class MeltOptionsDb;
class BreathSurvey;
class MeltChannelDb;
class MappingPointGroup;
class MappingPointGroupDb;
class MappingSetting;

class MeltDataSource : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Profile* profile WRITE setProfile READ profile NOTIFY profileChanged);
    Q_PROPERTY(Combined* combined WRITE setCombined FINAL);
    Q_PROPERTY(Channel *channel WRITE setChannel FINAL)
    Q_PROPERTY(bool updating READ updating WRITE setUpdating NOTIFY updatingChanged FINAL)
    Q_PROPERTY(ChannelDataFilter *channelDataFilter WRITE setChannelDataFilter FINAL)
    Q_PROPERTY(MeltOptions *meltOptions READ meltOptions  WRITE setMeltOptions  NOTIFY meltOptionsChanged FINAL)
    Q_PROPERTY(MeltSetting *meltSetting READ meltSetting WRITE setMeltSetting NOTIFY meltSettingChanged FINAL)
    Q_PROPERTY(MappingSetting *mappingSetting READ mappingSetting WRITE setMappingSetting NOTIFY mappingSettingChanged FINAL)
    Q_PROPERTY(QString consultElectrodeId READ consultElectrodeId WRITE setConsultElectrodeId NOTIFY consultElectrodeIdChanged FINAL)
    Q_PROPERTY(BreathSurvey *breathSurvey WRITE setBreathSurvey FINAL)

public:
    enum Review  {
        PREVIOUS,
        NEXT
    };
    Q_ENUM(Review);

public:
    explicit MeltDataSource(QObject *parent = nullptr);
    ~MeltDataSource();
    void setProfile(Profile* profile);
    Profile *profile() const;
    Q_INVOKABLE void update();
    Q_INVOKABLE void resetSeriesSize();

    Q_INVOKABLE void setSeries(const QString& id, const ElectrodeNode *node, QAbstractSeries *series, MeltSampleLine *sampleLine);
    Q_INVOKABLE void setConsultSeries(QAbstractSeries* series);
    void setChannelDataFilter(ChannelDataFilter *channelDataFilter);
    Q_INVOKABLE void setSampleLineIndex(const QString &id, qint32 index);
    Q_INVOKABLE void saveSampleLineIndex(const QString &id);
    void setCombined(Combined *combined);

    void setChannel(Channel *newChannel);
    Channel* channel() const;

    void setMeltOptions(MeltOptions *meltOptions);
    MeltOptions* meltOptions() const;

    Q_INVOKABLE void showConsultData(Review nav);
    Q_INVOKABLE void reloadSeriesData();
    Q_INVOKABLE void restrikConsultData();

    Q_INVOKABLE void strik();
    Q_INVOKABLE void adjust();

    qint32 consultDataIndex() const;

    bool updating() const;
    void setUpdating(bool newUpdating);

    MeltSetting *meltSetting() const;
    void setMeltSetting(MeltSetting *newMeltSetting);

    MappingSetting *mappingSetting() const;
    void setMappingSetting(MappingSetting *newMeltSetting);

    QString consultElectrodeId() const;
    void setConsultElectrodeId(const QString &consultElectrodeNodeId);

    double basic() const;
    void setBasic(double newBasic);

    void setBreathSurvey(BreathSurvey *newBreathSurvey);

signals:
    void profileChanged();
    void consultDataIndexChanged();
    void updatingChanged();
    void meltOptionsChanged();
    void mappingSettingChanged();
    void striked();
    void channelChanged();
    void meltSettingChanged();
    void consultElectrodeIdChanged();
    void resetMappingPointFinished();

private slots:
    void onCatheterChanged(Catheter* catheter);
    void onCurrentMappingPointIdChanged();
    void onCurrentMappingPointGroupChanged();
    void onChannelSampleChanged(const QList<ChannelData> &channelDatas);
    void onWorkerUpdateFinished(const ElectrodeNodeHash &electrodeDatas,  MeltOptions *meltOptions,  MeltSetting *meltSetting);
    void onWorkerSampleDataFinished(const ElectrodeNode* electrodeNode,const ChannelData::List &datas, qint32 consultDataIndex, const ElectrodeNodeFruit &lat,  MeltOptions *meltOptions,  MeltSetting *meltSetting);
    void onWorkerConsultDataFinished(const ChannelData::List &datas, qint32 consultDataIndex ,const std::vector<qint32> &consultDataIndexList,  MeltOptions *meltOptions,  MeltSetting *meltSetting);
    void onWorkerAppointDataFinished(const ElectrodeNode* electrodeNode,const ChannelData::List &datas, qint32 consultDataIndex,  MeltOptions *meltOptions,  MeltSetting *meltSetting);
    void onCatheterTrackChanged(const QSharedPointer<CatheterTrackPackage> &trackDataList);
    void onStateChanged();
    void onMappintPointDeleted(qint64 rowId);
    void onMappingPointsCleaned();

private:
    void connectWorkerSignal();
    void resetSeriesSize(QXYSeries *xyseries);
    SeriesIndex resetSeriesData(QXYSeries *series, int consultDataIndex, const ChannelData::List &datas, qint32 latDataIndex, qint32 voltageIdx);

    void resetMappingPointSeriesData(const QList<MappingPoint> &mappingPointList);
    bool getTrackPosition(const CatheterTrack &track, vtkVector3d &position);

    void setLatPerimeter(const std::vector<int> &indexDatas);
    qint32 convertSeriesIndexToDataIndex(qint32 seriesIndex, const ChannelData::List &samplingDatas);
    MappingPoint createCurrentMappingPoint(qint64 currentTime, const QString &electrodeId);
    MeltSampleLine* setMeltSampleLineData(QXYSeries* series,const ElectrodeNode* electrodeNode, const ChannelData::List &samplingDatas, int consultDataIndex,const MappingPoint &mappingPoint);
    MeltSampleLine* setMeltSampleLineData(QXYSeries* series,const ElectrodeNode* electrodeNode, const ChannelData::List &samplingDatas, int consultDataIndex,qint32 lat, qint32 latDataIndex, double maxVoltage, qint32 voltageIdx, double minVoltage, bool adjust);

    void loadLocateMappingPointData(const QList<ChannelData> &datas, qint64 meltOptionsId = 0);
    void onLoadLocateMappingPointData(QList<MappingPoint> mappingPoint);

    void strikSampleElectrode(const ElectrodeNode* electrodeNode,MeltSampleLine* meltSampleLine, qint64 currentTime);
    void strikConsultElectrode(const ElectrodeNode* electrodeNode,qint64 currentTime);
    QAbstractSeries* getSeriesFromElectrodeNode(const ElectrodeNode* node);
    std::optional<CatheterTrack> getElectrodeTrack(const ElectrodeNode* node) const;
    void addSampleMappintPoint(MappingPoint &mappingPoint,MeltSampleLine *meltSampleLine, const QString &electrodeId);
    void adjustMappingPoints(const QList<MappingPoint> &mappingPoint);
    void adjustSampleElectrode(MappingPoint mappingPoint, MeltSampleLine *meltSampleLine);
    void adjustConsultElectrode(MappingPoint mappingPoint);
    void saveMeltOptions(qint64 currentTime);
    MeltSampleLine* getMeltSampleLineFromElectrodeNode(const ElectrodeNode* node);

private:
    QPointer<Profile> m_profile;
    QPointer<Channel> m_channel;
    QPointer<MeltOptionsDb> m_meltOptionsDb;
    QPointer<MeltWorker> m_worker;
    QPointer<MeltChannelDb> m_meltChannelDb;
    QPointer<CatheterDb> m_catheterDb;
    QPointer<MappingPointsDb> m_mappingPointsDb;
    QPointer<MappingPointGroupDb> m_mappingPointGroupDb;
    MappingSetting *m_mappingSetting = nullptr;
    QPointer<ChannelDataFilter> m_channelDataFilter;
    QPointer<Combined> m_combined;
    QList<Catheter *> m_catheters;
    QPointer<ReseauDb> m_reseauDb;
    QList<ChannelData> m_channelDatas;
    QList<ChannelData> m_strikChannelDatas;
    ElectrodeNodeHash m_electrodeDatas;
    QPointer<BreathOptions> m_breathOptions;
    QPointer<BreathSurvey> m_breathSurvey;

    bool m_updating = false;
    QSharedPointer<CatheterTrackPackage> m_catheterTrack;
    QPointer<QXYSeries> m_consultSeries;
    QPointer<ElectrodeNode> m_consultElectrodeNode;
    MeltOptions* m_meltOptions = nullptr;
    MeltSetting* m_meltSetting = nullptr;

    struct SeriesNode {
        QAbstractSeries* series;
        const ElectrodeNode* electrodeNode;
        MeltSampleLine* sampleLine;
    };
    QHash<QString, SeriesNode> m_seriesData;
};

#endif // MELTDATASOURCE_H
