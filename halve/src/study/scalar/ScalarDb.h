#ifndef SCALARDB_H
#define SCALARDB_H

#include "ScalarColor.h"

#include <QLinearGradient>
#include <QObject>
#include <QPointer>

class Scalar;
class QLinearGradient;
class ScalarOptions;

class ScalarDb : public QObject
{
    Q_OBJECT
public:
    explicit ScalarDb(const QString &path, ScalarOptions *scalarOptions, QObject *parent = nullptr);
    bool save();
    void load();
    int size() const;
    void backup(const QString &path);
    Scalar* getLatData() const;
    Scalar* getVoltageData() const;
    QList<ScalarColor*> getLinearGradient();

    double latMax() const;
    void setLatMax(double newMax);

    double latMin() const;
    void setLatMin(double newMin);

    double voltageMax() const;
    void setVoltageMax(double newMax);

    double voltageMin() const;
    void setVoltageMin(double newMin);


    double latPos() const;
    void setLatPos(double newMax);

    double latHeight() const;
    void setLatHeight(double newMin);

    double voltagePos() const;
    void setVoltagePos(double newMax);

    double voltageHeight() const;
    void setVoltageHeight(double newMin);


    void getLatColor(double v, QColor& color);
    void getLatColor(double v, unsigned char color[3]);

    void getVoltageColor(double v, QColor& color);
    void getVoltageColor(double v, unsigned char color[3]);

private:
    void load(const QByteArray &catheterJson);
    Scalar* load(const QJsonObject &json);
    void initDefault();
    void connectSignals(Scalar *scalar);
    QString getStoragePath(const QString &path) const;

signals:
    void changed(Scalar *scalar);
    void adjusted(Scalar *scalar);

private:
    QString m_path;
    QPointer<ScalarOptions> m_scalarOptions;
    QPointer<Scalar> m_latScalar;
    QPointer<Scalar> m_voltageScalar;
};

#endif // SCALARDB_H
