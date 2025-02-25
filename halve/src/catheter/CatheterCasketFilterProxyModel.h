#ifndef CATHETERCASKETFILTERPROXYMODEL_H
#define CATHETERCASKETFILTERPROXYMODEL_H

#include <QPointer>
#include <QSortFilterProxyModel>

class CasketDb;
class Profile;

class CatheterCasketFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(int box READ box WRITE setBox NOTIFY boxChanged FINAL)
    Q_PROPERTY(Profile* profile WRITE setProfile FINAL);

public:
    explicit CatheterCasketFilterProxyModel(QObject *parent = nullptr);

    int box() const;
    void setBox(int newBox);
    void setProfile(Profile *newProfile);

signals:
    void boxChanged();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    int m_box = 0;
    QPointer<Profile> m_profile;
    QPointer<CasketDb> m_casketDb;
};

#endif // CATHETERCASKETFILTERPROXYMODEL_H
