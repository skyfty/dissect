#ifndef PROFILESTORAGEWORKER_H
#define PROFILESTORAGEWORKER_H

#include <QObject>
#include <QPointer>

class QThread;

class ProfileStorageWorker : public QObject
{
    Q_OBJECT
public:
    explicit ProfileStorageWorker(QObject *parent = nullptr);
    ~ProfileStorageWorker();
    void init();
    void exit();

private:
    QPointer<QThread> m_thread;
};

#endif // PROFILESTORAGEWORKER_H
