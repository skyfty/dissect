#include "Azimuth.h"
#include "qjsonarray.h"
#include "AzimuthDb.h"
#include <qjsonobject.h>

#include <QFile>
#include <QJsonDocument>
#include <QResource>
#include <QVector3D>
#include <vtkMath.h>
#include <vtkSmartPointer.h>
#include <study/CameraInfo.h>

AzimuthDb::AzimuthDb(const QString& path, QObject* parent)
    : AzimuthBuildinDb{ parent } {
    Q_ASSERT(!path.isEmpty());
    m_path = getStoragePath(path);
    if (QFile::exists(m_path)) {
        load();
    } else {
        initDefault();
    }
}

bool AzimuthDb::save() {
    QFile saveFile(m_path);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        return false;
    }
    QJsonArray azimuthJson;
    for(auto azimuth:m_azimuths) {
        QJsonObject json;
        azimuth->toJson(json);
        azimuthJson.append(json);
    }
    return saveFile.write(QJsonDocument(azimuthJson).toJson());
}
void AzimuthDb::load() {
    QFile f(m_path);
    if (f.open(QIODevice::ReadOnly)) {
        load(f.readAll());
    }
}
QString AzimuthDb::getStoragePath(const QString &path) const
{
    return path + "/azimuth.json";
}

void AzimuthDb::backup(const QString &path)
{
    QString backupFile = getStoragePath(path);
    QFile::copy(m_path, backupFile);
}

void AzimuthDb::remove(Azimuth* azimuth) {
    Q_ASSERT(azimuth !=nullptr);
    int idx = m_azimuths.indexOf(azimuth);
    if (idx != -1) {
        m_azimuths.removeAt(idx);
        save();
        azimuth->deleteLater();
        emit deleted(azimuth);
    }
}

void AzimuthDb::remove(int row) {
    if (row < m_azimuths.size()) {
        remove(m_azimuths[row]);
    }
}


void AzimuthDb::add(const QString &name, const CameraInfo &pos)
{
    Azimuth* azimuth = new Azimuth(name, pos, this);
    azimuth->setId(QDateTime::currentMSecsSinceEpoch());
    connectSignals(azimuth);
    m_azimuths.insert(0, azimuth);
    save();
    emit added(azimuth);
}
