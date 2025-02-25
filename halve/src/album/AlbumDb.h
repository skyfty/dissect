#ifndef ALBUMDB_H
#define ALBUMDB_H

#include <QHash>
#include <QObject>
#include <QPointer>

class Picture;
class Profile;
class QProcess;
class ScreenRecord;

class AlbumDb : public QObject
{
    Q_OBJECT

public:
    explicit AlbumDb(QString path,QObject *parent = nullptr);
    void remove(Picture* melt);
    void remove(qsizetype row);
    void remove(const QString &path);

    bool save();
    void load();
    qsizetype size() const;
    Picture* getData(int row);
    QList<Picture*> getDatas();
    Picture* getData(const QString &portrait);

    QString screenshot();
    void screenRecord(const QString &name);
    void stopScreenRecord(const QString &name);
    bool running(const QString &name) const;

    void add();
    void ship(const QString &name, std::chrono::milliseconds value);

    QString savePath() const;
    bool exists(const QString &fileName);
    static QString getPictureStoragePath(const QString &path);

signals:
    void added(const Picture *picture);
    void deleted(const Picture* picture);
    void screenRecordChanged(const QString &name);
    void runningChanged();
    void shipFinished(const QString &name, const QString &savePath, const QString &fileName);
    void backupFinished(const QString &savePath);

private slots:
    void onScreenRecordFinised(const QString &name, const QString &savePath, const QString &filename);

private:
     Picture* addPicture(const QString &savePath, const QString &filename, int type);
private:
    QString m_path;
    QString m_savePath;
    QList<Picture*> m_pictures;
    QHash<QString, ScreenRecord*> m_screenRecords;
};

#endif // ALBUMDB_H
