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

signals:
    void gapChanged();
    void outsideThrowChanged();
    void insideThrowChanged();
    void useDuplicatePointsChanged();
private:
    double m_insideThrow = 7.0;
    double m_outsideThrow = 7.0;
    double m_gap =7.0;
    Halve::UseDuplicatePoints m_useDuplicatePoints = Halve::Last;


};

#endif // MAPPINGSETTING_H
