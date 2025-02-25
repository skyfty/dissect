#ifndef STUDY_H
#define STUDY_H

#include <QDateTime>
#include <QQuickItem>
#include <vtkSmartPointer.h>
#include "utility/VtkDataWarp.h"

class vtkPoints;
class Combined;
class Profile;
class CatheterDb;
class Catheter;
class vtkQuaterniond;
class MappingPointsDb;
class MappingPoint;
class Mapping;
class DissolveDb;
class Obscurity;
class ScalarDb;
class StageOptions;
class ReseauDb;
class QColor;
class AlbumDb;
class ReproduceOptions;
class DissolveOptions;
class Channel;
class MappingPointGroup;
class BreathOptions;
class BreathSurvey;
class CatheterTrackPackage;
class CatheterTrack;
class vtkVector3d;
class AlarmDb;
class Alarm;
class Reseau;

class Study : public QQuickItem
{
    Q_OBJECT;
    Q_PROPERTY(Profile* profile WRITE setProfile  READ profile NOTIFY profileChanged FINAL);
    Q_PROPERTY(Combined* combined WRITE setCombined READ combined NOTIFY combinedChanged FINAL);
    Q_PROPERTY(Obscurity *obscurity WRITE setObscurity  FINAL)
    Q_PROPERTY(Channel *channel WRITE setChannel READ channel  NOTIFY channelChanged FINAL)

public:
    explicit Study(QQuickItem* parent = nullptr);
    ~Study();
    void setProfile(Profile* profile);
    Profile* profile() const;
    void setCombined(Combined *combined);
    Combined* combined() const;

    void setObscurity(Obscurity *newObscurity);

    void setChannel(Channel *newChannel);
    Channel* channel() const;

public slots:
    void onAlined();
    void onCenterPointChanged();
    void onPolyDataChanged(PolyDataWarp::Ptr result);
    void onChannelModeChanged();
    void onCombinedEnvironmentFlgasChanged();
    void onAlarmDeleted(const Alarm *alarm);
    void onProfileStateChanged();
    void onCatheterPortChanged(Catheter *catheter);
    void onCatheterDeleted(Catheter *catheter);
    void onReproduceCatheterChanged();
    void onSavePolyDataTimerEvent();

signals:
    void channelChanged();
    void combinedChanged();
    void profileChanged();

private:
    QPointer<Profile> m_profile;
    QPointer<Channel> m_channel;
    QPointer<CatheterDb> m_catheterDb;
    QPointer<Obscurity> m_obscurity;
    QPointer<Combined> m_combined;
    QPointer<ReseauDb> m_reseauDb;
    QTimer* m_savePolyDataTimer = nullptr;
};

#endif // STUDY_H
