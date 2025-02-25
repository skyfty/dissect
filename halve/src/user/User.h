#ifndef USER_H
#define USER_H

#include <QObject>
#include <QDate>

class ProfileManager;
class Profile;

class User : public QObject
{
    Q_OBJECT;

public:
    Q_PROPERTY(QString id READ getId CONSTANT);
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged);
    Q_PROPERTY(QString description READ getDescription WRITE setDescription NOTIFY descriptionChanged);
    Q_PROPERTY(Sex sex READ getSex WRITE setSex NOTIFY sexChanged);
    Q_PROPERTY(QDate birthday READ getBirthday WRITE setBirthday NOTIFY birthdayChanged);
    Q_PROPERTY(QString idcard READ getIdCard WRITE setIdCard NOTIFY idcardChanged);
    Q_PROPERTY(int weight READ getWeight WRITE setWeight NOTIFY weightChanged);
    Q_PROPERTY(QString doctor READ getDoctor WRITE setDoctor NOTIFY doctorChanged);
    Q_PROPERTY(QDateTime createtime READ getCreatetime WRITE setCreatetime NOTIFY createtimeChanged);
    Q_PROPERTY(QDateTime updatetime READ getUpdatetime WRITE setUpdatetime NOTIFY updatetimeChanged);
    Q_PROPERTY(QDateTime opentime READ getOpentime WRITE setOpentime NOTIFY opentimeChanged);
    Q_PROPERTY(bool startovering READ isStartovering  NOTIFY startoveringChanged);

    enum Sex {
        Woman,
        Man
    };
    Q_ENUM(Sex);

public:
    explicit User(QObject *parent = nullptr);
    static User* fromJson(const QString &dir, const QJsonObject &v, QObject *parent = nullptr);
    void toJson(QJsonObject &json) const;
    ~User()  = default;
    Q_INVOKABLE void deepCopy(const User* user);
    Q_INVOKABLE void startover();
    bool isStartovering() const;


    const QString& getPath() const {
        return m_path;
    }
    const QString& getId() const {
        return m_id;
    }

    void setId(const QString& id) {
        m_id = id;
    }

    QString getName() const;
    void setName(const QString& name);

    QString getDoctor() const;
    void setDoctor(const QString& name);
    void setIdCard(const QString& v);
    QString getIdCard() const;
    int getWeight() const;
    void setWeight(const int& v);
    QDate getBirthday() const;
    void setBirthday(const QDate& v);

    QDateTime getCreatetime() const;
    void setCreatetime(const QDateTime& v);

    QDateTime getUpdatetime() const;
    void setUpdatetime(const QDateTime& v);

    QDateTime getOpentime() const;
    void setOpentime(const QDateTime& v);

    Sex getSex() const;
    void setSex(const Sex& v);
    QString getDescription() const;
    void setDescription(const QString &name);


signals:
    void nameChanged();
    void descriptionChanged();
    void birthdayChanged();
    void sexChanged();
    void idcardChanged();
    void weightChanged();
    void doctorChanged();
    void createtimeChanged();
    void updatetimeChanged();
    void opentimeChanged();
    void startoveringChanged(bool b);


private slots:
    void onStartoverFinished();
private:
    bool m_startovering = false;
    QString m_id;
    mutable QString m_path;
    QDateTime m_updatetime;
    QDateTime m_createtime;
    QDateTime m_opentime;
    QString m_name;
    QString m_idcard;
    Sex m_sex;
    QDate m_birthday = QDate::currentDate();
    QString m_description;
    int m_weight = 0;
	QString m_doctor;

    friend ProfileManager;
};

#endif // USER_H
