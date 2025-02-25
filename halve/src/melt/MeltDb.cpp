#include "MeltDb.h"
#include "melt/Melt.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <profile/Profile.h>
#include <profile/ProfileManager.h>

MeltDb::MeltDb(const QString& path, QObject* parent)
    : QObject{ parent } {
    Q_ASSERT(!path.isEmpty());
    m_path = getStoragePath(path);
    load();
}

QString MeltDb::getStoragePath(const QString &path)
{
    return path + "/melts.json";
}

void MeltDb::fromJson(const QJsonArray &meltJson) {
    for(auto json:meltJson) {
        Melt* melt = new Melt(this);
        melt->fromJson(json.toObject());
        m_melts.push_front(melt);
    }
}

QJsonArray MeltDb::toJson() const {
    QJsonArray meltJson;
    for(const Melt* melt:m_melts) {
        meltJson.append(melt->toJson());
    }
    return meltJson;
}

void MeltDb::load() {
    QFile f(m_path);
    if (f.open(QIODevice::ReadOnly)) {
        QJsonArray meltJson = QJsonDocument::fromJson(f.readAll()).array();
        fromJson(meltJson);
    }
}

bool MeltDb::save() {
    QFile saveFile(m_path);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        return false;
    }
    QJsonArray meltJson = toJson();
    return saveFile.write(QJsonDocument(meltJson).toJson());
}

qsizetype MeltDb::size() const
{
    return m_melts.size();
}

Melt *MeltDb::getData(int row)
{
    if (row > m_melts.size()) {
        return nullptr;
    }
    return m_melts[row];
}

void MeltDb::remove(Melt* melt) {
    Q_ASSERT(melt !=nullptr);
    auto iter = std::find(std::begin(m_melts), std::end(m_melts), melt);
    if (iter != std::end(m_melts)) {
        m_melts.erase(iter);
        save();
        melt->deleteLater();
        emit deleted(melt);
    }
}

void MeltDb::remove(int row) {
    if (row < m_melts.size()) {
        remove(m_melts[row]);
    }
}

void MeltDb::add(const QColor &color, const QString &description) {
    Profile *profile = ProfileManager::instance()->getCurrentProfile();
    Melt *melt = new Melt(this);
    melt->setDyestuff(color);
    melt->setDescription(description);
    melt->setOrder(profile->getIncreaseMeltOrder());
    m_melts.append(melt);
    save();
    emit added(melt);
}

void MeltDb::backup(const QString &path)
{
    QString backupFile = getStoragePath(path);
    QFile::copy(m_path, backupFile);
}
