#ifndef OSCILLOGRAPOPTIONS_H
#define OSCILLOGRAPOPTIONS_H

#include <QObject>
#include <QPointer>

class ElectrodeSurveyor;

class OscillograpOptions : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool oscillograpAxisLine READ oscillograpAxisLine WRITE setOscillograpAxisLine NOTIFY oscillograpAxisLineChanged FINAL)
    Q_PROPERTY(int oscillograpSpeed READ oscillograpSpeed WRITE setOscillograpSpeed NOTIFY oscillograpSpeedChanged FINAL)
    Q_PROPERTY(SortRole sortRole READ sortRole WRITE setSortRole NOTIFY sortRoleChanged FINAL)
    Q_PROPERTY(int rulerType READ rulerType WRITE setRulerType NOTIFY rulerTypeChanged FINAL)
    Q_PROPERTY(qint32 lookbackSpeed READ lookbackSpeed WRITE setLookbackSpeed NOTIFY lookbackSpeedChanged FINAL)

    Q_PROPERTY(double ecgSurveyor READ ecgSurveyor WRITE setEcgSurveyor NOTIFY ecgSurveyorChanged FINAL)
    Q_PROPERTY(double bodySurveyor READ bodySurveyor WRITE setBodySurveyor NOTIFY bodySurveyorChanged FINAL)

public:
    enum SortRole{
        ASC = 0,
        DESC
    };
    Q_ENUM(SortRole)
public:
    explicit OscillograpOptions(QObject *parent = nullptr);
    void fromJson(const QJsonObject &v);
    QJsonValue toJson();

    int oscillograpSpeed() const;
    void setOscillograpSpeed(int newOscillograpSpeed);

    bool oscillograpAxisLine() const;
    void setOscillograpAxisLine(bool newOscillograpAxisLine);

    SortRole sortRole() const;
    void setSortRole(SortRole newSortRole);

    int rulerType() const;
    void setRulerType(int newRulerType);


    double ecgSurveyor() const;
    void setEcgSurveyor(double newEcgSurveyor);

    double bodySurveyor() const;
    void setBodySurveyor(double newBodySurveyor);

    ElectrodeSurveyor *electrodeSurveyor();

    qint32 lookbackSpeed() const;
    void setLookbackSpeed(qint32 newLookbackSpeed);

signals:
    void oscillograpAxisLineChanged();
    void oscillograpSpeedChanged();

    void sortRoleChanged();
    void regulateChanged(int type, double val);

    void rulerTypeChanged();


    void ecgSurveyorChanged();

    void bodySurveyorChanged();

    void lookbackSpeedChanged();

private:
    qint32 m_oscillograpSpeed = 100;
    bool m_oscillograpAxisLine = false;

    SortRole m_sortRole = SortRole::ASC;
    int m_rulerType = 0;

    double m_ecgSurveyor = 10;
    double m_bodySurveyor = 10;
    qint32 m_lookbackSpeed = 30;

    QPointer<ElectrodeSurveyor> m_electrodeSurveyor;
};

#endif // OSCILLOGRAPOPTIONS_H
