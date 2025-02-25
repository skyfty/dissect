#ifndef DISSOLVEDATASOURCE_H
#define DISSOLVEDATASOURCE_H

#include <QObject>
#include <QPointer>
#include <QMap>
#include "combined/CatheterTrack.h"

class Profile;
class DissolveOptions;
class Combined;
class DissolveDb;
class CatheterDb;
class Catheter;
class ElectrodeNode;
class BreathOptions;
class BreathSurvey;
class ReproduceOptions;
class CatheterTrackPackage;

class DissolveDataSource : public QObject {
    Q_OBJECT
    Q_PROPERTY(Profile *profile WRITE setProfile READ profile NOTIFY profileChanged);
    Q_PROPERTY(Combined *combined WRITE setCombined FINAL);
    Q_PROPERTY(Catheter *currentCatheter READ getCatheter  NOTIFY currentCatheterChanged CONSTANT FINAL);
    Q_PROPERTY(BreathSurvey *breathSurvey WRITE setBreathSurvey FINAL)

public:
    explicit DissolveDataSource(QObject *parent = nullptr);

    void setProfile(Profile* profile);
    Profile *profile() const;
    Catheter *getCatheter() const;
    void setCombined(Combined *combined);
    Q_INVOKABLE void strik();
    void setBreathSurvey(BreathSurvey *newBreathSurvey);

private slots:
    void onCenterPointChanged();
    void onCatheterTrackChanged(const QSharedPointer<CatheterTrackPackage> &trackData);
signals:
    void profileChanged();
    void currentCatheterChanged();

private:
    std::optional<CatheterTrack> getElectrodeTrack(Catheter *catheter) const;
    void getTrackPosition(const CatheterTrack &track, vtkVector3d &position);

private:
    QPointer<Profile> m_profile;
    QPointer<DissolveOptions> m_dissolveOptions;
    QPointer<ReproduceOptions> m_reproduceOptions;
    QPointer<Combined> m_combined;
    QSharedPointer<CatheterTrackPackage> m_catheterTrack;
    QPointer<DissolveDb> m_dissolveDb;
    QList<qint64> m_dissolveIds;
    QPointer<CatheterDb> m_catheterDb;
    QPointer<BreathOptions> m_breathOptions;
    QPointer<BreathSurvey> m_breathSurvey;
};

#endif // DISSOLVEDATASOURCE_H
