#ifndef MELTELECTRODENODEFILTERMODEL_H
#define MELTELECTRODENODEFILTERMODEL_H

#include <QPointer>
#include <QSortFilterProxyModel>

class ExposedElectrode;
class Profile;
class MeltSamplingElectrode;

class MeltElectrodeNodeFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(ExposedElectrode *exposedElectrode READ exposedElectrode WRITE setExposedElectrode NOTIFY exposedElectrodeChanged FINAL)
    Q_PROPERTY(Profile* profile WRITE setProfile READ profile NOTIFY profileChanged);
    Q_PROPERTY(QString consultElectrodeId READ consultElectrodeId WRITE setConsultElectrodeId NOTIFY consultElectrodeChanged FINAL)

public:
    explicit MeltElectrodeNodeFilterModel(QObject *parent = nullptr);
    void setProfile(Profile* profile);
    Profile *profile() const;

    ExposedElectrode *exposedElectrode() const;
    void setExposedElectrode(ExposedElectrode *newExposedElectrode);

    QString consultElectrodeId() const;
    void setConsultElectrodeId(const QString &newConsultElectrodeId);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
public slots:
    void onExposedElectrodeChanged();

signals:

    void profileChanged();
    void exposedElectrodeChanged();

    void consultElectrodeChanged();

private:
    QPointer<Profile> m_profile;
    QString m_consultElectrodeId;
    QPointer<ExposedElectrode> m_exposedElectrode;

};

#endif // MELTELECTRODENODEFILTERMODEL_H
