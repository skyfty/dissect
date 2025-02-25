#pragma once

#include <QObject>
#include <QDir>
#include <QSettings>
#include <QFutureWatcher>
#include <HalveType.h>
#include <QtQuick/QQuickItem>
#include <user/User.h>

class Profile;
class User;
class QQmlApplicationEngine;
class SurfaceSortDb;
class CatheterDb;
class ProfileData;
class CatheterRepertory;

class ProfileManager : public QObject
{
    Q_OBJECT;
    Q_PROPERTY(Profile* currentProfile READ getCurrentProfile NOTIFY currentProfileChanged);
    Q_PROPERTY(int userSize READ getUserCount CONSTANT);
    Q_PROPERTY(User* currentUser READ getCurrentUser NOTIFY currentUserChanged);
    Q_PROPERTY(bool studying READ isStudying NOTIFY studyingChanged);
    Q_PROPERTY(int profileSize READ getProfileSize CONSTANT);
    Q_PROPERTY(CatheterRepertory* catheterRepertory READ catheterRepertory CONSTANT);
    Q_PROPERTY(QString lastUserId READ lastUserId NOTIFY lastUserIdChanged  FINAL)
    Q_PROPERTY(Halve::ProfileManagerState state READ state WRITE setState NOTIFY stateChanged FINAL)

public:
    static ProfileManager* instance();

signals:
    void userAdded();
    void userDeleted();
    void userLoaded();

    void currentUserChanged();

    void studyingChanged();
    void currentProfileChanged();
    void profileListFinished();

    void profileAdded(const ProfileData *profile);
    void profileDeleted(const QString &id);

    void lastUserIdChanged();

    void currentLoadProfileIdChanged();

    void stateChanged();

private slots:
    void onStartoveringChanged(bool b);

public:
    ProfileManager(const QString& filePath, QObject *parent = nullptr);
    ~ProfileManager() ;
    Q_INVOKABLE Profile* getCurrentProfile() const;
    Q_INVOKABLE void setCurrentProfile(const QString &id);
    Q_INVOKABLE void setCurrentProfile(const ProfileData *profileData);
    void setCurrentProfile(Profile *profile);
    Q_INVOKABLE bool isCurrentProfile(const QString &id);

    Q_INVOKABLE User* getCurrentUser() const;

    bool isStudying() const;
    Q_INVOKABLE void startStudy();

    Q_INVOKABLE void removeProfile(const QString &id);
    Q_INVOKABLE void removeProfile(ProfileData *profileData);

    Q_INVOKABLE void addProfile(const QString& doctor, const QDateTime &surgicalDate, Halve::ChannelMode mode);
    QList<ProfileData*> getProfiles() const;
    int getProfileSize() const;
    Q_INVOKABLE void loadProfiles();
    ProfileData *loadProfileData(const QString& filePath);
    void saveProfile(ProfileData* profiles);
    ProfileData *getProfile(const QString& id) const;

    QList<User*> getUsers() const;
    int getUserCount() const;
    User* getUser(const QString& id) const;
    User* getUser(int row) const;
    User* getUserByIdCard(const QString& row) const;

    void removeUser(const User* user);
    void addUser(const User* user, bool open = false);
    void updateUser(const User* user);
    Q_INVOKABLE bool open(User* user);
    Q_INVOKABLE User* loadUser(const QString& filePath);
    User* activeUser(const QString& filePath);
    Q_INVOKABLE void loadUsers();
    Q_INVOKABLE bool open(const QString &id);
    Q_INVOKABLE void clean();
    CatheterRepertory* catheterRepertory();
    QString lastUserId() const;

    Halve::ProfileManagerState state() const;
    void setState(const Halve::ProfileManagerState &newState);

protected:
    bool saveUser(const User* user);
    void saveLastUser(const User* user);

private:
    bool m_studying = false;
    Halve::ProfileManagerState m_state = Halve::ProfileManagerState::PMS_STRAINING;
    QString m_usersDataPath;
    QList<User*> m_userList;
    QPointer<User> m_currentUser;
    QPointer<Profile> m_currentProfile;
    QPointer<CatheterRepertory> m_catheterRepertory;
    QList<ProfileData*> m_profileDataList;
};
