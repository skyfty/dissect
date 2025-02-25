#include "ReseauListModel.h"
#include "reseau/ReseauDb.h"
#include <profile/Profile.h>
#include "utility/ConnectCheck.h"

ReseauListModel::ReseauListModel(QObject *parent)
    : QAbstractListModel{parent}
{}

void ReseauListModel::setProfile(Profile* profile) {
    if (m_profile == profile) {
        return;
    }
    beginResetModel();
    m_profile = profile;
    m_reseauDb = profile->reseauDb();
    endResetModel();
    connect(m_reseauDb, &ReseauDb::added, this, &ReseauListModel::onReseauAdded);
    QObject::connect(m_reseauDb, &ReseauDb::cleaned, this, &ReseauListModel::onReseauCleaned);
    emit profileChanged();
}

Profile *ReseauListModel::profile() const {
    return m_profile;
}
QVariant ReseauListModel::getData(Reseau *reseau, int role) const {
    if (reseau != nullptr) {
        switch (role) {
        case Qt::DisplayRole:
        case Name:
            return reseau->getName();
        case Id:
            return reseau->id();
        case DYESTUFF:
            return reseau->dyestuff();
        case APPARENT:
            return reseau->apparent();
        case BUILDIN:
            return reseau->buildin();
        case INUSE: {
            return m_profile->currentReseauId() == reseau->id();
        }
        }
    }
    return QVariant();
}

bool ReseauListModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!index.isValid())
        return false;
    Reseau* reseau = m_reseauDb->getData(index.row());
    switch (role) {
    case Qt::EditRole:
    case Name: {
        QString name = value.toString();
        reseau->setName(name);
        break;
    }
    case APPARENT: {
        reseau->setApparent(value.value<bool>());
        break;
    }
    case INUSE: {
        auto currentIndex = this->index(indexFromId(m_profile->currentReseauId()), 0);
        m_profile->setCurrentReseauId(reseau->id());
        m_profile->save();
        emit dataChanged(currentIndex, currentIndex, QVector<int>() << role);
        break;
    }
    case DYESTUFF: {
        reseau->setDyestuff(value.value<QColor>());
        break;
    }
    default:
        return false;
    }
    m_reseauDb->save();
    emit dataChanged(index, index, QVector<int>() << role);
    return true;
}
void ReseauListModel::onReseauCleaned(){
    beginResetModel();
    endResetModel();
}
int ReseauListModel::rowCount(const QModelIndex&) const {
    return m_reseauDb == nullptr?0:m_reseauDb->size();
}

Qt::ItemFlags ReseauListModel::flags(const QModelIndex& index) const
{
    return Qt::ItemIsSelectable;
}

void ReseauListModel::onReseauAdded(Reseau* reseau) {
    beginInsertRows(QModelIndex(), 0, 0);
    m_reseauDb->save();
    endInsertRows();
}

QVariant ReseauListModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }
    if (index.row() < 0 || index.row()>=m_reseauDb->size()) {
        return QVariant();
    }
    return getData(m_reseauDb->getData(index.row()), role);
}

bool ReseauListModel::insertRows(int row, int count, const QModelIndex &parent) {
    m_reseauDb->add(QDateTime::currentMSecsSinceEpoch(), QString("Body#%1").arg(m_profile->increaseReseauId()));
    m_profile->save();
    return true;
}

bool ReseauListModel::removeRows(int row, int count, const QModelIndex& parent) {
    if (row != -1) {
        beginRemoveRows(parent, row, row);
        m_reseauDb->remove(row);
        m_reseauDb->save();
        endRemoveRows();
    }
    return true;
}

int ReseauListModel::indexFromId(qint64 id) const
{
    return m_reseauDb->index(id);
}

int ReseauListModel::indexOfName(const QString &name) const
{
    return m_reseauDb->indexOfName(name);
}

QHash<int, QByteArray> ReseauListModel::roleNames() const {
    return {
            {Name, "name"},
            {Id, "id"},
        {BUILDIN, "buildin"},
        {DYESTUFF, "dyestuff"},
        {APPARENT, "apparent"},
        {INUSE, "inuse"},
        };
}
