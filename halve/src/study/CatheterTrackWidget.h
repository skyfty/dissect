#ifndef CATHETERTRACKWIDGET_H
#define CATHETERTRACKWIDGET_H

#include <QDateTime>
#include <QQuickVTKItem.h>
#include <vtkBoundingBox.h>
#include "utility/VtkDataWarp.h"


class Profile;
class Combined;
class CatheterTrackPackage;
class CatheterTrack;
class vtkVector3d;
class Catheter;
class CatheterDb;
class BreathOptions;
class BreathSurvey;
class vtkRenderWindow;
class vtkUnstructuredGrid;
class CatheterTrackWidgetData;
class FrameRate;
class CatheterTrackWorker;

class CatheterTrackWidget : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(Profile* profile WRITE setProfile  READ profile NOTIFY profileChanged FINAL);
    Q_PROPERTY(Combined* combined WRITE setCombined READ combined NOTIFY combinedChanged FINAL);
    Q_PROPERTY(double consultPrecision READ consultPrecision WRITE setConsultPrecision NOTIFY consultPrecisionChanged FINAL)
    Q_PROPERTY(BreathSurvey *breathSurvey WRITE setBreathSurvey FINAL)

public:
    explicit CatheterTrackWidget(QQuickItem *parent = nullptr);
    ~CatheterTrackWidget();
    Q_INVOKABLE void init();

    void setProfile(Profile* profile);
    Profile* profile() const;
    void setCombined(Combined *combined);
    Combined* combined() const;
    void setBreathSurvey(BreathSurvey *newBreathSurvey);

    double consultPrecision() const;
    void setConsultPrecision(double newConsultPrecision);


public slots:
    void onCatheterTrackChanged(const QSharedPointer<CatheterTrackPackage> &trackData);
    void onCatheterDyestuffChanged(Catheter *catheter);
    void onCatheterEmployChanged(Catheter *catheter);
    void onCatheterAlined(Catheter *catheter);
    void onCarpenterResult(Catheter *catheter, UnstructuredGridWarp::Ptr grid, UnstructuredGridWarp::Ptr polyData);
    void onCenterPointChanged();


signals:
    void profileChanged();
    void combinedChanged();
    void pant0Changed();
    void consultPrecisionChanged();
    void initRender();

private:
    bool getTrackPosition(const CatheterTrack &track, vtkVector3d &position);
    void checkCatheterTrack(Catheter* catheter, const QList<CatheterTrack> &trackDatas);
    void checkPantCatheterTrack(Catheter* catheter, const QList<CatheterTrack> &trackDatas);
    void refreshCatheterTube(Catheter* catheter, vtkSmartPointer<vtkUnstructuredGrid> grid);
    void checkPant0Position(const QSharedPointer<CatheterTrackPackage> &trackDat);
    vtkUnstructuredGrid* prepareCatheterMesh(Catheter* catheter);
    void checkCatheterTracks(const QList<Catheter*> &catheters, const QSharedPointer<CatheterTrackPackage>& trackDataPackage);

private:
    QPointer<Profile> m_profile;
    QPointer<CatheterDb> m_catheterDb;
    QPointer<Combined> m_combined;
    QPointer<BreathOptions> m_breathOptions;
    QPointer<BreathSurvey> m_breathSurvey;
    QPointer<CatheterTrackWorker> m_worker;
    bool m_pantElectricalNeedInit = true;

    vtkBoundingBox m_pant0BoundingBox{0,-1,0,-1,0,-1};
    double m_consultPrecision = 5.0;
};

#endif // CATHETERTRACKWIDGET_H
