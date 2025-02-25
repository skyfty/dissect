#include "StageOptions.h"

#include "CameraInfo.h"

#include <QIODevice>
#include <QJsonObject>

StageOptions::StageOptions(QObject *parent)
    : QObject{parent}
{}

void StageOptions::fromJson(const QJsonObject &json) {
    if (json.contains("name"))
        m_name = json["name"].toString();
    if (json.contains("showAxes"))
        m_showAxes = json["showAxes"].toBool();
    if (json.contains("showOrientation"))
        m_showOrientation = json["showOrientation"].toBool();
    if (json.contains("scalarBar"))
        m_scalarBar = json["scalarBar"].toBool();
    if (json.contains("backfaceCulling"))
        m_backfaceCulling = json["backfaceCulling"].toBool();
    if (json.contains("pointCloud"))
        m_pointCloud = json["pointCloud"].toBool();
    if (json.contains("showPant"))
        m_showPant = json["showPant"].toBool();
    if (json.contains("showCatheter"))
        m_showCatheter = json["showCatheter"].toBool();
    if (json.contains("showMesh"))
        m_showMesh = json["showMesh"].toBool();
    if (json.contains("useAxisOrigin"))
        m_useAxisOrigin = json["useAxisOrigin"].toBool();
    if (json.contains("showInnerGridlines"))
        m_showInnerGridlines = json["showInnerGridlines"].toBool();
    if (json.contains("showCatheterLabel"))
        m_showCatheterLabel = json["showCatheterLabel"].toBool();
    if (json.contains("representation"))
        m_representation = ( Halve::Representation)json["representation"].toInt();
    if (json.contains("transparency"))
        m_transparency = json["transparency"].toDouble();
    if (json.contains("cameraInfo")) {
        QByteArray byteArray = json["cameraInfo"].toString().toLatin1();
        m_cameraInfo.reset(new CameraInfo());
        QDataStream stream(&byteArray, QIODevice::ReadOnly);
        stream >> *m_cameraInfo;
    }
    if (json.contains("show3dPoints"))
        m_show3dPoints = json["show3dPoints"].toBool();
    if (json.contains("show3dAnnotaions"))
        m_show3dAnnotaions = json["show3dAnnotaions"].toBool();
    if (json.contains("show3dLabel"))
        m_show3dLabel = json["show3dLabel"].toBool();
    if (json.contains("showSurfacePoints"))
        m_showSurfacePoints = json["showSurfacePoints"].toBool();
    if (json.contains("showDissolvePoints"))
        m_showDissolvePoints = json["showDissolvePoints"].toBool();

}


QJsonValue StageOptions::toJson() const {
    QJsonObject json;
    json["name"] = m_name;
    json["showAxes"] = m_showAxes;
    json["showOrientation"] = m_showOrientation;
    json["scalarBar"] = m_scalarBar;
    json["backfaceCulling"] = m_backfaceCulling;
    json["representation"] = m_representation;
    json["transparency"] = m_transparency;
    json["pointCloud"] = m_pointCloud;
    json["showPant"] = m_showPant;
    json["showCatheter"] = m_showCatheter;
    json["useAxisOrigin"] = m_useAxisOrigin;
    json["showCatheterLabel"] = m_showCatheterLabel;
    json["showMesh"] = m_showMesh;
    json["showInnerGridlines"] = m_showInnerGridlines;
    json["show3dPoints"] = m_show3dPoints;
    json["show3dAnnotaions"] = m_show3dAnnotaions;
    json["show3dLabel"] = m_show3dLabel;
    json["showSurfacePoints"] = m_showSurfacePoints;
    json["showDissolvePoints"] = m_showDissolvePoints;

    if (m_cameraInfo != nullptr) {
        QByteArray byteArray;
        QDataStream stream(&byteArray, QIODevice::WriteOnly);
        stream << *m_cameraInfo;
        json["cameraInfo"] = QString::fromLatin1(byteArray);
    }
    return json;
}

QString StageOptions::name() const
{
    return m_name;
}

void StageOptions::setName(const QString &newName)
{
    if (m_name == newName)
        return;
    m_name = newName;
    emit nameChanged();
    emit changed();
}

bool StageOptions::showAxes() const
{
    return m_showAxes;
}

void StageOptions::setShowAxes(bool newShowAxes)
{
    if (m_showAxes == newShowAxes)
        return;
    m_showAxes = newShowAxes;
    emit showAxesChanged();
    emit changed();
}

bool StageOptions::showOrientation() const
{
    return m_showOrientation;
}

void StageOptions::setShowOrientation(bool newShowOrientation)
{
    if (m_showOrientation == newShowOrientation)
        return;
    m_showOrientation = newShowOrientation;
    emit showOrientationChanged();
    emit changed();
}

bool StageOptions::scalarBar() const
{
    return m_scalarBar;
}

void StageOptions::setScalarBar(bool newScalarBar)
{
    if (m_scalarBar == newScalarBar)
        return;
    m_scalarBar = newScalarBar;
    emit scalarBarChanged();
    emit changed();
}

bool StageOptions::backfaceCulling() const
{
    return m_backfaceCulling;
}

