#include "Catheter.h"
#include "utility/ConnectCheck.h"
#include "CatheterListModel.h"
#include <catheter/CatheterDb.h>
#include <combined/Combined.h>
#include <profile/Profile.h>

CatheterListModel::CatheterListModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

void CatheterListModel::setProfile(Profile* profile) {
    if (m_profile == profile) {
        return;
    }
    m_profile = profile;

    m_catheterDb = profile->catheterDb();
    QObject::connect(m_catheterDb, &CatheterDb::deleted, this, &CatheterListModel::onCatheterChanged);
    QObject::connect(m_catheterDb, &CatheterDb::mixed, this, &CatheterListModel::onCatheterChanged);
    QObject::connect(m_catheterDb, &CatheterDb::typeChanged, this, &CatheterListModel::onCatheterChanged);
    QObject::connect(m_catheterDb, &CatheterDb::imported, this, &CatheterListModel::onResetModel);
    reset();
}

Profile *CatheterListModel::profile() const {
    return m_profile;
}

void CatheterListModel::onResetModel() {
    beginResetModel();
    reset();
    endResetModel();
}


Catheter *CatheterListModel::get(int index) const
{
    if (index <0 || index >= m_catheters.size()) {
        return nullptr;
    }
    return m_catheters[index];

}

QVariant CatheterListModel::data(const QString &id, int role) const
{
    Catheter *catheter = m_catheterDb->getData(id);
    return getData(catheter, role);
}

void CatheterListModel::reset()
{
    m_catheters = m_catheterDb->getDatas();
}

QVariant CatheterListModel::getData(Catheter *catheter, int role) const {
    if (catheter != nullptr) {
        switch (role) {
        case Type:
            return catheter->getType();
        case Qt::DisplayRole:
        case Name:
            return catheter->getName();
        case Id:
            return catheter->id();
        case Amount:
            return catheter->getAmount();
        case Employ:
            return catheter->employ();
        }
    }
    return QVariant();
}

void CatheterListModel::onCatheterChanged(Catheter *catheter) {
    onResetModel();
}

int CatheterListModel::rowCount(const QModelIndex&) const {
    return m_catheters.size();
}

Qt::ItemFlags CatheterListModel::flags(const QModelIndex& index) const
{
    return Qt::ItemIsSelectable;
}

QVariant CatheterListModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }
    Catheter *catheter = get(index.row());
    return getData(catheter, role);
}


QHash<int, QByteArray> CatheterListModel::roleNames() const {
    return {
            {Name, "name"},
            {Id, "id"},
        {Amount, "amount"},
        {Type, "type"},
            };
}
