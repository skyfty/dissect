#include <QProcess>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "profile/ProfileSnapshot.h"
#include "user/User.h"
#include "ProfileData.h"
#include "profile/ProfileTest.h"
#include "qdiriterator.h"
#include "qjsonobject.h"
#include <QJsonDocument>
#include <QtConcurrent>
#include "Profile.h"
#include "ProfileManager.h"
#include <catheter/CatheterRepertory.h>
#include "ProfileTestCleaner.h"

static ProfileManager* profileManager = nullptr;

ProfileManager::ProfileManager(const QString& filePath,QObject *parent)
    : QObject(parent),
      m_usersDataPath(filePath) {
    QDir appdataPath = filePath;
    if (!appdataPath.exists())appdataPath.mkpath(".");
    profileManager = this;
    qmlRegisterSingletonInstance("Halve", 1, 0, "ProfileManager", this);
}

ProfileManager::~ProfileManager() {
    profileManager = nullptr;
    if(m_currentProfile != nullptr) {
        ProfileTest* profileTest = qobject_cast<ProfileTest*>(m_currentProfile);
        if (profileTest) {
            ProfileTestCleaner(profileTest->path());
        }
    }
}

ProfileManager* ProfileManager::instance() {
    return profileManager;
}

void ProfileManager::clean() {
    qDeleteAll(m_userList);
    m_userList.clear();
}

CatheterRepertory *ProfileManager::catheterRepertory()
{
    if (m_catheterRepertory == nullptr) {
        m_catheterRepertory = new CatheterRepertory(m_usersDataPath, this);
    }
    return m_catheterRepertory;
}

void ProfileManager::startStudy() {
    if (m_currentUser == nullptr) {
        return;
    }
    m_currentUser->startover();
}

void ProfileManager::loadUsers() {
    QDirIterator it(m_usersDataPath, QDir::Dirs | QDir::NoDotAndDotDot);
    while (it.hasNext()) {
        User* newUser = loadUser(it.next());
        if (newUser != nullptr && getUser(newUser->getId()) == nullptr) {
            m_userList.push_front(newUser);
        }
    }
    emit userLoaded();
}

User* ProfileManager::loadUser(const QString& filePath) {
    User* newUser = nullptr;
    QFile f(filePath + "/config.json");
    if (f.open(QIODevice::ReadOnly)) {
        QJsonObject json = QJsonDocument::fromJson(f.readAll()).object();
        if (json.contains("id")) {
            QString id = json["id"].toString();
            if (!id.isEmpty()) {
                newUser = User::fromJson(filePath, json, this);
            }
        }
    }
    return newUser;
}

User* ProfileManager::activeUser(const QString& filePath) {
    User* newUser = loadUser(filePath);
    if (newUser != nullptr && getUser(newUser->getId()) == nullptr) {
        m_userList.push_front(newUser);
    }
    return newUser;
}


bool ProfileManager::saveUser(const User* user) {
    Q_ASSERT(user);
    Q_ASSERT(!user->m_id.isEmpty());
    if (user->m_path.isEmpty()) {
        user->m_path = m_usersDataPath + "/" + user->m_id;
        QDir userDir(user->m_path);if (!userDir.exists())userDir.mkdir(".");
    }
    QFile saveFile(user->m_path +"/config.json");
    if (!saveFile.open(QIODevice::WriteOnly)) {
        return false;
    }
    QJsonObject json;
    user->toJson(json);
    saveFile.write(QJsonDocument(json).toJson());
    return true;
}

void ProfileManager::saveLastUser(const User *user) {
    QSettings settings;
    settings.setValue("user/lastId", user->getId());
    emit lastUserIdChanged();
}

Halve::ProfileManagerState ProfileManager::state() const
{
    return m_state;
}

void ProfileManager::setState(const Halve::ProfileManagerState &newState)
{
    if (m_state == newState)
        return;
    m_state = newState;
    emit stateChanged();
}

QString ProfileManager::lastUserId() const {
    QSettings settings;
    QString id = settings.value("user/lastId").toString();
    if (id != "") {
        User *user = getUser(id);
        if (user == nullptr) {
            id = "";
        }
    }
    return id;
}


User* ProfileManager::getUser(const QString& id) const {
    auto iter = std::find_if(std::begin(m_userList), std::end(m_userList),
        [id](const User* user) {return user->getId() == id; });
    return iter != std::end(m_userList) ? *iter : nullptr;
}

User *ProfileManager::getUser(int row) const {
    return m_userList[row];
}

User *ProfileManager::getUserByIdCard(const QString& idcard) const
{
    auto iter = std::find_if(std::begin(m_userList), std::end(m_userList),
                             [idcard](const User* user) {return user->getIdCard() == idcard; });
    return iter != std::end(m_userList) ? *iter : nullptr;
}

Profile* ProfileManager::getCurrentProfile() const {
    return m_currentProfile;
}

void ProfileManager::setCurrentProfile(const QString &id) {
    if (m_currentUser == nullptr) {
        return;
    }
    ProfileData *profileData = getProfile(id);
    if (profileData != nullptr) {
        setCurrentProfile(profileData);
    } else {
        setCurrentProfile(new ProfileTest(this));
    }
}

void ProfileManager::setCurrentProfile(const ProfileData *profileData) {
    if (m_currentProfile != nullptr && m_currentProfile->getId() == profileData->getId()) {
        return;
    }
    setCurrentProfile(profileData->loadProfile(this));
}

