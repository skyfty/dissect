#ifndef CONSULTELECTRODELISTMODEL_H
#define CONSULTELECTRODELISTMODEL_H

#include "catheter/ElectrodeNodeListModelBase.h"

class Profile;
class MeltSetting;
class CatheterDb;
class Catheter;
class ElectrodeNode;

class MeltConsultElectrodeListModel : public ElectrodeNodeListModelBase
{
    Q_OBJECT
    Q_PROPERTY(Profile* profile WRITE setProfile READ profile NOTIFY profileChanged);
    Q_PROPERTY(QString consultElectrodeId WRITE setConsultElectrodeId);

public:
    explicit MeltConsultElectrodeListModel(QObject *parent = nullptr);
    void setProfile(Profile* profile);
    Profile *profile() const;
    void setConsultElectrodeId(const QString &consultElectrode);

    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
public slots:
    void onCatheterDeleted(Catheter *catheter);
    void onCatheterEmployChanged(Catheter *catheter);

signals:
    void profileChanged();
    void deleted(ElectrodeNode* node);

private:
    QPointer<Profile> m_profile;
    QPointer<CatheterDb> m_catheterDb;

};

#endif // CONSULTELECTRODELISTMODEL_H
