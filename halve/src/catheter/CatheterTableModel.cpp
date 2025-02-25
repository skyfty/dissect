#include "catheter/CatheterTableModel.h"
#include "catheter/Catheter.h"
#include "catheter/CatheterMagnetism.h"
#include "utility/ConnectCheck.h"
#include "catheter/CatheterDb.h"
#include "profile/Profile.h"
#include "CatheterType.h"
#include "CatheterTypeDb.h"

#include <profile/Profile.h>

CatheterTableModel::CatheterTableModel(QObject *parent)
    :QAbstractTableModel(parent){
}

void CatheterTableModel::setProfile(Profile* profile) {
    if (m_profile == profile) {
        return;
    }
    m_profile = profile;
    m_catheterDb = m_profile->catheterDb();
    QObject::connect(m_catheterDb, &CatheterDb::mixed, this, &CatheterTableModel::onMixed);
    QObject::connect(m_catheterDb, &CatheterDb::alined, this, &CatheterTableModel::onResetModel);
    QObject::connect(m_catheterDb, &CatheterDb::imported, this, &CatheterTableModel::onResetModel);
    emit profileChanged();
}

Profile *CatheterTableModel::profile() const {
    return m_profile;
}

void CatheterTableModel::setCatheterTypeDb(CatheterTypeDb* catheterTypeDb) {
    oc::disconnect(m_catheterTypeDb, nullptr, this, nullptr);
    m_catheterTypeDb = catheterTypeDb;
    QObject::connect(m_catheterTypeDb, &CatheterTypeDb::nameChanged, this, &CatheterTableModel::onCatheterTypeNameChanged);
    QObject::connect(m_catheterTypeDb, &CatheterTypeDb::deleted, this, &CatheterTableModel::onCatheterTypeDeleted);

}

void CatheterTableModel::onResetModel() {
    beginResetModel(); endResetModel();
}

int CatheterTableModel::rowCount(const QModelIndex&) const {
    return m_catheterDb == nullptr?0:m_catheterDb->size();
}

int CatheterTableModel::columnCount(const QModelIndex&) const {
    return 6;
}

QVariant CatheterTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();
    Catheter *catheter = m_catheterDb->getData(index.row());
    if (catheter == nullptr) {
        return QVariant();
    }
    return data(catheter,role);
}

QVariant CatheterTableModel::data(Catheter *catheter, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
    case Name:
        return catheter->getName();
    case Amount:
        return catheter->getAmount();
    case Manufacturer:
        return catheter->getManufacturer();
    case Type:
        return catheter->getType();
    case Version:
        return catheter->getVersion();
    case Gap: {
        QStringList sl;
        for(quint16 v:catheter->getGap()) {
            sl.append(QString::number(v));
        }
        return sl;
    }
    case Fr:
        return catheter->getFr();
    case Diameter:
        return catheter->getDiameter();
    case Far:
        return catheter->getFar();
    case Electrode:
        return catheter->getElectrode();

    case Employ: {
        return catheter->employ();
    }
    case DESTROYED: {
        return catheter->destroyed();
    }
    case Dyestuff: {
        return catheter->getDyestuff();
    }
    case MultiPassEnable: {
        return catheter->getMultiPassEnable();
    }
    case MultiLowPass: {
        return catheter->getMultiLowPass();
    }
    case MultiHeighPass: {
        return catheter->getMultiHeighPass();
    }
    case SinglePassEnable: {
        return catheter->getSinglePassEnable();
    }
    case SingleLowPass: {
        return catheter->getSingleLowPass();
    }
    case SingleHeighPass: {
        return catheter->getSingleHeighPass();
    }
    case ShowPassData: {
        return catheter->getShowPassData();
    }
    case Magnetism: {
        return catheter->magnetism();
    }
    case MagnetismConsult: {
        return catheter->catheterMagnetism()->consult();
    }
    case MagnetismConsultDistance: {
        return catheter->catheterMagnetism()->consultDistance();
    }
    case MagnetismTarget: {
        return catheter->catheterMagnetism()->target();
    }
    case MagnetismTargetDistance: {
        return catheter->catheterMagnetism()->targetDistance();
    }
    case APPARENT: {
        return catheter->apparent();
    }
    case Builtin: {
        return catheter->getBuiltin();
    }
    case Prefab: {
        return catheter->prefab();
    }
    case ElectrodeLength: {
        return catheter->electrodeLength();
    }
    case MagnetismDescription: {
        return catheter->catheterMagnetism()->description();
    }
    case Port: {
        return catheter->port();
    }
    case ACTIVE: {
        if (m_profile == nullptr) {
            return false;
        }
        return catheter->id() == m_profile->reproduceOptions()->catheterId();
    }
    }
    return QVariant();
}

