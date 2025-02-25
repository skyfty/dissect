#ifndef STAGEOPTIONS_H
#define STAGEOPTIONS_H

#include "HalveType.h"

class CameraInfo;

class StageOptions : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(float transparency READ transparency WRITE setTransparency NOTIFY transparencyChanged FINAL)
    Q_PROPERTY(Halve::Representation representation READ representation WRITE setRepresentation NOTIFY representationChanged FINAL)

    Q_PROPERTY(bool pointCloud READ pointCloud WRITE setPointCloud NOTIFY pointCloudChanged FINAL)
    Q_PROPERTY(bool showAxes READ showAxes WRITE setShowAxes NOTIFY showAxesChanged FINAL)
    Q_PROPERTY(bool showOrientation READ showOrientation WRITE setShowOrientation NOTIFY showOrientationChanged FINAL)
    Q_PROPERTY(bool scalarBar READ scalarBar WRITE setScalarBar NOTIFY scalarBarChanged FINAL)
    Q_PROPERTY(bool backfaceCulling READ backfaceCulling WRITE setBackfaceCulling NOTIFY backfaceCullingChanged FINAL)
    Q_PROPERTY(bool showPant READ showPant WRITE setShowPant NOTIFY showPantChanged FINAL)
    Q_PROPERTY(bool showInnerGridlines READ showInnerGridlines WRITE setShowInnerGridlines NOTIFY showInnerGridlinesChanged FINAL)
    Q_PROPERTY(bool useAxisOrigin READ useAxisOrigin WRITE setUseAxisOrigin NOTIFY useAxisOriginChanged FINAL)
    Q_PROPERTY(bool show3dPoints READ show3dPoints WRITE setShow3dPoints NOTIFY show3dPointsChanged FINAL)
    Q_PROPERTY(bool showSurfacePoints READ showSurfacePoints WRITE setShowSurfacePoints NOTIFY showSurfacePointsChanged FINAL)

    Q_PROPERTY(bool showCatheter READ showCatheter WRITE setShowCatheter NOTIFY showCatheterChanged FINAL)
    Q_PROPERTY(bool showMesh READ showMesh WRITE setShowMesh NOTIFY showMeshChanged FINAL)
    Q_PROPERTY(bool showCatheterLabel READ showCatheterLabel WRITE setShowCatheterLabel NOTIFY showCatheterLabelChanged FINAL)
    Q_PROPERTY(bool show3dAnnotaions READ show3dAnnotaions WRITE setShow3dAnnotaions NOTIFY show3dAnnotaionsChanged FINAL)
    Q_PROPERTY(bool show3dLabel READ show3dLabel WRITE setShow3dLabel NOTIFY show3dLabelChanged FINAL)
    Q_PROPERTY(bool showDissolvePoints READ showDissolvePoints WRITE setShowDissolvePoints NOTIFY showDissolvePointsChanged FINAL)

public:

    explicit StageOptions(QObject *parent = nullptr);
    void fromJson(const QJsonObject &v);
    QJsonValue toJson() const;

    QString name() const;
    void setName(const QString &newName);

    bool showAxes() const;
    void setShowAxes(bool newShowAxes);

    bool showOrientation() const;
    void setShowOrientation(bool newShowOrientation);

    bool scalarBar() const;
    void setScalarBar(bool newScalarBar);

    bool backfaceCulling() const;
    void setBackfaceCulling(bool newBackfaceCulling);

    float transparency() const;
    void setTransparency(float newTransparency);

    Halve::Representation representation() const;
    void setRepresentation(const Halve::Representation &newRepresentation);



    const CameraInfo* cameraInfo() const;
    void setCameraInfo(const CameraInfo &newCameraInfo);
    bool pointCloud() const;
    void setPointCloud(bool newPointCloud);

    bool showCatheter() const;
    void setShowCatheter(bool newShowCatheter);

    bool showMesh() const;
    void setShowMesh(bool newShowMesh);

    bool showPant() const;
    void setShowPant(bool newShowPant);

    bool showInnerGridlines() const;
    void setShowInnerGridlines(bool newShowInnerGridlines);

    bool useAxisOrigin() const;
    void setUseAxisOrigin(bool newUseAxisOrigin);

    bool showCatheterLabel() const;
    void setShowCatheterLabel(bool newShowCatheterLabel);

    bool show3dPoints() const;
    void setShow3dPoints(bool newShow3dPoints);

    bool showSurfacePoints() const;
    void setShowSurfacePoints(bool newShowSurfacePoints);

    bool show3dLabel() const;
    void setShow3dLabel(bool newShow3dLabel);

    bool show3dAnnotaions() const;
    void setShow3dAnnotaions(bool newShow3dAnnotaions);

    bool showDissolvePoints() const;
    void setShowDissolvePoints(bool newShowDissolvePoints);

signals:
    void changed();
    void nameChanged();

    void showAxesChanged();

    void showOrientationChanged();

    void show3dPointsChanged();

    void show3dAnnotaionsChanged();

    void show3dLabelChanged();

    void showSurfacePointsChanged();

    void scalarBarChanged();

    void backfaceCullingChanged();

    void transparencyChanged();


    void representationChanged();

    void pointCloudChanged();

    void showCatheterChanged();

    void showMeshChanged();

    void showPantChanged();

    void showInnerGridlinesChanged();

    void useAxisOriginChanged();

    void showCatheterLabelChanged();

    void showDissolvePointsChanged();

private:
    QString m_name;
    bool m_showAxes = false;
    bool m_showOrientation = true;
    bool m_scalarBar = true;
    bool m_backfaceCulling = true;
    bool m_pointCloud = false;
    bool m_showPant = true;
    bool m_showCatheter = true;
    bool m_showMesh = true;
    bool m_useAxisOrigin = false;
    float m_transparency = 1.0f;
    bool m_showInnerGridlines = false;
    bool m_showCatheterLabel = false;
    bool m_show3dPoints = false;
    bool m_show3dAnnotaions = false;
    bool m_show3dLabel = false;
    bool m_showSurfacePoints = true;
    bool m_showDissolvePoints = false;

    Halve::Representation m_representation = Halve::SURFACE;
    std::unique_ptr<CameraInfo> m_cameraInfo;
};

#endif // STAGEOPTIONS_H
