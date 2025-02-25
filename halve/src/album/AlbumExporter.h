#ifndef ALBUMEXPORTER_H
#define ALBUMEXPORTER_H

#include <QObject>
#include <QPointer>

#include <profile/Profile.h>

class Exporter;
class Profile;
class AlbumDb;

class AlbumExporter : public QObject {
    Q_OBJECT
    Q_PROPERTY(Profile *profile WRITE setProfile READ profile NOTIFY profileChanged FINAL)
    Q_PROPERTY(bool running READ running NOTIFY runningChanged FINAL)
    Q_PROPERTY(double progressValue READ progressValue NOTIFY progressValueChanged FINAL)

public:
    explicit AlbumExporter(QObject *parent = nullptr);
    ~AlbumExporter();
    void setProfile(Profile* profile);
    Profile *profile() const;
    bool running() const;
    double progressValue() const;
    void setProgressValue(int v);
    Q_INVOKABLE void exportToPath(const QString& destPath);

signals:
    void finished();
    void runningChanged();
    void progressValueChanged();
    void profileChanged();

public slots:
    void onCopyFinised(const QString &srcPath, const QString &destPath);

private:
    QPointer<Profile> m_profile;
    QPointer<AlbumDb> m_albumDb;
    QStringList m_list;
    qint32 m_progressValue = -1;
    QPointer<Exporter> m_exporter;
};

#endif // ALBUMEXPORTER_H
