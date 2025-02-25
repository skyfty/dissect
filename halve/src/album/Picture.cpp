#include "Picture.h"

#include <QFile>
#include <QJsonObject>

Picture::Picture(QObject *parent)
    : QObject{parent}
{

}

bool Picture::fromJson(const QString &path, const QJsonObject& json) {
    if (json.contains("portrait")) {
        m_portrait = json["portrait"].toString();
        m_filePath = path + "/" + m_portrait;
    }
    if (json.contains("type")) {
        m_type = (Type)json["type"].toInt();
    }
    return true;
}

QJsonValue Picture::toJson() const {
    QJsonObject json;
    json["portrait"] = m_portrait;
    json["type"] = m_type;

    return json;
}

void Picture::deleteFile() {
    QFile file(m_filePath);
    if (file.exists()) {
        file.remove();
    }

    if (m_type == Type::Video) {
        QFile file(m_filePath + ".jpg");
        if (file.exists()) {
          file.remove();
        }
    }
}
QString Picture::portrait() const {
    return m_portrait;
}
QString Picture::getFilePath() const {
    return "file:///" +  m_filePath;
}

QString Picture::filePath() const
{
    return m_filePath;
}

void Picture::setPortrait(const QString &newFilePath) {
    if (m_portrait == newFilePath)
        return;
    m_portrait = newFilePath;
    emit portraitChanged();
}
QString Picture::thumbnail() const {
    if (m_type == Type::Image) {
        return getFilePath();
    }
    if (m_type == Type::Video) {
        return getFilePath() + ".jpg";
    }
    return "";
}

QDateTime Picture::createTime() const {
    return m_createTime;
}

void Picture::setCreateTime(const QDateTime &newCreateTime) {
    if (m_createTime == newCreateTime)
        return;
    m_createTime = newCreateTime;
    emit createTimeChanged();
}

void Picture::setFilePath(const QString &newFilePath)
{
    m_filePath = newFilePath;
}

Picture::Type Picture::type() const
{
    return m_type;
}

void Picture::setType(Picture::Type newType)
{
    if (m_type == newType)
        return;
    m_type = newType;
    emit typeChanged();
}
