#include "StorageChekcerHost.h"

#include <QStorageInfo>
#include <QTimer>
#include "HalveType.h"

StorageChekcerHost::StorageChekcerHost(const QString &path, qint32 threshold,QObject *parent)
    : StorageCheckerSimpleSource{parent}
{
    setPath(path);
    setThreshold(threshold);
    m_timer = new QTimer(this);
    QObject::connect(m_timer, &QTimer::timeout, this, &StorageChekcerHost::onTimeout);
    m_timer->start(std::chrono::seconds(10));
}

qint64 convertGBToBytes(qint32 gigabytes) {
    return static_cast<qint64>(gigabytes) * 1024 * 1024 * 1024;
}
void StorageChekcerHost::onTimeout()
{
    QStorageInfo storage(path());
    if (storage.isValid() && storage.isReady()) {
        qint64 availableSpace = storage.bytesAvailable();
        if (availableSpace < convertGBToBytes(1)) {
            setState(Halve::SS_DANGER);
            emit alarm();
        } else if (availableSpace < convertGBToBytes(threshold())) {
            setState(Halve::SS_WARNING);
            emit alarm();
        } else {
            setState(Halve::SS_STEADY);
        }
        setAvailableSpace(availableSpace);
    } else {
        setState(Halve::SS_DANGER);
    }
}