void StageOptions::setBackfaceCulling(bool newBackfaceCulling)
{
    if (m_backfaceCulling == newBackfaceCulling)
        return;
    m_backfaceCulling = newBackfaceCulling;
    emit backfaceCullingChanged();
    emit changed();
}

float StageOptions::transparency() const
{
    return m_transparency;
}

void StageOptions::setTransparency(float newTransparency)
{
    if (qFuzzyCompare(m_transparency, newTransparency))
        return;
    m_transparency = newTransparency;
    emit transparencyChanged();
    emit changed();
}


Halve::Representation StageOptions::representation() const
{
    return m_representation;
}

void StageOptions::setRepresentation(const Halve::Representation &newRepresentation)
{
    if (m_representation == newRepresentation)
        return;
    m_representation = newRepresentation;
    emit representationChanged();
    emit changed();
}

const CameraInfo* StageOptions::cameraInfo() const
{
    return m_cameraInfo.get();
}

void StageOptions::setCameraInfo(const CameraInfo &newCameraInfo)
{
    m_cameraInfo.reset(new CameraInfo(newCameraInfo));
}

bool StageOptions::pointCloud() const
{
    return m_pointCloud;
}

void StageOptions::setPointCloud(bool newPointCloud)
{
    if (m_pointCloud == newPointCloud)
        return;
    m_pointCloud = newPointCloud;
    emit pointCloudChanged();
}

bool StageOptions::showCatheter() const
{
    return m_showCatheter;
}

void StageOptions::setShowCatheter(bool newShowCatheter)
{
    if (m_showCatheter == newShowCatheter)
        return;
    m_showCatheter = newShowCatheter;
    emit showCatheterChanged();
    emit changed();
}

bool StageOptions::showMesh() const
{
    return m_showMesh;
}

void StageOptions::setShowMesh(bool newShowMesh)
{
    if (m_showMesh == newShowMesh)
        return;
    m_showMesh = newShowMesh;
    emit showMeshChanged();
    emit changed();
}

bool StageOptions::showPant() const
{
    return m_showPant;
}

void StageOptions::setShowPant(bool newShowPant)
{
    if (m_showPant == newShowPant)
        return;
    m_showPant = newShowPant;
    emit showPantChanged();
    emit changed();
}

bool StageOptions::showInnerGridlines() const
{
    return m_showInnerGridlines;
}

void StageOptions::setShowInnerGridlines(bool newShowInnerGridlines)
{
    if (m_showInnerGridlines == newShowInnerGridlines)
        return;
    m_showInnerGridlines = newShowInnerGridlines;
    emit showInnerGridlinesChanged();
    emit changed();
}

bool StageOptions::useAxisOrigin() const
{
    return m_useAxisOrigin;
}

void StageOptions::setUseAxisOrigin(bool newUseAxisOrigin)
{
    if (m_useAxisOrigin == newUseAxisOrigin)
        return;
    m_useAxisOrigin = newUseAxisOrigin;
    emit useAxisOriginChanged();
    emit changed();
}

bool StageOptions::showCatheterLabel() const
{
    return m_showCatheterLabel;
}

void StageOptions::setShowCatheterLabel(bool newShowCatheterLabel)
{
    if (m_showCatheterLabel == newShowCatheterLabel)
        return;
    m_showCatheterLabel = newShowCatheterLabel;
    emit showCatheterLabelChanged();
    emit changed();
}


bool StageOptions::show3dPoints() const
{
    return m_show3dPoints;
}

void StageOptions::setShow3dPoints(bool newShow3dPoints)
{
    if (m_show3dPoints == newShow3dPoints)
        return;
    m_show3dPoints = newShow3dPoints;
    emit show3dPointsChanged();
    emit changed();
}

bool StageOptions::show3dAnnotaions() const
{
    return m_show3dAnnotaions;
}

void StageOptions::setShow3dAnnotaions(bool newShow3dAnnotaions)
{
    if (m_show3dAnnotaions == newShow3dAnnotaions)
        return;
    m_show3dAnnotaions = newShow3dAnnotaions;
    emit show3dAnnotaionsChanged();
    emit changed();
}

bool StageOptions::showDissolvePoints() const
{
    return m_showDissolvePoints;
}

void StageOptions::setShowDissolvePoints(bool newShowDissolvePoints)
{
    if (m_showDissolvePoints == newShowDissolvePoints)
        return;
    m_showDissolvePoints = newShowDissolvePoints;
    emit showDissolvePointsChanged();
    emit changed();
}

bool StageOptions::show3dLabel() const
{
    return m_show3dLabel;
}

void StageOptions::setShow3dLabel(bool newShow3dLabel)
{
    if (m_show3dLabel == newShow3dLabel)
        return;
    m_show3dLabel = newShow3dLabel;
    emit show3dLabelChanged();
    emit changed();
}

bool StageOptions::showSurfacePoints() const
{
    return m_showSurfacePoints;
}

void StageOptions::setShowSurfacePoints(bool newShowSurfacePoints)
{
    if (m_showSurfacePoints == newShowSurfacePoints)
        return;
    m_showSurfacePoints = newShowSurfacePoints;
    emit showSurfacePointsChanged();
    emit changed();
}
