#ifndef STORAGECHEKCERHOST_H
#define STORAGECHEKCERHOST_H

#include <QObject>
#include <QPointer>
#include "rep_storagechecker_merged.h"

class QTimer;

class StorageChekcerHost : public StorageCheckerSimpleSource
{
    Q_OBJECT
public:
    explicit StorageChekcerHost(const QString &path, qint32 threshold,QObject *parent = nullptr);
public slots:
    void onTimeout();

signals:
private:
    QPointer<QTimer> m_timer = nullptr;
};

#endif // STORAGECHEKCERHOST_H
