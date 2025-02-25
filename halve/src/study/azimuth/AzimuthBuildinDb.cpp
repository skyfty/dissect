#include "AzimuthBuildinDb.h"

#include <QJsonArray>
#include "Azimuth.h"
#include <QJsonObject>

#include <QFile>
#include <QJsonDocument>
#include <QResource>
#include <QVector3D>
#include <vtkMath.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>

#include <study/CameraInfo.h>

AzimuthBuildinDb::AzimuthBuildinDb(QObject *parent)
    : QObject{parent}
{}

void AzimuthBuildinDb::load(const QByteArray &azimuthJson) {
    auto azimuths = QJsonDocument::fromJson(azimuthJson.data()).array();
    for(auto json:azimuths) {
        Azimuth* azimuth = load(json.toObject());
        m_azimuths.append(azimuth);
        if (azimuth->id() >= 0 && azimuth->id() < 6) {
            m_buildinAzimuths[azimuth->id()] = azimuth;
        }
    }
}

Azimuth* AzimuthBuildinDb::load(const QJsonObject &json) {
    Azimuth* azimuth = new Azimuth(this);
    connectSignals(azimuth);
    azimuth->fromJson(json);
    return azimuth;
}

void AzimuthBuildinDb::initDefault() {
    load(QResource(":/assets/configures/azimuth.json").uncompressedData());
}


void AzimuthBuildinDb::connectSignals(Azimuth *azimuth) {
    QObject::connect(azimuth, &Azimuth::nameChanged, this, std::bind(&AzimuthBuildinDb::changed, this, azimuth));
}

qsizetype AzimuthBuildinDb::size() const
{
    return m_azimuths.size();
}

Azimuth *AzimuthBuildinDb::getData(int row) const {
    if (row < 0 || row > m_azimuths.length() -1) {
        return nullptr;
    }
    return m_azimuths[row];
}

Azimuth *AzimuthBuildinDb::getData(double x, double y, double z) const
{
    for(auto azimuth:m_azimuths) {
        const double* pos = azimuth->position();
        if (qFuzzyCompare(pos[0], x) && qFuzzyCompare(pos[1], y) && qFuzzyCompare(pos[2], z)) {
            return azimuth;
        }
    }
    return nullptr;
}

Azimuth *AzimuthBuildinDb::getData(const QVector3D &pos) const
{
    return getData(pos.x(), pos.y(), pos.z());
}

Azimuth *AzimuthBuildinDb::getData(const double pos[]) const
{
    return getData(pos[0], pos[1], pos[2]);

}

Azimuth *AzimuthBuildinDb::getData(const QString &name) const
{
    for(auto azimuth:m_azimuths) {
        if (azimuth->name() == name) {
            return azimuth;
        }
    }
    return nullptr;
}
Azimuth* AzimuthBuildinDb::getData(const vtkVector3d &pos) const {
    return getData(pos.GetData());
}

Azimuth* AzimuthBuildinDb::getData(qint64 id) const {
    for (auto azimuth : m_azimuths) {
        if (azimuth->id() == id) {
            return azimuth;
        }
    }
    return nullptr;
}

