#ifndef SCALAR_H
#define SCALAR_H

#include <QColor>
#include <QObject>

class Scalar : public QObject
{
    Q_OBJECT

    Q_PROPERTY(double min READ min WRITE setMin NOTIFY minChanged FINAL)
    Q_PROPERTY(double max READ max WRITE setMax NOTIFY maxChanged FINAL)
    Q_PROPERTY(double pos READ pos WRITE setPos NOTIFY posChanged FINAL)
    Q_PROPERTY(double height READ height WRITE setHeight NOTIFY heightChanged FINAL)

public:
    explicit Scalar(QObject *parent = nullptr);
    void fromJson(const QJsonObject &v);
    void toJson(QJsonObject &json) const;

    double min() const;
    void setMin(double newMin);

    double max() const;
    void setMax(double newMax);

    double height() const;
    void setHeight(double newHegiht);

    double pos() const;
    void setPos(double newPos);

    double begin() const;
    double end() const;

signals:
    void colorChanged();
    void minChanged();
    void maxChanged();

    void heightChanged();

    void posChanged();

private:
    double m_min = 0;
    double m_max = 0;
    double m_pos = 0;
    double m_height = 0;
};

#endif // SCALAR_H
