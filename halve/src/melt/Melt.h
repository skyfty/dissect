#pragma once

#include <QColor>
#include <QObject>

class Melt : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QColor dyestuff READ dyestuff WRITE setDyestuff NOTIFY dyestuffChanged FINAL)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged FINAL)
    Q_PROPERTY(bool appeared READ appeared WRITE setAppeared NOTIFY appearedChanged FINAL)
    Q_PROPERTY(int diameter READ diameter WRITE setDiameter NOTIFY diameterChanged FINAL)
    Q_PROPERTY(bool descriptionAppeared READ descriptionAppeared WRITE setDescriptionAppeared NOTIFY descriptionAppearedChanged FINAL)
    Q_PROPERTY(int order READ order WRITE setOrder NOTIFY orderChanged FINAL)

public:
    explicit Melt(QObject *parent);
    ~Melt() = default;

    void fromJson(const QJsonObject& v);
    QJsonValue toJson() const;

    bool appeared() const;
    void setAppeared(bool newVisible);

    QColor dyestuff() const;
    void setDyestuff(const QColor &newColor);

    QString description() const;
    void setDescription(const QString &newDescription);

    int diameter() const;
    void setDiameter(int newDiameter);

    bool descriptionAppeared() const;
    void setDescriptionAppeared(bool newDescriptionAppeared);

    int order() const;
    void setOrder(int newOrder);

signals:
    void appearedChanged();
    void dyestuffChanged();
    void descriptionChanged();
    void diameterChanged();
    void descriptionAppearedChanged();
    void orderChanged();

private:
    bool m_appeared = true;
    int m_order = 1;

    QColor m_dyestuff;
    QString m_description;
    int m_diameter = 0;
    bool m_descriptionAppeared = false;
};
