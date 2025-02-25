#include "Casket.h"
#include "utility/ConnectCheck.h"
#include "CasketListModel.h"
#include "casket/CasketDb.h"

#include <catheter/Catheter.h>
#include <catheter/CatheterDb.h>

#include <profile/Profile.h>

CasketListModel::CasketListModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

void CasketListModel::setProfile(Profile* profile) {
    if (m_profile == profile) {
        return;
    }
    beginResetModel();
    m_profile = profile;
    m_casketDb = profile->casketDb();
    QObject::connect(m_casketDb, &CasketDb::imported, this, &CasketListModel::onCasketImported);

    m_catheterDb = profile->catheterDb();
    QObject::connect(m_catheterDb, &CatheterDb::deleted, this, &CasketListModel::onCatheterDeleted);
    QObject::connect(m_catheterDb, &CatheterDb::typeChanged, this, &CasketListModel::onCatheterTypeChanged);

    endResetModel();
    emit profileChanged();
}

Profile *CasketListModel::profile() const {
    return m_profile;
}

int CasketListModel::rowCount(const QModelIndex&) const {
    return m_casketDb ==nullptr?0:m_casketDb->size();
}

Qt::ItemFlags CasketListModel::flags(const QModelIndex& index) const
{
    return Qt::ItemIsSelectable;
}

void CasketListModel::onCatheterDeleted(Catheter *catheter) {
    beginResetModel();
    m_casketDb->reset(catheter->id());
    catheter->resetElectrodeSeat();
    m_casketDb->save();
    endResetModel();
}
void CasketListModel::onCasketImported() {
    beginResetModel();
    endResetModel();
}

void CasketListModel::onCatheterTypeChanged(Catheter *catheter) {
    beginResetModel();endResetModel();
}


QVariant CasketListModel::data(const QModelIndex& index, int role) const {
    if (index.isValid()) {
        Casket *casket = m_casketDb->getData(index.row());
        if (casket != nullptr) {
            switch (role) {
            case Qt::DisplayRole:
            case Name:
                return casket->name();
            case BOX:
                return casket->box();
            case IDX:
                return casket->index();
            case CATHETER:
                return casket->catheterId();
            case RESERVED:
                return casket->reserved();
            }
        }
    }
    return QVariant();
}


QHash<int, QByteArray> CasketListModel::roleNames() const {
    return {
            {Name, "name"},
            {Id, "id"},
        {BOX, "box"},
        {IDX, "idx"},
        {CATHETER, "catheter"},
        {RESERVED, "reserved"},
        };
}

Casket *CasketListModel::get(int index) const
{
    return m_casketDb->getData(index);
}

void CasketListModel::occupy(int index, const QString &catheterId) {
    Catheter *catheter = m_catheterDb->getData(catheterId);
    if (catheter == nullptr) {
        return;
    }
    beginResetModel();
    int amount = catheter->getAmount();
    m_casketDb->occupy(index, catheterId);
    endResetModel();
}

void CasketListModel::reset(int index) {
    beginResetModel();
    m_casketDb->reset(index);
    endResetModel();
}
