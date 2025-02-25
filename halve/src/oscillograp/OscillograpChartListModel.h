#ifndef CHANNELLISTMODEL_H
#define CHANNELLISTMODEL_H

#include "catheter/ElectrodeNodeItemListModel.h"

class CatheterDb;
class Catheter;
class ElectrodeNode;
class Profile;
class OscillograpElectrode;
class ElectrodeSurveyor;
class OscillograpOptions;

class OscillograpChartListModel :public ElectrodeNodeItemListModel
{
    Q_OBJECT
    Q_PROPERTY(Profile* profile WRITE setProfile READ profile NOTIFY profileChanged);
public:
    explicit OscillograpChartListModel(QObject *parent = nullptr);
    void setProfile(Profile* profile);
    Profile *profile() const;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

public slots:
    void onPantSamplingChanged();
    void onElectrodeAdded(ElectrodeNode* node);
    void onElectrodeDeleted(ElectrodeNode* node);
    void onElectrodeTypeChanged(ElectrodeNode* node);
    void onEcgSurveyorChanged();
    void onBodySurveyorChanged();
    void onSortRoleChanged();

signals:
    void profileChanged();
protected:
    QPointer<Profile> m_profile;
    QPointer<OscillograpElectrode> m_oscillograpElectrode;
    QPointer<OscillograpOptions> m_oscillograpOptions;

};

#endif // CHANNELLISTMODEL_H
