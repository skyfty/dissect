#ifndef CHANNELDATAFILTER_H
#define CHANNELDATAFILTER_H

#include "ChannelData.h"

#include <QHash>
#include <QMutex>
#include <QObject>
#include <QPointer>

#include "filter/FilterPipe.h"

class ElectrodeNode;
class Catheter;
class Profile;
class CatheterDb;
class FilterOptions;
class FilterOptionItem;

typedef ys::FilterPipe<ChannelData::DataType> FilterPipe;

class ChannelDataFilter : public QObject
{
    Q_OBJECT;
    Q_PROPERTY(Profile* profile WRITE setProfile READ profile NOTIFY profileChanged);
    Q_PROPERTY(quint16 samplingRate READ samplingRate WRITE setSamplingRate NOTIFY samplingRateChanged FINAL)

public:
    explicit ChannelDataFilter(QObject *parent = nullptr);
    ~ChannelDataFilter() override;

    ChannelData::List pass(const ChannelData::List &channelData,const ElectrodeNode *electrodeNode, bool batch = false) const;
    ChannelData::List& pass(ChannelData::List &channelData, const ElectrodeNode *electrodeNode, bool batch = false) const;
    ChannelData::List passNoState(ChannelData::List &channelData, const ElectrodeNode *electrodeNode) const;
    std::vector<ChannelData::DataType> pass(const  std::vector<ChannelData::DataType> &channelData, const ElectrodeNode *electrodeNode, bool batch = false) const;

    void setProfile(Profile* profile);
    Profile *profile() const;
    void setFilterOptions(FilterOptions* filterOptions);

    void changeState(bool b);

    quint16 samplingRate() const;
    void setSamplingRate(quint16 newSamplingRate);

public slots:
    void onEmployChanged(Catheter* catheter);
    void onCatheterDeleted(Catheter* catheter);
    void onFilterOptionItemChanged(FilterOptionItem* item);

signals:
    void profileChanged();
    void samplingRateChanged();

private:
    FilterPipe* getFilterPipes(const ElectrodeNode *electrodeNode) const;
    FilterPipe* createFilterPipes(int type, FilterOptionItem *filterEcgOption) const;
    void setFilterOptions(int type, FilterPipe* fp, FilterOptionItem* item) const;

private:
    QPointer<Profile> m_profile;
    QPointer<CatheterDb> m_catheterDb;
    QPointer<FilterOptions> m_filterOptions;
    quint16 m_samplingRate = -1;
    mutable QMutex m_mutex;
    mutable QHash<const ElectrodeNode*,FilterPipe*> m_ecgFilterPipes;
    mutable QHash<const ElectrodeNode*,FilterPipe*> m_singleFilterPipes;
    mutable QHash<const ElectrodeNode*,FilterPipe*> m_doubleFilterPipes;


};


#endif // CHANNELDATAFILTER_H
