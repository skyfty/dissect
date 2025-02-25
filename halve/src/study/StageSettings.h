#ifndef STAGESETTINGS_H
#define STAGESETTINGS_H

#include <QHash>
#include <QObject>

class QJsonObject;
class QJsonArray;
class StageOptions;

class StageSettings : public QObject
{
    Q_OBJECT
public:
    explicit StageSettings(QObject *parent = nullptr);
    void fromJson(const QJsonArray &v);
    QJsonValue toJson() const;
    Q_INVOKABLE StageOptions* getOptions(const QString &name);

signals:

private:
    StageOptions* load(const QJsonObject &json);

private:
    QHash<QString, StageOptions*> m_options;
};

#endif // STAGESETTINGS_H
