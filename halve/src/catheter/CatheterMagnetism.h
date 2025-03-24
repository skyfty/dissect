#ifndef CATHETERMAGNETISM_H
#define CATHETERMAGNETISM_H

#include <QObject>
#include <Eigen/Eigen>
#include "Matrix3x4d.h"

class QJsonObject;

class CatheterMagnetism : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool magnetism READ magnetism WRITE setMagnetism NOTIFY magnetismChanged FINAL)
    Q_PROPERTY(quint32 consult READ consult WRITE setConsult NOTIFY consultChanged FINAL)
    Q_PROPERTY(double consultDistance READ consultDistance WRITE setConsultDistance NOTIFY consultDistanceChanged FINAL)
    Q_PROPERTY(quint32 target READ target WRITE setTarget NOTIFY targetChanged FINAL)
    Q_PROPERTY(double targetDistance READ targetDistance WRITE setTargetDistance NOTIFY targetDistanceChanged FINAL)
    Q_PROPERTY(QString description READ description CONSTANT FINAL)

public:
    explicit CatheterMagnetism(QObject *parent = nullptr);

    void fromJson(const QJsonObject &v);
    void toJson(QJsonObject &json) const;
    bool magnetism() const;
    void setMagnetism(bool newMagnetism);
    Matrix3x4d matrix() const;
    void setMatrix(const Matrix3x4d& newMatrix);


    int consult() const;
    void setConsult(int newMagnetismConsult);

    double consultDistance() const;
    void setConsultDistance(double newMagnetismConsultDistance);

    int target() const;
    void setTarget(int newMagnetismTarget);

    double targetDistance() const;
    void setTargetDistance(double newMagnetismTargetDistance);


    QString description() const;

signals:
    void consultChanged();
    void magnetismChanged();
    void consultDistanceChanged();
    void targetChanged();
    void targetDistanceChanged();

    void descriptionChanged();

private:
    bool m_magnetism = false;
    int m_consult = -1;
    double m_consultDistance = 0.0;
    int m_target = -1;
    double m_targetDistance = 0.0;
    Matrix3x4d m_matrix = Matrix3x4d::Zero();
};

#endif // CATHETERMAGNETISM_H
