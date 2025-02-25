#ifndef ALLEYWAY_H
#define ALLEYWAY_H

#include <QHash>
#include <QHostAddress>
#include <QObject>
#include <QPointer>

class QUdpSocket;
class Alleyway : public QObject
{
    Q_OBJECT
    Q_PROPERTY(quint16 port READ port WRITE setPort NOTIFY portChanged FINAL)
    Q_PROPERTY(quint16 fpgaPort READ fpgaPort WRITE setFpgaPort NOTIFY fpgaPortChanged FINAL)
    Q_PROPERTY(QString fpgaAddress WRITE setFpgaAddress NOTIFY fpgaAddressChanged FINAL)

public:
    explicit Alleyway(QObject *parent = nullptr);
    ~Alleyway();

    Q_INVOKABLE void connect();
    Q_INVOKABLE void disconnect();
    Q_INVOKABLE void reconnect();
    Q_INVOKABLE void turnon();
    Q_INVOKABLE void turnoff();
    Q_INVOKABLE void save();

    Q_INVOKABLE void send(const QString &data);
    void send(const QByteArray &data);
    quint16 port() const;
    void setPort(quint16 newPort);


    quint16 fpgaPort() const;
    void setFpgaPort(quint16 newFpgaPort);

    QString fpgaAddress() const;
    void setFpgaAddress(const QString &newFpgaAddress);



public slots:
    void onDataReady();
signals:
    void portChanged();
    void dataReady(const QString &data);
    void error(const QString &data);
    void fpgaPortChanged();
    void fpgaAddressChanged();

private:
    quint16 m_port = 6007;
    quint16 m_fpgaPort = 6007;
    QHostAddress m_fpgaAddress;
    QPointer<QUdpSocket> m_receiverSocket;
 };

#endif // ALLEYWAY_H
