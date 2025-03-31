#ifndef REACTWORKER_H
#define REACTWORKER_H

#include <QObject>
#include <QFuture>
#include <QPointer>
#include <QPromise>
#include <vtkIdList.h>

#include <combined/CatheterTrack.h>

class Combined;
class Profile;
class CatheterTrackPackage;
class Catheter;
class BlackboardDb;
class BreathOptions;
class ReproduceOptions;
class FrameRate;


class ReactWorker : public QObject
{
    Q_OBJECT
public:
    explicit ReactWorker(QObject *parent = nullptr);
    void exit();
    void init(Profile *newProfile,Combined *combined);
    void setTrackRate(qint32 newTrackRate);


signals:
    void depicted(quint32 count);

public slots:
    void onStarted();
    void onProfileStateChanged();
    void onCatheterTrackChanged(const QSharedPointer<CatheterTrackPackage> &trackData);

private:
    void checkCatheterTrackData(Catheter *catheter, const QList<CatheterTrack> &trackDatas, vtkIdList *idTotalList);
    void depictBlackboardPoint(vtkIdList *idTotalList);
    void checkElectrodeTrackPosition(Catheter *catheter,const QString &electrodeId,const vtkVector3d &trackPosition, vtkIdList *idTotalList);
private:
    QPointer<Profile> m_profile;
    QPointer<Combined> m_combined;
    QPointer<QThread> m_thread;
    QPointer<BlackboardDb> m_blackboardDb;
    QPointer<BreathOptions> m_breathOptions;
    QPointer<ReproduceOptions> m_reproduceOptions;
    QPointer<FrameRate> m_frameRate;

};

#endif // REACTWORKER_H
