#ifndef DISSOLVEOPTIONS_H
#define DISSOLVEOPTIONS_H

#include <QObject>

class DissolveOptions : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool onlyValid READ onlyValid WRITE setOnlyValid NOTIFY onlyValidChanged FINAL)
    Q_PROPERTY(bool showFocalInstructions READ showFocalInstructions WRITE setShowFocalInstructions NOTIFY showFocalInstructionsChanged FINAL)
    Q_PROPERTY(double insideThrow READ insideThrow WRITE setInsideThrow NOTIFY insideThrowChanged FINAL)
    Q_PROPERTY(double outsideThrow READ outsideThrow WRITE setOutsideThrow NOTIFY outsideThrowChanged FINAL)
    Q_PROPERTY(int electrodeIndex READ electrodeIndex WRITE setElectrodeIndex NOTIFY electrodeIndexChanged FINAL)
    Q_PROPERTY(double radius READ radius WRITE setRadius NOTIFY radiusChanged FINAL)

public:
    explicit DissolveOptions(QObject *parent = nullptr);
    void fromJson(const QJsonObject &v);
    QJsonValue toJson() const;
    double insideThrow() const;
    void setInsideThrow(double newInsideThrow);
    double outsideThrow() const;
    void setOutsideThrow(double newOutsideThrow);

    bool onlyValid() const;
    void setOnlyValid(bool newOnlyValid);

    bool showFocalInstructions() const;
    void setShowFocalInstructions(bool newShowFocalInstructions);

    int electrodeIndex() const;
    void setElectrodeIndex(int newElectrodeIndex);

    double radius() const;
    void setRadius(double newRadius);

signals:

    void changed();
    void onlyValidChanged();
    void outsideThrowChanged();
    void insideThrowChanged();

    void showFocalInstructionsChanged();

    void electrodeIndexChanged();

    void radiusChanged();

private:
    bool m_onlyValid = true;
    bool m_showFocalInstructions = false;
    double m_insideThrow = 0.5;
    double m_outsideThrow = 0.5;
    int m_electrodeIndex = 0;
    double m_radius = 4.0;

};

#endif // DISSOLVEOPTIONS_H
