#ifndef ALBUMMODLE_H
#define ALBUMMODLE_H

#include <QAbstractListModel>
#include <QPointer>

#include <profile/Profile.h>

class AlbumDb;
class Picture;
class Profile;

class AlbumModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(Profile* profile WRITE setProfile READ profile NOTIFY profileChanged);
    Q_PROPERTY(bool screenGrapRunning READ screenGrapRunning NOTIFY screenGrapRunningChanged FINAL)
    Q_PROPERTY(bool screenRecordRunning READ screenRecordRunning NOTIFY screenRecordRunningChanged FINAL)

public:
    enum DataRoles{
        First = Qt::UserRole + 1,
        Thumbnail,
        Portrait,
        Type,
        Last
    };
    Q_ENUM(DataRoles)
public:
    explicit AlbumModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    void setProfile(Profile* profile);
    Profile *profile() const;
    Q_INVOKABLE void remove(int index);
    Q_INVOKABLE QString getFilePath(const QString &fileName);

    Q_INVOKABLE void screenshot();
    Q_INVOKABLE void startScreenGrap();
    Q_INVOKABLE void stopScreenGrap();
    bool screenGrapRunning() const;

    Q_INVOKABLE void startScreenRecord();
    Q_INVOKABLE void stopScreenRecord();
    bool screenRecordRunning() const;

private slots:
    void onAdded(const Picture* picture);
    void onScreenRecordChanged(const QString &name);

signals:
    void profileChanged();
    void shipFinished(const QString folder, const QString fileName);
    void screenGrapRunningChanged();
    void screenRecordRunningChanged();


private:
    QPointer<Profile> m_profile;
    QPointer<AlbumDb> m_albumDb;
};

#endif // ALBUMMODLE_H
