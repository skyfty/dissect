#ifndef ELECTRODENODELISTMODEL_H
#define ELECTRODENODELISTMODEL_H

#include "catheter/ElectrodeNodeListModelBase.h"

class Catheter;
class Profile;
class CatheterDb;

class ElectrodeNodeListModel : public ElectrodeNodeListModelBase
{
    Q_OBJECT
    Q_PROPERTY(Profile* profile WRITE setProfile READ profile NOTIFY profileChanged);
public:
    explicit ElectrodeNodeListModel(QObject *parent = nullptr);
    virtual void setProfile(Profile* profile);
    Profile *profile() const;
    Q_INVOKABLE QVariant data(const QString &id, int role) const;
    using ElectrodeNodeListModelBase::data;

    Q_INVOKABLE virtual void reset();
public slots:
    void onCatheterElectrodeChanged(Catheter *catheter);
    void onCatheterImported();

signals:
    void profileChanged();
protected:
    QPointer<Profile> m_profile;
    QPointer<CatheterDb> m_catheterDb;
};

#endif // ELECTRODENODELISTMODEL_H