void ProfileManager::setCurrentProfile(Profile *profile) {
    if (profile == m_currentProfile) {
        return;
    }
    Profile *tmp = m_currentProfile;
    m_currentProfile = profile;
    emit currentProfileChanged();
    if (tmp != nullptr) {
        tmp->deleteLater();
    }
}

bool ProfileManager::isCurrentProfile(const QString &id) {
    if (m_currentProfile == nullptr) {
        return false;
    }
    return m_currentProfile->getId() == id;
}

User* ProfileManager::getCurrentUser() const {
    return m_currentUser;
}

bool ProfileManager::isStudying() const {
    return m_studying;
}

bool ProfileManager::open(const QString &id) {
    User* user = getUser(id);
    if (user!=nullptr) {
        return open(user);
    }
    return false;
}

bool ProfileManager::open(User* user) {
    if (m_currentUser != nullptr) {
        if (m_currentUser->m_id == user->m_id)
            return true;
    }
    if (m_currentUser != nullptr) {
        m_currentUser->disconnect(this);
    }
    qDeleteAll(m_profileDataList);
    m_profileDataList.clear();

    m_currentUser = user;
    m_currentUser->connect(m_currentUser, &User::startoveringChanged, this, &ProfileManager::onStartoveringChanged);
    m_currentUser->m_opentime = QDateTime::currentDateTime();
    saveUser(m_currentUser);
    saveLastUser(m_currentUser);
    emit currentUserChanged();
    return true;
}

void ProfileManager::onStartoveringChanged(bool b) {
    m_studying = b;
    emit studyingChanged();
}

QVector<User*> ProfileManager::getUsers() const {
    return m_userList;
}

int ProfileManager::getUserCount() const {
    return m_userList.size();
}

void ProfileManager::removeUser(const User* user) {
    Q_ASSERT(user !=nullptr);
    auto iter = std::find(std::begin(m_userList), std::end(m_userList), user);
    if (iter != std::end(m_userList)) {
        m_userList.erase(iter);
        QDir(user->m_path).removeRecursively();
        delete user;
    }
}

void ProfileManager::addUser(const User* user, bool active) {
    User *newUser = new User(this);
    newUser->deepCopy(user);
    newUser->m_createtime = newUser->m_updatetime = QDateTime::currentDateTime();
    newUser->m_id = QString::number(newUser->m_createtime.toMSecsSinceEpoch());
    m_userList.push_front(newUser);
    emit userAdded();
    if (active) {
        open(m_userList[0]);
    } else {
        saveUser(newUser);
    }
}

void ProfileManager::updateUser(const User* user) {
    Q_ASSERT(user !=nullptr);
    saveUser(user);
}

QList<ProfileData*> ProfileManager::getProfiles() const {
    return m_profileDataList;
}

int ProfileManager::getProfileSize() const
{
    return m_profileDataList.size();
}

void ProfileManager::removeProfile(const QString &id) {
    if (m_currentUser != nullptr) {
        ProfileData *profileData = getProfile(id);
        if (profileData != nullptr) {
            removeProfile(profileData);
        }
    }
}

void ProfileManager::removeProfile(ProfileData *profileData) {
    QString id = profileData->getId();
    m_profileDataList.removeOne(profileData);

    if (QFileInfo::exists(profileData->m_path)) {
        QDir(profileData->m_path).removeRecursively();
    }
    if (m_currentProfile != nullptr && profileData->getId() == m_currentProfile->getId()) {
        setCurrentProfile(new ProfileTest(this));
    }
    delete profileData;
    emit profileDeleted(id);
}

void ProfileManager::addProfile(const QString& doctor, const QDateTime &surgicalTime, Halve::ChannelMode mode) {
    if (m_currentUser != nullptr) {
        ProfileData *newProfileData = new ProfileData(this);
        newProfileData->setDoctor(doctor);
        newProfileData->setSurgicalTime(surgicalTime);
        newProfileData->setChannelMode(mode);
        newProfileData->m_id = QString("%1").arg(QDateTime::currentMSecsSinceEpoch());
        newProfileData->m_createtime = newProfileData->m_updatetime = QDateTime::currentDateTime();
        m_profileDataList.push_front(newProfileData);
        saveProfile(newProfileData);
        emit profileAdded(newProfileData);
    }
}

void ProfileManager::loadProfiles() {
    QDirIterator it(m_currentUser->getPath(),QDir::Dirs | QDir::NoDotAndDotDot);
    while (it.hasNext()) {
        loadProfileData(it.next());
    }
    emit profileListFinished();
}

ProfileData *ProfileManager::loadProfileData(const QString &filePath) {
    ProfileData* newProfileData = ProfileData::fromFile(filePath, this);
    if (newProfileData != nullptr) {
        m_profileDataList.push_front(newProfileData);
    }
    return newProfileData;
}

void ProfileManager::saveProfile(ProfileData *profileData) {
    profileData->save(m_currentUser->getPath());
}

ProfileData *ProfileManager::getProfile(const QString &id) const {
    auto iter = std::find_if(std::begin(m_profileDataList), std::end(m_profileDataList), [id](const ProfileData* profile) {return profile->getId() == id; });
    return iter != std::end(m_profileDataList) ? *iter : nullptr;
}

