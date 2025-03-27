#include "Profile.h"
#include "ProfileData.h"
#include "ProfileFile.h"

#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <qjsonobject.h>

ProfileData::ProfileData(QObject *parent)
    : QObject{parent}
{
}

ProfileData::ProfileData(const QString &path, QObject *parent)
    : QObject{parent},
      m_path(path)
{
}

void ProfileData::fromJson(const QJsonObject &json) {
    if (json.contains("id"))
        m_id = json["id"].toString();
    if (json.contains("doctor"))
        m_doctor = json["doctor"].toString();
    if (json.contains("channelMode")) {
        m_channelMode =(Halve::ChannelMode)json["channelMode"].toInt();
    }
    if (json.contains("description"))
        m_description = json["description"].toString();
    if (json.contains("updatetime"))
        m_updatetime = QDateTime::fromMSecsSinceEpoch(json["updatetime"].toInteger());
    if (json.contains("createtime"))
        m_createtime = QDateTime::fromMSecsSinceEpoch(json["createtime"].toInteger());
    if (json.contains("opentime"))
        m_opentime = QDateTime::fromMSecsSinceEpoch(json["opentime"].toInteger());
    if (json.contains("surgicalTime")) {
        m_surgicalTime = QDateTime::fromMSecsSinceEpoch(json["surgicalTime"].toInteger());
    }
}

QJsonValue ProfileData::toJson() const {
    QJsonObject json;
    json["id"] = m_id;
    json["doctor"] = m_doctor;
    json["channelMode"] = m_channelMode;
    json["description"] = m_description;
    json["updatetime"] = m_updatetime.toMSecsSinceEpoch();
    json["createtime"] = m_createtime.toMSecsSinceEpoch();
    json["opentime"] = m_opentime.toMSecsSinceEpoch();
    json["surgicalTime"] = m_surgicalTime.toMSecsSinceEpoch();

    return json;
}

ProfileData *ProfileData::fromFile(const QString &path, QObject *parent) {
    ProfileData* newProfileData = nullptr;
    QFile f(path + "/profile.json");
    if (f.open(QIODevice::ReadOnly)) {
        QJsonObject json = QJsonDocument::fromJson(f.readAll()).object();
        if (json.contains("id")) {
            newProfileData = new ProfileData(path, parent);
            newProfileData->fromJson(json);
        }
    }
    return newProfileData;
}

const QString &ProfileData::getId() const {
    return m_id;
}

void ProfileData::save(const  QString &userPath) {
    if (m_path.isEmpty()) {
        m_path = userPath + "/" + m_id;
    }
    QDir dir(m_path);if (!dir.exists()) dir.mkdir(".");

    QFile saveFile(m_path + "/profile.json");
    if (saveFile.open(QIODevice::WriteOnly)) {
        QJsonValue json = toJson();
        saveFile.write(QJsonDocument(json.toObject()).toJson());
    }
}

Profile *ProfileData::loadProfile(QObject *parent) const {
    Profile *profile = new ProfileFile(getId(), m_path, parent);
    profile->setChannelMode(m_channelMode);
    profile->setName(m_doctor);
    return profile;
}

void ProfileData::showInFolder() const
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(m_path));
}

QString ProfileData::getDoctor() const {
    return m_doctor;
}

void ProfileData::setDoctor(const QString& v) {
    if (m_doctor != v) {
        m_doctor = v;
        emit doctorChanged();
    }
}

QString ProfileData::getDescription() const {
    return m_description;
}

void ProfileData::setDescription(const QString& v) {
    if (m_description != v) {
        m_description = v;
        emit descriptionChanged();
    }
}

QDateTime ProfileData::surgicalTime() const
{
    return m_surgicalTime;
}

void ProfileData::setSurgicalTime(const QDateTime &newSurgicalTime)
{
    if (m_surgicalTime == newSurgicalTime)
        return;
    m_surgicalTime = newSurgicalTime;
    emit surgicalTimeChanged();
}

Halve::ChannelMode ProfileData::channelMode() const
{
    return m_channelMode;
}

void ProfileData::setChannelMode(const Halve::ChannelMode &newChannelMode)
{
    if (m_channelMode == newChannelMode)
        return;
    m_channelMode = newChannelMode;
    emit channelModeChanged();
}


QDateTime ProfileData::getCreatetime() const{
    return m_createtime;

}
void ProfileData::setCreatetime(const QDateTime& v) {
    if (m_createtime != v) {
        m_createtime = v;
        emit createtimeChanged();
    }
}

QDateTime ProfileData::getUpdatetime() const{
    return m_updatetime;

}
void ProfileData::setUpdatetime(const QDateTime& v) {
    if (m_updatetime != v) {
        m_updatetime = v;
        emit updatetimeChanged();
    }
}

QDateTime ProfileData::getOpentime() const{
    return m_opentime;

}
void ProfileData::setOpentime(const QDateTime& v) {
    if (m_opentime != v) {
        m_opentime = v;
        emit opentimeChanged();
    }
}
