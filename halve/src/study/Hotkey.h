#ifndef HOTKEY_H
#define HOTKEY_H
#include <windows.h>

#include <QObject>
#include <HalveType.h>
#include <QPointer>
#include <QShortcut>
#include <QMap>
#include <QAbstractNativeEventFilter>
class Profile;

class Hotkey : public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT
    Q_PROPERTY(Profile* profile WRITE setProfile READ profile NOTIFY profileChanged);
public:
    explicit Hotkey(QObject *parent = nullptr);   
    ~Hotkey();
    void load();
    void load(const QJsonObject &json);
    void setProfile(Profile* profile);
    Profile *profile() const;
signals:
    void profileChanged();
protected:
    bool nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result) override;

private:
    QPointer<Profile> m_profile;
    QMap<int,QString> m_shortcuts;
};

#endif // HOTKEY_H
