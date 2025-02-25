#include "Melt.h"
#include "profile/Profile.h"
#include "MeltTableModel.h"
#include "melt/MeltDb.h"
#include "utility/ConnectCheck.h"

MeltTableModel::MeltTableModel(QObject *parent)
    : QAbstractTableModel{parent}
{

}

void MeltTableModel::setProfile(Profile* profile) {
    if (m_profile == profile) {
        return;
    }
    m_meltDb = m_profile->meltDb();
    connect(m_meltDb, &MeltDb::added, this, &MeltTableModel::onAdded, Qt::UniqueConnection);
    emit profileChanged();
}

Profile *MeltTableModel::profile() const {
    return m_profile;
}
int MeltTableModel::rowCount(const QModelIndex&) const {
    return  m_meltDb == nullptr?0:m_meltDb->size();
}

int MeltTableModel::columnCount(const QModelIndex&) const {
    return DataRoles::Last - DataRoles::First - 1;
}


Qt::ItemFlags MeltTableModel::flags(const QModelIndex& index) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant MeltTableModel::data(const QModelIndex &index, int role) const {
    if (index.isValid()) {
        Melt *melt = m_meltDb->getData(index.row());
        if (melt != nullptr) {
            switch (role) {
            case Dyestuff:
                return melt->dyestuff();
            case Description:
                return melt->description();
            case Appeared:
                return melt->appeared();
            case DescriptionAppeared:
                return melt->descriptionAppeared();
            case Diameter:
                return melt->diameter();
            case Order:
                return melt->order();
            }
        }
    }
    return QVariant();
}

bool MeltTableModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!index.isValid())
        return false;
    Melt *melt = m_meltDb->getData(index.row());
    if (melt != nullptr) {
        switch (role) {
        case Dyestuff:
            melt->setDyestuff(value.value<QColor>());
            break;
        case Description:
            melt->setDescription(value.toString());
            break;
        case Appeared:
            melt->setAppeared(value.value<bool>());
            break;
        case DescriptionAppeared:
            melt->setDescriptionAppeared(value.value<bool>());
            break;
        case Diameter:
            melt->setDiameter(value.value<int>());
            break;
        case Order:
            melt->setOrder(value.value<int>());
            break;
        default:
            return false;
        }
    }
    emit dataChanged(index, index, QVector<int>() << role);
    return true;
}

QHash<int, QByteArray> MeltTableModel::roleNames() const {
    return {
            {Description, "description"},
            {Dyestuff, "dyestuff"},
            {Appeared, "appeared"},
            {DescriptionAppeared, "descriptionAppeared"},
            {Diameter, "diameter"},
            {Order, "order"},
            };
}


void MeltTableModel::remove(int row) {
    beginRemoveRows(QModelIndex(), row, row);
    m_meltDb->remove(row);
    endRemoveRows();
}

void MeltTableModel::onAdded(const Melt* melt) {
    beginResetModel(); endResetModel();
}
