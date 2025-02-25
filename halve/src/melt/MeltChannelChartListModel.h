#ifndef MELTCHANNELCHARTLISTMODEL_H
#define MELTCHANNELCHARTLISTMODEL_H


#include "catheter/ElectrodeNodeItemListModel.h"

class MeltSamplingElectrode;
class CatheterDb;
class Profile;
class ElectrodeNode;
class AppointElectrode;
class Catheter;
class MeltSetting;

class MeltChannelChartListModel :public ElectrodeNodeItemListModel
{
    Q_OBJECT
    Q_PROPERTY(Profile* profile WRITE setProfile READ profile NOTIFY profileChanged);

public:
    explicit MeltChannelChartListModel(QObject *parent = nullptr);
    ~MeltChannelChartListModel() = default;
    void setProfile(Profile* profile);
    Profile *profile() const;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
public slots:
    void onElectrodeAdded(ElectrodeNode* node);
    void onElectrodeDeleted(ElectrodeNode* node);
    void onElectrodeTypeChanged(ElectrodeNode* node);
    void onEcgSurveyorChanged();
    void onBodySurveyorChanged();
signals:
    void profileChanged();


private:
    QPointer<Profile> m_profile;
    QPointer<MeltSamplingElectrode> m_meltSamplingElectrode;
    QPointer<AppointElectrode> m_appointElectrode;
    QPointer<MeltSetting> m_meltSetting;

};

#endif // MELTCHANNELCHARTLISTMODEL_H
