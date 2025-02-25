#ifndef FPGATEST_H
#define FPGATEST_H

#include <QObject>
#include <QPointer>

class QUdpSocket;
class QFile;

class FpgaTest : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool s6008 READ s6008 WRITE setS6008 NOTIFY s6008Changed FINAL)
    Q_PROPERTY(bool s6009 READ s6009 WRITE setS6009 NOTIFY s6009Changed FINAL)

public:
    explicit FpgaTest(QObject *parent = nullptr);
    bool s6008() const;
    void setS6008(bool b);
    bool s6009() const;
    void setS6009(bool b);

public slots:
    void on6008DataReady();
    void on6009DataReady();
signals:
    void s6008Changed();
    void s6009Changed();

private:
    QPointer<QUdpSocket> m_receiverSocket6008;
    QPointer<QFile> m_file6008;

    QPointer<QUdpSocket> m_receiverSocket6009;
    QPointer<QFile> m_file6009;

};

#endif // FPGATEST_H
