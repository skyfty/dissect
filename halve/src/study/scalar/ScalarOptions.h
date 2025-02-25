#ifndef SCALAROPTIONS_H
#define SCALAROPTIONS_H

#include <QLinearGradient>
#include <QObject>

class ScalarColor;
class Scalar;

class ScalarOptions : public QObject
{
    Q_OBJECT

    Q_PROPERTY(double latStep READ latStep WRITE setLatStep NOTIFY latStepChanged FINAL)
    Q_PROPERTY(double voltageStep READ voltageStep WRITE setVoltageStep NOTIFY voltageStepChanged FINAL)
    Q_PROPERTY(QList<ScalarColor *> scalarColor READ getLinearGradient CONSTANT FINAL)
    Q_PROPERTY(QColor lowColor READ lowColor WRITE setLowColor NOTIFY lowColorChanged FINAL)
    Q_PROPERTY(QColor highColor READ highColor WRITE setHighColor NOTIFY highColorChanged FINAL)

public:
    explicit ScalarOptions(QObject *parent = nullptr);
    void fromJson(const QJsonObject &v);
    QJsonValue toJson();
    QList<ScalarColor*> getLinearGradient();

    double latStep() const;
    void setLatStep(double newLatStep);

    double voltageStep() const;
    void setVoltageStep(double newVoltageStep);

    QColor getColorAtPosition(const QLinearGradient &gradient, qreal position);
    void getScalarColor(const Scalar* scalars,double v, QColor &color);
    void getVoltageScalarColor(const Scalar* scalars, double v, QColor& color);

    QColor lowColor() const;
    void setLowColor(const QColor &newLowColor);

    QColor highColor() const;
    void setHighColor(const QColor &newHighColor);

private:
    void loadScalarColor(const QByteArray &reseauJson);

signals:

    void latStepChanged();
    void voltageStepChanged();

    void lowColorChanged();

    void highColorChanged();

private:
    QList<ScalarColor*> m_scalarColors;
    QLinearGradient m_linearGradient;
    double m_latStep = 10;
    double m_voltageStep = 0.1;
    QColor m_lowColor = Qt::lightGray;
    QColor m_highColor{128, 0, 128};
};

#endif // SCALAROPTIONS_H
