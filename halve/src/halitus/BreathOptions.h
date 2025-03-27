#ifndef BREATHOPTIONS_H
#define BREATHOPTIONS_H

#include <QObject>
#include <vtkSmartPointer.h>
#include <vtkVector.h>
#include "HalveType.h"

class vtkVector3d;
class vtkPoints;

class BreathOptions : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool enabledCompensate READ enabledCompensate WRITE setEnabledCompensate NOTIFY enabledCompensateChanged FINAL)
    Q_PROPERTY(bool breatheGate READ breatheGate WRITE setBreatheGate NOTIFY breatheGateChanged FINAL)
    Q_PROPERTY(bool breatheCompensation READ breatheCompensation WRITE setBreatheCompensation NOTIFY breatheCompensationChanged FINAL)
    Q_PROPERTY(bool breatheRestrictRenovate READ breatheRestrictRenovate WRITE
                   setBreatheRestrictRenovate NOTIFY
                       breatheRestrictRenovateChanged FINAL)
    Q_PROPERTY(double breatheLimitTop READ breatheLimitTop WRITE setBreatheLimitTop NOTIFY breatheLimitTopChanged FINAL)
    Q_PROPERTY(double breatheLimitBottom READ breatheLimitBottom WRITE setBreatheLimitBottom NOTIFY breatheLimitBottomChanged FINAL)
    Q_PROPERTY(double breatheMax READ breatheMax WRITE setBreatheMax NOTIFY breatheMaxChanged FINAL)
    Q_PROPERTY(double breatheBase READ breatheBase WRITE setBreatheBase NOTIFY breatheBaseChanged FINAL)
    Q_PROPERTY(double breathBoundaryRatio READ breathBoundaryRatio WRITE setBreathBoundaryRatio NOTIFY breathBoundaryRatioChanged FINAL)
    Q_PROPERTY(double breathePeak READ breathePeak WRITE setBreathePeak NOTIFY breathePeakChanged FINAL)
    Q_PROPERTY(double breatheValley READ breatheValley WRITE setBreatheValley NOTIFY breatheValleyChanged FINAL)
    Q_PROPERTY(double breatheLimit READ breatheLimit WRITE setBreatheLimit NOTIFY breatheLimitChanged FINAL)
    Q_PROPERTY(Halve::RespiratorMode respiratoryMode READ respiratoryMode WRITE setRespiratoryMode NOTIFY respiratoryModeChanged FINAL)

public:
    explicit BreathOptions(QObject *parent = nullptr);
    void fromJson(const QJsonObject &v);
    QJsonValue toJson();

    bool breatheCompensation() const;
    void setBreatheCompensation(bool newBreatheCompensation);

    bool breatheGate() const;
    void setBreatheGate(bool newBreatheGate);

    bool breatheRestrictRenovate() const;
    void setBreatheRestrictRenovate(bool newBreatheRestrictRenovate);

    double breatheLimitTop() const;
    void setBreatheLimitTop(double newBreatheLimitTop);

    double breatheLimitBottom() const;
    void setBreatheLimitBottom(double newBreatheLimitBottom);

    double breatheMax() const;
    void setBreatheMax(double newBreatheMax);

    bool enabledCompensate() const;
    void setEnabledCompensate(bool newEnabledCompensate);

    double lowestPosition() const;
    void setLowestPosition(const double &newLowestPosition);


    double breatheBase() const;
    void setBreatheBase(double newBreatheBase);

    double breathBoundaryRatio() const;
    void setBreathBoundaryRatio(double newBreathRatio);

    double breathePeak() const;
    void setBreathePeak(double newBreathePeak);

    double breatheValley() const;
    void setBreatheValley(double newBreatheValley);

    vtkSmartPointer<vtkPoints> compensatePoints() const;
    void setCompensatePoints(vtkSmartPointer<vtkPoints> newCompensatePoints);

    vtkVector3d compensateBasePoint() const;
    void setCompensateBasePoint(const vtkVector3d &newCompensateBasePoint);

    double breatheLimit() const;
    void setBreatheLimit(double newBreatheLimit);

    const QPair<double, double>& gatingValid() const;
    void setGatingValid(const QPair<double, double>&points);


    Halve::RespiratorMode respiratoryMode() const;
    void setRespiratoryMode(const Halve::RespiratorMode &newRespiratoryMode);

signals:

    void breatheRestrictRenovateChanged();

    void breatheGateChanged();

    void breatheCompensationChanged();

    void breathOptionsChanged();
    void breatheLimitTopChanged();

    void breatheLimitBottomChanged();

    void breatheMaxChanged();


    void enabledCompensateChanged();


    void breatheBaseChanged();
    void compensatePointsChanged();

    void breathBoundaryRatioChanged();

    void breathePeakChanged();

    void breatheValleyChanged();

    void breatheLimitChanged();


    void respiratoryModeChanged();

private:
    bool m_enabledCompensate = false;
    bool m_breatheGate = true;
    QPair<double, double> m_gateValid;
    bool m_breatheCompensation = false;
    bool m_breatheRestrictRenovate = false;
    double m_breatheMax = 8.0;
    double m_breatheLimitTop = 4.0;
    double m_breatheLimitBottom = -4.0;
    double m_breatheLimit = 0.0;
    double m_breatheBase = 0;
    double m_breathePeak = 0.0;
    double m_breatheValley = 0.0;
    double m_lowestPosition = 0;
    double m_breathBoundaryRatio = 5;
    vtkSmartPointer<vtkPoints> m_compensatePoints;
    vtkVector3d m_compensateBasePoint;
    Halve::RespiratorMode m_respiratoryMode =Halve::RES_MODE_GATING;
};

#endif // BREATHOPTIONS_H
