#ifndef SCALARCOLOR_H
#define SCALARCOLOR_H

#include <QColor>
#include <QObject>

class ScalarColor : public QObject
{
    Q_OBJECT

    Q_PROPERTY(double begin READ begin WRITE setBegin NOTIFY beginChanged FINAL)
    Q_PROPERTY(QString beginColor READ beginColorName CONSTANT FINAL)
    Q_PROPERTY(double end READ end WRITE setEnd NOTIFY endChanged FINAL)
    Q_PROPERTY(QString endColor READ endColorName CONSTANT FINAL)

public:
    explicit ScalarColor(QObject *parent = nullptr);
    void fromJson(const QJsonObject &v);

    double begin() const;
    void setBegin(double newBegin);

    QColor beginColor() const;
    QString beginColorName() const;
    void setBeginColor(const QColor &newBeginColor);

    double end() const;
    void setEnd(double newEnd);

    QColor endColor() const;
    QString endColorName() const;

    void setEndColor(const QColor &newEndColor);

signals:
    void endColorChanged();
    void endChanged();
    void beginColorChanged();
    void beginChanged();

private:
    double m_begin;
    QColor m_beginColor;
    double m_end;
    QColor m_endColor;
};

#endif // SCALARCOLOR_H
