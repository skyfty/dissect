#ifndef PICTURE_H
#define PICTURE_H

#include <QDateTime>
#include <QObject>

class Picture : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString portrait READ portrait NOTIFY portraitChanged FINAL)
    Q_PROPERTY(QString thumbnail READ thumbnail FINAL)

    Q_PROPERTY(QDateTime createTime READ createTime WRITE setCreateTime NOTIFY createTimeChanged FINAL)
    Q_PROPERTY(Type type READ type WRITE setType NOTIFY typeChanged FINAL)
public:
    enum Type{
        First = Qt::UserRole + 1,
        Image,
        Video,
        Last
    };
    Q_ENUM(Type)
public:
    explicit Picture(QObject *parent = nullptr);

    bool fromJson(const QString &path, const QJsonObject& v);
    QJsonValue toJson() const;

    void deleteFile();
    QString portrait() const;
    void setPortrait(const QString &newFilePath);
    QString thumbnail() const;

    QDateTime createTime() const;
    void setCreateTime(const QDateTime &newCreateTime);
    void setFilePath(const QString &newFilePath);
    QString getFilePath() const;
    QString filePath() const;

    Type type() const;
    void setType(Type newType);

signals:
    void portraitChanged();
    void createTimeChanged();


    void typeChanged();

private:
    QString m_portrait;
    QString m_filePath;
    Type m_type = Type::Image;

    QDateTime m_createTime;

};

#endif // PICTURE_H
