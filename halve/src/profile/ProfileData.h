#ifndef PROFILEDATA_H
#define PROFILEDATA_H

#include <QObject>
#include <QDate>
#include <HalveType.h>

class QJsonObject;
class Profile;

class ProfileData : public QObject
{
    Q_OBJECT;

    Q_PROPERTY(QString id READ getId CONSTANT);
    Q_PROPERTY(QString doctor READ getDoctor WRITE setDoctor NOTIFY doctorChanged);
    Q_PROPERTY(QDateTime createtime READ getCreatetime WRITE setCreatetime NOTIFY createtimeChanged);
    Q_PROPERTY(QDateTime updatetime READ getUpdatetime WRITE setUpdatetime NOTIFY updatetimeChanged);
    Q_PROPERTY(QDateTime opentime READ getOpentime WRITE setOpentime NOTIFY opentimeChanged);
    Q_PROPERTY(QString description READ getDescription WRITE setDescription NOTIFY descriptionChanged);
    Q_PROPERTY(QDateTime surgicalTime READ surgicalTime WRITE setSurgicalTime NOTIFY surgicalTimeChanged FINAL)
    Q_PROPERTY(Halve::ChannelMode channelMode READ channelMode WRITE setChannelMode NOTIFY channelModeChanged FINAL)

public:
    explicit ProfileData(QObject *parent = nullptr);
    ProfileData(const QString &path, QObject *parent = nullptr);
    void fromJson(const QJsonObject &v);
    QJsonValue toJson() const;
    static ProfileData* fromFile(const QString &path,QObject *parent = nullptr);
    void save(const  QString &userPath);
    Profile* loadProfile(QObject *parent) const;
    void showInFolder() const;

    const QString& getId() const;

    QString getDoctor() const;
    void setDoctor(const QString& name);

    QDateTime getCreatetime() const;
    void setCreatetime(const QDateTime& v);

    QDateTime getUpdatetime() const;
    void setUpdatetime(const QDateTime& v);

    QDateTime getOpentime() const;
    void setOpentime(const QDateTime& v);

    QString getDescription() const;
    void setDescription(const QString &name);

    QDateTime surgicalTime() const;
    void setSurgicalTime(const QDateTime &newSurgicalTime);

    Halve::ChannelMode channelMode() const;
    void setChannelMode(const Halve::ChannelMode &newChannelMode);

signals:
    void descriptionChanged();
    void doctorChanged();
    void createtimeChanged();
    void updatetimeChanged();
    void opentimeChanged();

    void surgicalTimeChanged();

    void channelModeChanged();

private:
    QString m_path;
    QString m_id;
    QDateTime m_updatetime;
    QDateTime m_createtime;
    QDateTime m_opentime;
    QString m_doctor;
    QString m_description;
    QDateTime m_surgicalTime;
    Halve::ChannelMode m_channelMode = Halve::CHANNELMODE_BLEND;

    friend class Profile;
    friend class ProfileManager;
};

#endif // PROFILEDATA_H
