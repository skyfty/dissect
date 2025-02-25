#include "ProfileFile.h"
#include "mapping/MappingPointGroupDb.h"
#include "reseau/ReseauDb.h"
#include <QJsonDocument>
#include <qfile.h>
#include <qjsonobject.h>

ProfileFile::ProfileFile(const QString &id,const QString &path, QObject *parent)
    :Profile(path, parent)
{
    m_id = id;
    QFile f(getStoragePath(m_path));
    if (f.open(QIODevice::ReadOnly)) {
        fromJson(QJsonDocument::fromJson(f.readAll()).object());
    }
}

Halve::ProfileType ProfileFile::type() const
{
    return Halve::PROFILE_FILE;
}

void ProfileFile::save() {
    QFile saveFile(getStoragePath(m_path));
    if (saveFile.open(QIODevice::WriteOnly)) {
        QJsonValue json = toJson();
        saveFile.write(QJsonDocument(json.toObject()).toJson());
    }
    mappingPointGroupDb()->save();
    reseauDb()->save();
}
