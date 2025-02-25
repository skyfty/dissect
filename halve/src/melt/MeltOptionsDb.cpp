#include "MeltOptionsDb.h"
#include "melt/MeltOptions.h"
#include "qjsonarray.h"
#include "qjsonobject.h"

#include <QFile>
#include <QJsonDocument>
#include <QResource>

#include <catheter/CatheterDb.h>

MeltOptionsDb::MeltOptionsDb(const QString &path,CatheterDb* catheterDb,QObject *parent)
    : QObject(parent),m_catheterDb(catheterDb)
{
    m_filePath = getStoragePath(path);
    if (QFile::exists(m_filePath)) {
        load();
    } else {
        initDefault();
    }
}

QString MeltOptionsDb::getStoragePath(const QString &path)
{
    return path + "/melt_options.json";
}

void MeltOptionsDb::initDefault() {
    load(QResource(":/assets/configures/melt_options.json").uncompressedData());
}

void MeltOptionsDb::load(const QByteArray &jsonContent) {
    for(auto json:QJsonDocument::fromJson(jsonContent.data()).array()) {
        MeltOptions *options = new MeltOptions(m_catheterDb, this);
        options->fromJson(json.toObject());
        m_list[options->id()] = options;
    }
}

void MeltOptionsDb::load() {
    QFile f(m_filePath);
    if (f.open(QIODevice::ReadOnly)) {
        load(f.readAll());
    }
}


bool MeltOptionsDb::save() {
    QFile saveFile(m_filePath);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        return false;
    }
    QJsonArray surfaceSortJson;
    for(MeltOptions* config:m_list) {
        surfaceSortJson.append(config->toJson());
    }
    return saveFile.write(QJsonDocument(surfaceSortJson).toJson());
}

MeltOptions* MeltOptionsDb::add(qint64 id, const QJsonObject& v){
    MeltOptions *options = new MeltOptions(m_catheterDb, this);
    options->fromJson(v);
    options->setId(id);
    m_list[id] = options;
    emit added();
    return options;
}

MeltOptions* MeltOptionsDb::getData(const qint64 &id) {
    return m_list.contains(id)? m_list[id]: nullptr;
}


qsizetype MeltOptionsDb::size() const
{
    return m_list.size();
}

void MeltOptionsDb::backup(const QString &path)
{
    QString backupFile = getStoragePath(path);
    QFile::copy(m_filePath, backupFile);
}
