#ifndef MELTSETTING_H
#define MELTSETTING_H

#include <QObject>
#include <QPointer>
#include <HalveType.h>
class MeltSamplingElectrode;
class MeltConsultElectrodeOptions;
class ElectrodeNode;

class MeltSetting : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double ecgSurveyor READ ecgSurveyor WRITE setEcgSurveyor NOTIFY ecgSurveyorChanged FINAL)
    Q_PROPERTY(double bodySurveyor READ bodySurveyor WRITE setBodySurveyor NOTIFY bodySurveyorChanged FINAL)
    Q_PROPERTY(qint32 refurbishSpeed READ refurbishSpeed WRITE setRefurbishSpeed NOTIFY refurbishSpeedChanged FINAL)
    Q_PROPERTY(bool meltAxisLine READ meltAxisLine WRITE setMeltAxisLine NOTIFY meltAxisLineChanged FINAL)
    Q_PROPERTY(int rulerType READ rulerType WRITE setRulerType NOTIFY rulerTypeChanged FINAL)
    Q_PROPERTY(bool striked READ striked WRITE setStriked NOTIFY strikedChanged FINAL)
    Q_PROPERTY(qint32 max READ max WRITE setMax NOTIFY maxChanged FINAL)
    Q_PROPERTY(qint32 leftIndex READ leftIndex WRITE setLeftIndex NOTIFY leftIndexChanged FINAL)
    Q_PROPERTY(qint32 rightIndex READ rightIndex WRITE setRightIndex NOTIFY rightIndexChanged FINAL)
    Q_PROPERTY(qint32 baseIndex READ baseIndex WRITE setBaseIndex NOTIFY baseIndexChanged FINAL)
    Q_PROPERTY(qint32 leftShifting READ leftShifting WRITE setLeftShifting NOTIFY leftShiftingChanged FINAL)
    Q_PROPERTY(qint32 rightShifting READ rightShifting WRITE setRightShifting NOTIFY rightShiftingChanged FINAL)
    Q_PROPERTY(qint32 speed READ speed WRITE setSpeed NOTIFY speedChanged FINAL)
    Q_PROPERTY(QString consultElectrodeId READ getConsultElectrodeId WRITE setConsultElectrodeId NOTIFY consultElectrodeIdChanged FINAL)
    Q_PROPERTY(bool autoSensitivity READ autoSensitivity WRITE setAutoSensitivity NOTIFY autoSensitivityChanged FINAL)
    Q_PROPERTY(double sensitivity READ sensitivity WRITE setSensitivity NOTIFY sensitivityChanged FINAL)
    Q_PROPERTY(Halve::Perception consultPerception READ consultPerception WRITE setConsultPerception NOTIFY consultPerceptionChanged FINAL)
    Q_PROPERTY(qint32 minShiftWidth READ minShiftWidth WRITE setMinShiftWidth NOTIFY minShiftWidthChanged FINAL)
    Q_PROPERTY(bool shiftChanged READ shiftChanged WRITE setShiftChanged NOTIFY shiftChangedChanged FINAL)

public:
    explicit MeltSetting(QObject *parent = nullptr);
    void fromJson(const QJsonObject &v);
    QJsonObject toJson();
    bool striked() const;
    void setStriked(bool newStriked);
    qint32 refurbishSpeed() const;
    void setRefurbishSpeed(qint32 newRefurbishSpeed);

    double ecgSurveyor() const;
    void setEcgSurveyor(double newEcgSurveyor);

    double bodySurveyor() const;
    void setBodySurveyor(double newBodySurveyor);

    bool meltAxisLine() const;
    void setMeltAxisLine(bool newMeltAxisLine);

    int rulerType() const;
    void setRulerType(int newRulerType);

    qint32 max() const;
    void setMax(qint32 newMax);

    qint32 leftIndex() const;
    void setLeftIndex(qint32 newLeftLine);

    qint32 rightIndex() const;
    void setRightIndex(qint32 newRightLine);

    qint32 baseIndex() const;
    void setBaseIndex(qint32 newBaseLine);

    qint32 leftShifting() const;
    void setLeftShifting(qint32 newLeftShifting);

    qint32 rightShifting() const;
    void setRightShifting(qint32 newRightShifting);

    qint32 speed() const;
    void setSpeed(qint32 newSpeed);

    QString getConsultElectrodeId() const;
    void setConsultElectrodeId(const QString &newConsultElectrode);

    bool autoSensitivity() const;
    void setAutoSensitivity(bool newAutoSensitivity);

    double sensitivity() const;
    void setSensitivity(double newSensitivity);

    Halve::Perception consultPerception() const;
    void setConsultPerception(Halve::Perception newConsultPerception);
    qint32 minShiftWidth() const;
    void setMinShiftWidth(qint32 newMinShiftWidth);

    bool shiftChanged() const;
    void setShiftChanged(bool newShiftChanged);

signals:

    void consultElectrodeIdChanged();
    void consultChanged();
    void autoSensitivityChanged();
    void sensitivityChanged();
    void consultPerceptionChanged();
    void strikedChanged();
    void meltAxisLineChanged();
    void rulerTypeChanged();

    void refurbishSpeedChanged();
    void ecgSurveyorChanged();

    void bodySurveyorChanged();

    void leftIndexChanged();
    void rightIndexChanged();
    void baseIndexChanged();
    void changed();
    void maxChanged();
    void speedChanged();

    void leftShiftingChanged();

    void rightShiftingChanged();

    void minShiftWidthChanged();

    void shiftChangedChanged();

private:
    bool m_striked = false;
    bool m_meltAxisLine = false;
    int m_rulerType = 0;
    double m_ecgSurveyor = 10;
    qint32 m_refurbishSpeed = 500;
    double m_bodySurveyor = 10;
    qint32 m_max = 0;
    qint32 m_leftIndex = 0;
    qint32 m_rightIndex = 0;
    qint32 m_baseIndex = 0;
    qint32 m_leftShifting = -500;
    qint32 m_rightShifting= 500;
    qint32 m_speed = 25;
    QString m_consultElectrodeId;
    qint32 m_minShiftWidth = 2000;

    Halve::Perception m_consultPerception = Halve::Max;
    bool m_autoSensitivity = true;
    double m_sensitivity = 0.3;
    bool m_shiftChanged = false;
};

#endif // MELTSETTING_H
