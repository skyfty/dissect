#ifndef NOTEBOOKDATASOURCE_H
#define NOTEBOOKDATASOURCE_H

#include <QObject>
#include <QPointer>

class Profile;
class Notebook;
class Channel;
class Combined;
class AlbumDb;
class MappingPointGroupDb;
class Picture;
class MappingPointGroup;
class NotebookMapping;
class NotebookGenre;
class NotebookDb;
class NotebookSnapshot;

class NotebookDataSource : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Profile* profile WRITE setProfile READ profile NOTIFY profileChanged);
    Q_PROPERTY(bool autoRecordScreen READ autoRecordScreen WRITE setAutoRecordScreen NOTIFY autoRecordScreenChanged FINAL)
    Q_PROPERTY(qint32 autoRecordDuration READ autoRecordDuration WRITE setAutoRecordDuration NOTIFY autoRecordDurationChanged FINAL)
    Q_PROPERTY(Channel* channel WRITE setChannel FINAL);
    Q_PROPERTY(bool snapping READ snapping WRITE setSnapping NOTIFY snappingChanged FINAL)
public:
    explicit NotebookDataSource(QObject *parent = nullptr);
    void setProfile(Profile* profile);
    Profile *profile() const;
    void setChannel(Channel *newChannel);

    Q_INVOKABLE void beginSnapshot(qint64 beginTime);
    Q_INVOKABLE void snapshot(qint64 endTime);

    Q_INVOKABLE void autoRecord(const QString& name);
    Q_INVOKABLE void open(qint64 id);

    bool autoRecordScreen() const;
    void setAutoRecordScreen(bool newAutoRecordScreen);

    bool snapping() const;
    void setSnapping(bool newSnapping);

    qint32 autoRecordDuration() const;
    void setAutoRecordDuration(qint32 newAutoRecordDuration);

signals:
    void profileChanged();
    void channelChanged();

    void snappingChanged();

    void autoRecordScreenChanged();
    void autoRecordDurationChanged();

public slots:
    void onShipFinished(const QString &name, const QString &savePath, const QString &fileName);
    void onAlbumAdded(const Picture *picture);
    void onAlbumDeleted(const Picture *picture);
    void onMappingPointGroupAdded(MappingPointGroup *group);
    void onMappingPointGroupDeleted(MappingPointGroup *group);
    void onMappingPointGroupChanged(MappingPointGroup *group);
    void onNotebookDeleted(Notebook *notebook);
protected:
    std::pair<qint32, NotebookMapping*> getNotebookMapping(MappingPointGroup *group);
    QString getNotebookFileName();
    void takeSnapshot(NotebookSnapshot *snapshot);

protected:
    QPointer<Profile> m_profile;
    QPointer<NotebookDb> m_notebookDb;
    QPointer<Channel> m_channel;
    QPointer<AlbumDb> m_albumDb;
    QPointer<MappingPointGroupDb> m_mappingPointGroupDb;
    bool m_autoRecordScreen = false;
    qint64 m_snapshotBeginTime = 0;
    bool m_snapping = false;
    qint32 m_autoRecordDuration = 11;

private:
};

#endif // NOTEBOOKDATASOURCE_H
