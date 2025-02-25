#ifndef MAPPINGPOINTCOPYER_H
#define MAPPINGPOINTCOPYER_H

#include "channel/ChannelData.h"
#include <QObject>
#include <HalveType.h>
#include <QPointer>
#include <QMap>
#include <QMutex>
#include <QFuture>

class ChannelDataFilter;
class Profile;
class MeltSetting;
class MeltOptions;
class ElectrodeNode;
class MeltSamplingElectrode;
class MappingPointsDb;
class MeltChannelDb;
class MappingPointGroupDb;
class CatheterDb;
class MeltOptionsDb;
class MappingPointGroup;
class MappingSetting;

class MappingPointCopyer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Profile* profile WRITE setProfile READ profile NOTIFY profileChanged);
    Q_PROPERTY(ChannelDataFilter *channelDataFilter WRITE setChannelDataFilter FINAL)
    Q_PROPERTY(double progressValue READ progressValue WRITE setProgressValue NOTIFY progressValueChanged FINAL)

public:
    explicit MappingPointCopyer(QObject *parent = nullptr);

    void setProfile(Profile* profile);
    Profile *profile() const;
    void setChannelDataFilter(ChannelDataFilter *channelDataFilter);
    Q_INVOKABLE void copy(const QString &group, const QList<qint64> &ids);
    Q_INVOKABLE void attorn(const QString &group, const QList<qint64> &ids);


    double progressValue() const;
    void setProgressValue(double newProgressValue);

private:
    QJsonObject remainElectrode(QJsonObject meltOptionJson, const QList<QString>& electrodeIds);
    QFuture<void> clone(MappingPointGroup *pointToGroup, MappingSetting *mappingSetting, MeltSetting *meltSetting,ElectrodeNode *consultElectrode);
    void copyMappingPoint(const QString &group, const QList<qint64>& mappingPointIds, MappingSetting *mappingSetting, MeltSetting* meltSetting, MeltOptions* meltOption, ElectrodeNode* consultElectrode, QList<ChannelData> &channelData);

signals:
    void profileChanged();
    void copyFinished(QList<qint64> ids);
    void attornFinished(QList<qint64> ids);
    void progressValueChanged();

private:
    QMutex m_mutex;
    double m_progressValue = 0;
    QPointer<Profile> m_profile;
    QPointer<ChannelDataFilter> m_channelDataFilter;
    QPointer<MappingPointsDb> m_mappingPointsDb;
    QList<qint64> m_mappingPointIds;
    QPointer<CatheterDb> m_catheterDb;
    QPointer<MeltOptionsDb> m_meltOptionsDb;
    QPointer<MeltChannelDb> m_meltChannelDb;
    QPointer<MappingPointGroupDb> m_mappingPointGroupDb;

    QMap<quint64,QList<qint64>> m_consultPointMap;
    QList<QPair<quint64,MeltOptions*>> m_meltOptions;
    QAtomicInt m_meltOptionIndex = 0;
    QAtomicInteger<quint64> m_currentTime;
};

#endif // MAPPINGPOINTCOPYER_H
