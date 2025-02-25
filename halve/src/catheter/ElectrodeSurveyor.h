#ifndef ELECTRODESURVEYOR_H
#define ELECTRODESURVEYOR_H

#include <QHash>
#include <QObject>

class ElectrodeSurveyor : public QObject
{
    Q_OBJECT
public:
    explicit ElectrodeSurveyor(QObject *parent = nullptr);
    void fromJson(const QJsonArray &v);
    QJsonValue toJson() const;
    double getData(const QString &id, double def = 0) const;
    bool setData(const QString &id, double val);

    void remove(const QString &id);

signals:
    void changed(const QString &id);

private:
    QHash<QString, double> m_list;
};

#endif // ELECTRODESURVEYOR_H
