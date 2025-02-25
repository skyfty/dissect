#ifndef HOTKEY_H
#define HOTKEY_H

#include <QObject>
#include <HalveType.h>
#include <QPointer>
#include <QShortcut>
#include <QMap>
class Profile;

class Hotkey : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Profile* profile WRITE setProfile READ profile NOTIFY profileChanged);
public:
    explicit Hotkey(QObject *parent = nullptr);
    void load();
    void load(const QJsonObject &json);
    void setProfile(Profile* profile);
    Profile *profile() const;
signals:
    void profileChanged();

private:
    QPointer<Profile> m_profile;
    QMap<QShortcut*,QString> m_shortcuts;
};

#endif // HOTKEY_H
