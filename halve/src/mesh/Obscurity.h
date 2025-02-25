#pragma once

#include <vtkVector.h>
#include <QStack>
#include <QPointer>
#include <QFuture>
#include <combined/CatheterTrack.h>
#include "utility/VtkDataWarp.h"

class vtkKdTreePointLocator;
class vtkImageData;
class vtkPolyData;
class Combined;
class Profile;
class BlackboardDb;
class vtkIdList;
class vtkPlanes;
class ObscurityWorker;
class BreathSurvey;
class BreathOptions;
class CatheterDb;
class vtkDataSet;
class QPolygon;
class ReseauDb;
class ReproduceOptions;
class Reseau;
class Catheter;
class CatheterTrackPackage;

class Obscurity : public QObject
{
    Q_OBJECT;
    Q_PROPERTY(Combined* combined WRITE setCombined FINAL);
    Q_PROPERTY(Profile* profile WRITE setProfile READ profile NOTIFY profileChanged);
    Q_PROPERTY(bool state READ state WRITE setState NOTIFY stateChanged FINAL)
    Q_PROPERTY(BreathSurvey *breathSurvey WRITE setBreathSurvey FINAL)
    Q_PROPERTY(quint32 abradeCount READ abradeCount NOTIFY abradeCountChanged FINAL)

public:

    explicit Obscurity(QObject* parent = nullptr);
    ~Obscurity();

    Q_INVOKABLE void init();
    Q_INVOKABLE void refresh();
    void setCombined(Combined* combined);
    void setProfile(Profile *newProfile);
    Profile *profile() const;

    vtkSmartPointer<vtkPolyData> extract(vtkSmartPointer<vtkIdList> ids);
    QFuture<ImageDataWarp::Ptr> refine();
    void resetReseau(Reseau* reseau);

    void wound(QPolygon polygon,const vtkSmartPointer<vtkMatrix4x4>& mat, const vtkVector2i &windowSize,const vtkVector4d &viewport);
    void wound(const vtkSmartPointer<vtkIdList>& ids);
    void test();
    Q_INVOKABLE void unwound();
    bool state() const;
    void setState(bool newState);

    void setBreathSurvey(BreathSurvey *newBreathSurvey);

    quint32 abradeCount() const;

signals:
    void profileChanged();
    void changed(ImageDataWarp::Ptr result);
    void polyDataChanged(PolyDataWarp::Ptr result);
    void stateChanged();
    void abradeCountChanged();


public slots:
    void onCatheterTrackChanged(const QSharedPointer<CatheterTrackPackage> &trackData);
    void onCarpenterResult(PolyDataWarp::Ptr polyData);
    void onProfileStateChanged();
    void onCenterPointChanged();
    void onSaveCurrentReseauTimerEvent();
    void onCurrentReseauChanged();

private:
    void checkCatheterTrackData(Catheter *catheter, const QList<CatheterTrack> &trackDatas, vtkIdList *idTotalList);
    void checkElectrodeTrackPosition(Catheter *catheter,const QString &electrodeId,const vtkVector3d &trackPosition, vtkIdList *idTotalList);
    void depictBlackboardPoint(vtkIdList *idTotalList);
    void pushWoundStack(vtkSmartPointer<vtkIdList> ids);
    void saveCurrentReseauPoints(std::chrono::milliseconds value);

private:
    QPointer<Profile> m_profile;
    QPointer<BlackboardDb> m_blackboardDb;
    QPointer<Combined> m_combined;
    QPointer<ReseauDb> m_reseauDb;
    QPointer<BreathOptions> m_breathOptions;
    QPointer<ReproduceOptions> m_reproduceOptions;
    QPointer<BreathSurvey> m_breathSurvey;
    bool m_state  = false;
    QPointer<ObscurityWorker> m_worker;
    QStack<vtkSmartPointer<vtkIdList>> m_woundStack;
    vtkVector3d m_centerPoint{-1,-1,-1};
    QTimer *m_saveTimer = nullptr;
};
