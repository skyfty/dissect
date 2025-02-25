#ifndef ORIENTATION_H
#define ORIENTATION_H

#include "CameraInfo.h"

#include <QObject>
#include <QQuickVTKItem.h>


class vtkRenderWindow;
class vtkNamedColors;
class CameraInfo;
class vtkCamera;
class Profile;
class AzimuthDb;
class vtkMatrix4x4;

struct OrientationData;

class Orientation : public QQuickVTKItem
{
    Q_OBJECT
    Q_PROPERTY(Profile* profile WRITE setProfile READ profile NOTIFY profileChanged);
    Q_PROPERTY(CameraInfo cameraInfo  WRITE setCameraInfo  FINAL)

public:
    explicit Orientation(QQuickItem *parent = nullptr);

    void setProfile(Profile* profile);
    Profile *profile() const;
    void setCameraInfo(const CameraInfo &cameraInfo);
    Q_INVOKABLE void resetAP();

signals:
    void profileChanged();
public slots:
    void onOrientaionMatrixChanged();
private:
    vtkUserData initializeVTK(vtkRenderWindow* renderWindow) override;
    void setCameraInfo(OrientationData *userData, const CameraInfo &cameraInfo);
    void createFollowerActor(OrientationData* userData);
    void setActorMatrix(OrientationData *userData, vtkMatrix4x4* matrix);
    void resetAzimuthData(vtkMatrix4x4* orientaionMatrix);

private:
    QPointer<Profile> m_profile;
    QPointer<AzimuthDb> m_azimuthDb;
    CameraInfo m_cameraInfo;
};

#endif // ORIENTATION_H