bool CatheterTableModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!index.isValid())
        return false;
    Catheter *catheter = m_catheterDb->getData(index.row());
    if (catheter == nullptr) {
        return false;
    }
    QVector<int> roles ;
    roles << role;

    switch (role) {
    case Name:
        catheter->setName(value.toString());break;
    case Amount:
        catheter->setAmount(value.value<int>());
        break;
    case Manufacturer:
        catheter->setManufacturer(value.toString());break;
    case Type: {
        catheter->setType(value.toString());
        break;
    }
    case Version:
        catheter->setVersion(value.toString());break;
    case Gap: {
        QList<quint16> result;
        for(const QString in :value.toStringList()) {
            result.append(in.toUInt());
        }
        catheter->setGap(result);
        break;
    }
    case Fr:
        catheter->setFr(value.toString());break;
    case Diameter:
        catheter->setDiameter(value.value<double>());break;
    case Far:
        catheter->setFar(value.value<double>());break;
    case Electrode:
        catheter->setElectrode((Halve::ElectrodeType)value.value<int>());break;
    case Employ: {
        catheter->setEmploy(value.value<bool>());break;
    }
    case Magnetism: {
        catheter->setMagnetism(value.value<bool>());
        roles << MagnetismDescription;break;
    }
    case MagnetismConsult: {
        catheter->catheterMagnetism()->setConsult(value.value<int>());
        roles << MagnetismDescription;break;
    }
    case MagnetismConsultDistance: {
        catheter->catheterMagnetism()->setConsultDistance(value.value<double>());
        roles << MagnetismDescription;break;
    }
    case MagnetismTarget: {
        catheter->catheterMagnetism()->setTarget(value.value<int>());
        roles << MagnetismDescription;break;
    }
    case MagnetismTargetDistance: {
        catheter->catheterMagnetism()->setTargetDistance(value.value<double>());
        roles << MagnetismDescription;break;
    }
    case Dyestuff: {
        catheter->setDyestuff(value.value<QColor>());break;
        break;
    }
    case MultiPassEnable: {
        catheter->setMultiPassEnable(value.value<bool>());break;
        break;
    }
    case MultiLowPass: {
        catheter->setMultiLowPass(value.value<double>());break;
        break;
    }
    case MultiHeighPass: {
        catheter->setMultiHeighPass(value.value<double>());break;
        break;
    }
    case SinglePassEnable: {
        catheter->setSinglePassEnable(value.value<bool>());break;
        break;
    }
    case SingleLowPass: {
        catheter->setSingleLowPass(value.value<double>());break;
        break;
    }
    case SingleHeighPass: {
        catheter->setSingleHeighPass(value.value<double>());break;
        break;
    }
    case ShowPassData: {
        catheter->setShowPassData(value.value<bool>());break;
        break;
    }
    case APPARENT: {
        catheter->setApparent(value.value<bool>());break;
        break;
    }
    case ElectrodeLength: {
        catheter->setElectrodeLength(value.value<double>());break;
        break;
    }
    case Port: {
        catheter->setPort(value.value<qint32>());break;
        break;
    }
    case ACTIVE: {
        m_profile->reproduceOptions()->setCatheterId(catheter->id());
        m_profile->save();
        emit dataChanged(createIndex(0, 0), createIndex(m_catheterDb->size() - 1, columnCount() - 1), roles);
        return true;
    }
    default:
        return false;
    }
    m_catheterDb->save();
    emit dataChanged(createIndex(index.row(), 0), createIndex(index.row(), columnCount() - 1),  roles);
    return true;
}

