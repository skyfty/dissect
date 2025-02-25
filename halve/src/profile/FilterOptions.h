#ifndef FILTEROPTIONS_H
#define FILTEROPTIONS_H

#include <QObject>
#include <QPointer>

class FilterOptionItem;
class QJsonObject;

class FilterOptions : public QObject
{
    Q_OBJECT
    Q_PROPERTY(FilterOptionItem *ecg READ getEcg CONSTANT FINAL)
    Q_PROPERTY(FilterOptionItem *catheterDouble READ getCatheterDouble CONSTANT FINAL)
    Q_PROPERTY(FilterOptionItem *catheterSingle READ getCatheterSingle  CONSTANT FINAL)

public:
    explicit FilterOptions(QObject *parent = nullptr);
    void fromJson(const QJsonObject &v);
    QJsonValue toJson();

    FilterOptionItem *getEcg();
    FilterOptionItem *getCatheterDouble();
    FilterOptionItem *getCatheterSingle();

signals:
    void changed();

private:
    QPointer<FilterOptionItem> m_ecg;
    QPointer<FilterOptionItem> m_catheterDouble;
    QPointer<FilterOptionItem> m_catheterSingle;

};

#endif // FILTEROPTIONS_H
