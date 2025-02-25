#include "MappingPointGroupDb.h"
#include "qjsonarray.h"
#include "qjsonobject.h"
#include "MappingPointGroup.h"

#include <QDateTime>
#include <QFile>
#include <QJsonDocument>
#include <QResource>

MappingPointGroupDb::MappingPointGroupDb(const QString &path,QObject *parent)
    : QObject(parent)
{
    m_filePath = getStoragePath(path);
    if (QFile::exists(m_filePath)) {
        load();
    } else {
        initDefault();
    }
}

void MappingPointGroupDb::connectSignals(MappingPointGroup* group) {
    QObject::connect(group, &MappingPointGroup::nameChanged, this, std::bind(&MappingPointGroupDb::changed, this, group));
}

QString MappingPointGroupDb::getStoragePath(const QString &path) {
    return path + "/mapping_point_group.json";
}

void MappingPointGroupDb::initDefault() {
    load(QResource(":/assets/configures/mapping_point_group.json").uncompressedData());
}

void MappingPointGroupDb::load(const QByteArray &jsonContent) {
    for(auto json:QJsonDocument::fromJson(jsonContent.data()).array()) {
        MappingPointGroup *group = new MappingPointGroup(this);
        group->fromJson(json.toObject());
        connectSignals(group);
        m_list.append(group);
    }
}

void MappingPointGroupDb::load() {
    QFile f(m_filePath);
    if (f.open(QIODevice::ReadOnly)) {
        load(f.readAll());
    }
}

bool MappingPointGroupDb::save() {
    QFile saveFile(m_filePath);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        return false;
    }
    QJsonArray surfaceSortJson;
    for(const MappingPointGroup* group:m_list) {
        surfaceSortJson.append( group->toJson());
    }
    return saveFile.write(QJsonDocument(surfaceSortJson).toJson());
}

void MappingPointGroupDb::remove(int row) {
    if (row >= 0 && row < m_list.size()) {
       MappingPointGroup *group = m_list[row];
        m_list.removeAt(row);
        emit deleted(group);
        save();
    }
}

MappingPointGroup * MappingPointGroupDb::add(const QString &id, const QString &name) {
    MappingPointGroup *group = new MappingPointGroup(this);
    group->setName(name);
    group->setId(id);
    m_list.append(group);
    connectSignals(group);
    save();
    emit added(group);
    return group;
}

MappingPointGroup* MappingPointGroupDb::getData(int row) {
    if (row < 0 || row >= m_list.size()) {
        return nullptr;
    }
    return m_list[row];
}

QList<MappingPointGroup *> MappingPointGroupDb::getData()
{
    return m_list;
}

MappingPointGroup *MappingPointGroupDb::getData(const QString &id)
{
    for(MappingPointGroup *group: m_list) {
        if (group->id() == id) {
            return group;
        }
    }
    return nullptr;
}

int MappingPointGroupDb::indexOfName(const QString &name) const
{
    for(int i = 0; i < m_list.size(); ++i) {
        if (m_list[i]->name() == name) {
            return i;
        }
    }
    return -1;
}


qsizetype MappingPointGroupDb::size() const
{
    return m_list.size();
}

void MappingPointGroupDb::backup(const QString &path)
{
    QString backupFile = getStoragePath(path);
    QFile::copy(m_filePath, backupFile);
}
