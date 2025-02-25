#ifndef SERIALPORTCHECKER_H
#define SERIALPORTCHECKER_H

#include <QObject>

class SerialPortChecker : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged FINAL)

public:
    explicit SerialPortChecker(QObject *parent = nullptr);

    bool running() const;
    void setRunning(bool newRunning);

signals:
    void runningChanged();

private:
    bool m_running = false;
};

#endif // SERIALPORTCHECKER_H
