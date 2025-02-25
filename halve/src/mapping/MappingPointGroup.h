#ifndef MAPPINGPOINTGROUP_H
#define MAPPINGPOINTGROUP_H


#include <QObject>
#include <QPointer>
#include <QString>

#include "melt/MeltSetting.h"
#include "mapping/MappingSetting.h"

class MappingPointGroup : public QObject {

    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged FINAL)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(MeltSetting* meltSetting READ meltSetting CONSTANT FINAL)
    Q_PROPERTY(MappingSetting* mappingSetting READ mappingSetting CONSTANT FINAL)

public:
    explicit MappingPointGroup(QObject *parent = nullptr);
    void fromJson(const QJsonObject &v);
    QJsonObject toJson() const;

    QString id() const;
    void setId(const QString &newId);

    QString name() const;
    void setName(const QString &newName);

    MeltSetting* meltSetting();
    MappingSetting* mappingSetting();

signals:
    void nameChanged();
    void idChanged();

private:
    QString m_id;
    QString m_name;
    QPointer<MeltSetting> m_meltSetting;
    QPointer<MappingSetting> m_mappingSetting;
};

#endif // MAPPINGPOINTGROUP_H
