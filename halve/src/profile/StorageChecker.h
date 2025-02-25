#ifndef STORAGECHECKER_H
#define STORAGECHECKER_H

#include <QObject>
#include <QPointer>
#include "rep_storagechecker_merged.h"

class QRemoteObjectNode;
class QProcess;
class Profile;

class StorageChecker : public QObject
{
    Q_OBJECT

    Q_PROPERTY(qint32 threshold READ threshold WRITE setThreshold NOTIFY thresholdChanged FINAL)
    Q_PROPERTY(Profile* profile WRITE setProfile READ profile NOTIFY profileChanged);
    Q_PROPERTY(Halve::StorageState state READ state  NOTIFY stateChanged CONSTANT FINAL)

public:
    explicit StorageChecker(QObject *parent = nullptr);
    ~StorageChecker();
    Q_INVOKABLE void launch();
    qint32 threshold() const;
    void setThreshold(qint32 newThreshold);

    void setProfile(Profile* profile);
    Profile *profile() const;

    Halve::StorageState state() const;

signals:
    void thresholdChanged();

    void profileChanged();
    void alarm();

    void stateChanged();

public slots:
    void onFinised();


private:
    QPointer<Profile> m_profile;
    QScopedPointer<QRemoteObjectNode> m_remoteNode;
    QScopedPointer<StorageCheckerReplica> m_storageCheckerReplica;
    QProcess *m_process = nullptr;
    QString m_path;
    qint32 m_threshold = 1024;
};

#endif // STORAGECHECKER_H
