#include "User.h"
#include <QJsonDocument>
#include <QProcess>
#include <utility/StartoverState.h>
#include <QCoreApplication>
#include <qjsonobject.h>
#include <profile/Profile.h>

User::User(QObject *parent)
    :QObject(parent) {

}

User* User::fromJson(const QString &path, const QJsonObject &json, QObject *parent) {
    User *newUser = new User(parent);
    newUser->m_path = path;
    if (json.contains("id"))
        newUser->m_id = json["id"].toString();
    if (json.contains("name"))
        newUser->m_name = json["name"].toString();
    if (json.contains("description"))
        newUser->m_description = json["description"].toString();
    if (json.contains("sex"))
        newUser->m_sex = (Sex)json["sex"].toInt();
    if (json.contains("birthday"))
        newUser->m_birthday = QDate::fromString(json["birthday"].toString());
    if (json.contains("idcard"))
        newUser->m_idcard = json["idcard"].toString();
    if (json.contains("weight"))
        newUser->m_weight = json["weight"].toInt();
    if (json.contains("doctor"))
        newUser->m_doctor = json["doctor"].toString();
    if (json.contains("updatetime"))
        newUser->m_updatetime = QDateTime::fromMSecsSinceEpoch(json["updatetime"].toInteger());
    if (json.contains("createtime"))
        newUser->m_createtime = QDateTime::fromMSecsSinceEpoch(json["createtime"].toInteger());
    if (json.contains("opentime"))
        newUser->m_opentime = QDateTime::fromMSecsSinceEpoch(json["opentime"].toInteger());
    return newUser;
}

void User::toJson(QJsonObject &json) const {
    json["id"] = m_id;
    json["name"] = m_name;
    json["description"] = m_description;
    json["sex"] = (int)m_sex;
    json["birthday"] = m_birthday.toString();
    json["idcard"] = m_idcard;
    json["weight"] = m_weight;
    json["doctor"] = m_doctor;
    json["updatetime"] = m_updatetime.toMSecsSinceEpoch();
    json["createtime"] = m_createtime.toMSecsSinceEpoch();
    json["opentime"] = m_opentime.toMSecsSinceEpoch();
}

void User::deepCopy(const User* user) {
    Q_ASSERT(user != nullptr);
    m_id = user->m_id;
    m_name = user->m_name;
    m_description = user->m_description;
    m_birthday = user->m_birthday;
    m_sex = user->m_sex;
    m_idcard = user->m_idcard;
    m_weight = user->m_weight;
    m_doctor = user->m_doctor;
    m_updatetime = user->m_updatetime;
    m_opentime = user->m_opentime;
    m_createtime = user->m_createtime;
}

void User::startover() {
    auto ss = StartoverState::instance();
    if (ss->isRunning()) {
        return;
    }
    QProcess* process = new QProcess(this);
    QStringList arguments;
    arguments << "--pid" << QString("%1").arg(QCoreApplication::applicationPid());
    arguments << m_path;
    QObject::connect(process, &QProcess::finished, this, &User::onStartoverFinished);
    process->start(QCoreApplication::applicationFilePath(), arguments);
    m_startovering = process->waitForStarted();
    if (m_startovering) {
        ss->connect(ss, &StartoverState::finished, this, [this](){
            emit startoveringChanged(m_startovering);
        });
        ss->watch();
    } else {
        emit startoveringChanged(m_startovering);
    }
}

bool User::isStartovering() const {
    return m_startovering;
}

void User::onStartoverFinished() {
    m_startovering = false;
    emit startoveringChanged(m_startovering);
}

QString User::getName() const {
    return m_name;
}

void User::setName(const QString& name) {
    if (m_name != name) {
        m_name = name.trimmed();
        emit nameChanged();
    }
}


int User::getWeight() const {
    return m_weight;
}
void User::setWeight(const int& v) {
    if (m_weight != v) {
        m_weight = v;
        emit weightChanged();
    }
}

QString User::getDoctor() const {
    return m_doctor;
}

void User::setDoctor(const QString& v) {
    if (m_doctor != v) {
        m_doctor = v.trimmed();
        emit doctorChanged();
    }
}
QString User::getDescription() const {
    return m_description;
}

void User::setDescription(const QString& v) {
    if (m_description != v) {
        m_description = v;
        emit descriptionChanged();
    }
}

void User::setIdCard(const QString& v)  {
    if (m_idcard != v) {
        m_idcard = v.trimmed();
        emit idcardChanged();
    }
}
QString User::getIdCard() const  {
    return m_idcard;
}

User::Sex User::getSex() const {
    return m_sex;

}
void User::setSex(const User::Sex& v) {
    if (m_sex != v) {
        m_sex = v;
        emit sexChanged();
    }
}
QDate User::getBirthday() const {
    return m_birthday;

}
void User::setBirthday(const QDate& v) {
    if (m_birthday != v) {
        m_birthday = v;
        emit birthdayChanged();
    }
}

QDateTime User::getCreatetime() const{
    return m_createtime;

}
void User::setCreatetime(const QDateTime& v) {
    if (m_createtime != v) {
        m_createtime = v;
        emit createtimeChanged();
    }
}

QDateTime User::getUpdatetime() const{
    return m_updatetime;

}
void User::setUpdatetime(const QDateTime& v) {
    if (m_updatetime != v) {
        m_updatetime = v;
        emit updatetimeChanged();
    }
}

QDateTime User::getOpentime() const{
    return m_opentime;

}
void User::setOpentime(const QDateTime& v) {
    if (m_opentime != v) {
        m_opentime = v;
        emit opentimeChanged();
    }
}
