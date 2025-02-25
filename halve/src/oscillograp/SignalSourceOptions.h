#ifndef SIGNALSOURCEOPTIONS_H
#define SIGNALSOURCEOPTIONS_H

#include <QObject>

class SignalSourceOptions : public QObject
{
    Q_OBJECT
public:
    explicit SignalSourceOptions(QObject *parent = nullptr);
    void fromJson(const QJsonArray &v);
    QJsonValue toJson();

    Q_INVOKABLE bool itemSelected(int v);
    Q_INVOKABLE void setItemSelected(int v, bool b);

signals:
    void itemSelectedChanged();

private:
    QList<int> m_sources;
};

#endif // SIGNALSOURCEOPTIONS_H
