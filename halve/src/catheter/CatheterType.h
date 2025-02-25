#ifndef CATHETERTYPE_H
#define CATHETERTYPE_H

#include <QObject>

class CatheterType : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(bool buildin READ buildin WRITE setBuildin NOTIFY buildinChanged FINAL)

public:
    explicit CatheterType(QObject *parent = nullptr);
    void fromJson(const QJsonObject &v);
    QJsonObject toJson() const;

    QString name() const;
    void setName(const QString &newName);

    bool buildin() const;
    void setBuildin(bool newBuildin);

signals:
    void nameChanged(QString &name);

    void buildinChanged();

private:
    QString m_name;
    bool m_buildin = false;
};

#endif // CATHETERTYPE_H
