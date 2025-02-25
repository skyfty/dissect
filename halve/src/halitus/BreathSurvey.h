#ifndef BREATHSURVEY_H
#define BREATHSURVEY_H

#include <QObject>
#include <QPointer>
#include <vtkPoints.h>
#include <vtkSmartPointer.h>
#include <vtkVector.h>

class Combined;
class Profile;
class BreathOptions;
class CatheterTrackPackage;
class vtkParametricSpline;

class BreathSurvey : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Profile* profile WRITE setProfile READ profile NOTIFY profileChanged);
    Q_PROPERTY(Combined* combined WRITE setCombined FINAL);
    Q_PROPERTY(qint32 gating READ gating WRITE setGating NOTIFY gatingChanged FINAL)
    Q_PROPERTY(double gurgitation READ gurgitation WRITE setGurgitation NOTIFY gurgitationChanged FINAL)
    Q_PROPERTY(bool overflow READ overflow WRITE setOverflow NOTIFY overflowChanged FINAL)
    Q_PROPERTY(qint32 renovateDelay READ renovateDelay WRITE setRenovateDelay NOTIFY renovateDelayChanged FINAL)
    Q_PROPERTY(qint32 samplingRate READ samplingRate WRITE setSamplingRate NOTIFY samplingRateChanged FINAL)
    Q_PROPERTY(double pantZPosition READ pantZPosition WRITE setPantZPosition NOTIFY pantZPositionChanged FINAL)
    Q_PROPERTY(bool samplingFilter READ samplingFilter WRITE setSamplingFilter NOTIFY samplingFilterChanged FINAL)
    Q_PROPERTY(double breathSamplingValidLimit READ breathSamplingValidLimit WRITE setBreathSamplingValidLimit NOTIFY breathSamplingValidLimitChanged FINAL)
    Q_PROPERTY(bool abnormal READ abnormal WRITE setAbnormal NOTIFY abnormalChanged FINAL)
    Q_PROPERTY(double breathGatingRatio READ breathGatingRatio WRITE setBreathGatingRatio NOTIFY breathGatingRatioChanged FINAL)

public:
    typedef QVector<QList<double>> SamplingVector;

public:
    explicit BreathSurvey(QObject *parent = nullptr);
    void setProfile(Profile *newProfile);
    Profile *profile() const;
    void setCombined(Combined* combined);
    bool computeRenovatingData();

    qint32 gating() const;
    void setGating(qint32 newGating);

    double gurgitation() const;
    void setGurgitation(double newGurgitation);

    vtkVector3d currentCompensation() const;
    void setCurrentCompensation(const vtkVector3d &newCurrentCompensation);

    bool overflow() const;
    void setOverflow(bool newOverflow);

    bool compensatePosition(vtkVector3d &pos);

    const vtkSmartPointer<vtkPoints>& positions() const;
    const vtkSmartPointer<vtkPoints>& pant1Positions() const;


    qint32 renovateDelay() const;
    void setRenovateDelay(qint32 newRenovateDelay);

    qint32 samplingRate() const;
    void setSamplingRate(qint32 newSamplingRate);

    double pantZPosition() const;
    void setPantZPosition(double newPantZPosition);

    bool samplingFilter() const;
    void setSamplingFilter(bool newSamplingFilter);

    double breathSamplingValidLimit() const;
    void setBreathSamplingValidLimit(double newBreathSamplingValidLimit);

    bool abnormal() const;
    void setAbnormal(bool newAbnormal);

    double breathGatingRatio() const;
    void setBreathGatingRatio(double newBreathThreshold);

private:
    void onCatheterTrackChanged(const QSharedPointer<CatheterTrackPackage> &trackData);
    void disposePantTrackData(const vtkVector3d &position);
    bool isOverflow(double pos) const;

    vtkSmartPointer<vtkPoints> arrangeSamplings(const vtkSmartPointer<vtkPoints> &samplings);
    std::vector<double> convertSamplings(const vtkSmartPointer<vtkPoints>& samplings, int idx);
   std::tuple<size_t, size_t, size_t>  getStrikingBreathSamplings(const vtkSmartPointer<vtkPoints> &points, double &maxBreath);
    void setSplinePoints(vtkPoints *points);
   vtkSmartPointer<vtkPoints> getEdgeSamplings(const vtkSmartPointer<vtkPoints> &samplings);
    void checkBreathState();
   bool isSuccessive(const std::vector<int> &v);



public slots:
    void onRenovatingChanged();
    void onRenovatTimerEvent();
    void onBreathBoundaryRatioChanged();

signals:
    void profileChanged();
    void gatingChanged();
    void gurgitationChanged();
    void currentCompensationChanged();
    void overflowChanged();
    void positionChanged();
    void pant1PositionChanged();

    void renovateDelayChanged();
    void pantSampleFinished();

    void samplingRateChanged();

    void pantZPositionChanged();

    void samplingFilterChanged();

    void breathSamplingValidLimitChanged();

    void abnormalChanged();

    void breathGatingRatioChanged();

private:
    QPointer<Profile> m_profile;
    QPointer<BreathOptions> m_breathOptions;

    QTimer *m_renovatTimer;
    QPointer<Combined> m_combined;
    vtkSmartPointer<vtkPoints> m_positions;
    vtkSmartPointer<vtkPoints> m_pant1Positions;
    vtkSmartPointer<vtkParametricSpline> m_spline;
    std::vector<double> m_breathZPosition;
    vtkVector3d m_currentCompensation;
    qint32 m_gating = 0;
    double m_gurgitation = 0;
    bool m_overflow = false;
    qint32 m_renovateDelay = 5;
    qint32 m_samplingRate = 20;
    double m_pantZPosition;
    bool m_samplingFilter = false;
    double m_breathGatingRatio = 30.0;
    double m_breathSamplingValidLimit = 0.5;
    bool m_abnormal = false;
};

#endif // BREATHSURVEY_H
