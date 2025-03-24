#ifndef MAPPINGSETTING_H
#define MAPPINGSETTING_H

#include <QObject>
#include "HalveType.h"

class QJsonObject;
class MappingSetting : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double insideThrow READ insideThrow WRITE setInsideThrow NOTIFY insideThrowChanged FINAL)
    Q_PROPERTY(double outsideThrow READ outsideThrow WRITE setOutsideThrow NOTIFY outsideThrowChanged FINAL)
    Q_PROPERTY(double gap READ gap WRITE setGap NOTIFY gapChanged FINAL)
    Q_PROPERTY(Halve::UseDuplicatePoints useDuplicatePoints READ useDuplicatePoints WRITE setUseDuplicatePoints NOTIFY useDuplicatePointsChanged FINAL)
    Q_PROPERTY(double duplicateRadius READ duplicateRadius WRITE setDuplicateRadius NOTIFY duplicateRadiusChanged FINAL)

public:
    explicit MappingSetting(QObject *parent = nullptr);

    void fromJson(const QJsonObject &v);
    QJsonValue toJson() const;
    double insideThrow() const;
    void setInsideThrow(double newInsideThrow);
    double outsideThrow() const;
    void setOutsideThrow(double newOutsideThrow);
    double gap() const;
    void setGap(double newGap);
    Halve::UseDuplicatePoints useDuplicatePoints() const;
    void setUseDuplicatePoints(Halve::UseDuplicatePoints newUserDuplicatePoints);

    double duplicateRadius() const;
    void setDuplicateRadius(double newDuplicateRadius);

signals:
    void gapChanged();
    void outsideThrowChanged();
    void insideThrowChanged();
    void useDuplicatePointsChanged();
    void duplicateRadiusChanged();

private:
    double m_insideThrow = 7.0;
    double m_outsideThrow = 7.0;
    double m_gap =7.0;
    Halve::UseDuplicatePoints m_useDuplicatePoints = Halve::Last;
    double m_duplicateRadius = 7.0;


};

#endif // MAPPINGSETTING_H