QHash<int, QByteArray> CatheterTableModel::roleNames() const {
    return {
        {Name, "name"},
        {Amount, "amount"},
        {Manufacturer, "manufacturer"},
        {Type, "type"},
        {Version, "version"},
        {Gap, "gap"},
        {Fr, "fr"},
        {Diameter, "diameter"},
        {Far, "far"},
        {Electrode, "electrode"},
        {Employ, "employ"},
        {Dyestuff, "dyestuff"},
        {MultiPassEnable, "multiPassEnable"},
        {MultiLowPass, "multiLowPass"},
        {MultiHeighPass, "multiHeighPass"},
        {SinglePassEnable, "singlePassEnable"},
        {SingleLowPass, "singleLowPass"},
        {SingleHeighPass, "singleHeighPass"},
        {ShowPassData, "showPassData"},
        {Magnetism, "magnetism"},
        {MagnetismConsult, "magnetismConsult"},
        {MagnetismConsultDistance, "magnetismConsultDistance"},
        {MagnetismTarget, "magnetismTarget"},
        {MagnetismTargetDistance, "magnetismTargetDistance"},
        {APPARENT, "apparent"},
        {Builtin, "builtin"},
        {ElectrodeLength, "electrodeLength"},
        {Prefab, "prefab"},
        {ACTIVE, "active"},
        {MagnetismDescription, "magnetismDescription"},
        {Port, "port"},
        };
}

QVariantMap  CatheterTableModel::item(const QModelIndex &index) const
{
    Catheter *catheter = m_catheterDb->getData(index.row());
    QVariantMap  roles;
    if (catheter != nullptr) {
        for (auto [key, value]: roleNames().asKeyValueRange()) {
            QVariant variantData = data(catheter, key);
            if (variantData.isValid())
                roles.insert(value, variantData);
        }
    }
    return roles;

}

Catheter *CatheterTableModel::getData(int index) const
{
    return m_catheterDb->getData(index);
}

bool CatheterTableModel::removeRows(int row, int count, const QModelIndex& parent){
    if (row != -1) {
        beginRemoveRows(parent, row, row);
        Catheter* catheter = getData(row);
        if (catheter != nullptr) {
            m_catheterDb->remove(catheter);
            ReproduceOptions* reproduceOptions = m_profile->reproduceOptions();
            if (catheter->id() == reproduceOptions->catheterId()) {
                reproduceOptions->setCatheterId("");
            }
        }
        endRemoveRows();
    }
    return true;
}

void CatheterTableModel::onCatheterTypeNameChanged(CatheterType *type, const QString &name)
{
    resetCatheterName(name, type->name());
}

void CatheterTableModel::onCatheterTypeDeleted(const QString &name)
{
    resetCatheterName(name, "ABL");
}

void CatheterTableModel::resetCatheterName(const QString &srcName, const QString &newName)
{
    QList<Catheter *> list = m_catheterDb->getDatas();
    for(int i = 0; i < list.length(); ++i) {
        if (list[i]->getType() == srcName) {
            list[i]->setType(newName);
            emit dataChanged(createIndex(i, 0), createIndex(i, columnCount() - 1), QVector<int>() << Type);
        }
    }
}

Halve::ChannelMode CatheterTableModel::channeMode() const
{
    return m_channeMode;
}

void CatheterTableModel::setChanneMode(const Halve::ChannelMode &newMode)
{
    if (m_channeMode == newMode)
        return;
    m_channeMode = newMode;
    emit channeModeChanged();
}

QColor CatheterTableModel::ecgColor() const
{
    return m_ecgColor;
}

void CatheterTableModel::setEcgColor(const QColor &newEcgColor)
{
    if (m_ecgColor == newEcgColor)
        return;
    m_ecgColor = newEcgColor;
    m_catheterDb->getData(BudySurfaceCatheterID)->setDyestuff(m_ecgColor);
    emit ecgColorChanged();
}

void CatheterTableModel::onMixed(Catheter *catheter)
{
    qsizetype index = m_catheterDb->size() - 1;
    beginInsertRows(QModelIndex(), index, index);
    ReproduceOptions *reproduceOptions = m_profile->reproduceOptions();
    if (reproduceOptions->catheterId().isEmpty()) {
        switch(m_channeMode) {
        case Halve::ChannelMode::CHANNELMODE_MAGNETIC:
        case Halve::ChannelMode::CHANNELMODE_BLEND: {
            if (catheter->magnetism()) {
                reproduceOptions->setCatheterId(catheter->id());
                m_profile->save();
            }
            break;
        }
        case Halve::ChannelMode::CHANNELMODE_ELECTRICAL:
            reproduceOptions->setCatheterId(catheter->id());
            m_profile->save();
            break;
        }
    }
    endInsertRows();
}

Qt::ItemFlags CatheterTableModel::flags(const QModelIndex& index) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEditable;
}
