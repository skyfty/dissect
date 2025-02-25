#ifndef MAPPINGPOINTRESTRIK_H
#define MAPPINGPOINTRESTRIK_H

#include "mapping/MappingPoint.h"
#include <QObject>
#include <HalveType.h>
#include <QPointer>
#include <QMutex>

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

class MappingPointRestrik : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Profile* profile WRITE setProfile READ profile NOTIFY profileChanged);
    Q_PROPERTY(ChannelDataFilter *channelDataFilter WRITE setChannelDataFilter FINAL)
    Q_PROPERTY(double progressValue READ progressValue WRITE setProgressValue NOTIFY progressValueChanged FINAL)

public:
    explicit MappingPointRestrik(QObject *parent = nullptr);

    void setProfile(Profile* profile);
    Profile *profile() const;
    void setChannelDataFilter(ChannelDataFilter *channelDataFilter);
    Q_INVOKABLE void restrik(const QString &group, QList<qint64> ids);
    double progressValue() const;
    void setProgressValue(double newProgressValue);

signals:

signals:
    void profileChanged();
    void finished();
    void progressValueChanged();


private:
    void restrikMappingPoint(const QList<qint64>& ids, MeltSetting* meltSetting, MeltOptions* meltOption, ElectrodeNode* consultElectrode);
    void restrikMappingPoint(qint64 mappingPointId, const QList<ChannelData>& channelData, MeltSetting* meltSetting, int consultDataIndex, MeltSamplingElectrode* meltSamplingElectrode, QList<MappingPoint> &changedMappingPointList);

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
    QList<quint64> m_meltOptionIds;
    QAtomicInt m_meltOptionIndex = 0;
};

#endif // MAPPINGPOINTRESTRIK_H
