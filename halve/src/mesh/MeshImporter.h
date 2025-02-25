#ifndef MESHIMPORTER_H
#define MESHIMPORTER_H

#include <QObject>
#include <QPointer>

#include "utility/VtkDataWarp.h"
class Profile;

class MeshImporter : public QObject
{

    Q_OBJECT
    Q_PROPERTY(Profile* profile READ profile WRITE setProfile NOTIFY profileChanged FINAL)
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged FINAL)

public:
    explicit MeshImporter(QObject *parent = nullptr);
    Q_INVOKABLE void open(const QString& filePath);

    Profile* profile() const;
    void setProfile(Profile* newProfile);

    bool running() const;
    void setRunning(bool newRunning);

signals:
    void profileChanged();
    void runningChanged();
private:
    QPointer<Profile> m_profile;
    bool m_running = false;
};

#endif // MESHIMPORTER_H
