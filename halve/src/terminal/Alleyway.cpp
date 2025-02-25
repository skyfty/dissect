#include "Alleyway.h"

#include <QUdpSocket>

Alleyway::Alleyway(QObject *parent)
    : QObject{parent}
{}

Alleyway::~Alleyway() {
}

quint16 Alleyway::port() const
{
    return m_port;
}

void Alleyway::setPort(quint16 newPort)
{
    if (m_port == newPort)
        return;
    m_port = newPort;
    emit portChanged();
}

void Alleyway::disconnect(){
    if (m_receiverSocket != nullptr) {
        m_receiverSocket->close();
        m_receiverSocket->deleteLater();
    }
}

void Alleyway::reconnect()
{
    disconnect();
    connect();
}

void Alleyway::turnon()
{
    send(QByteArray::fromHex("5500010000000100AA"));
}

void Alleyway::turnoff()
{
    send(QByteArray::fromHex("5500010000000000AA"));
}

void Alleyway::save()
{
    send(QByteArray::fromHex("5504010000000100AA"));
}

void Alleyway::send(const QString &data) {
    send(data.toUtf8());
}

void Alleyway::send(const QByteArray &data)
{
    if (m_receiverSocket == nullptr) {
        emit error("Not connected");
    } else if(m_receiverSocket->writeDatagram(data, m_fpgaAddress, m_fpgaPort) == -1) {
        emit error(m_receiverSocket->errorString());
    }
}

void Alleyway::connect() {
    if (m_receiverSocket != nullptr) {
        return;
    }
    m_receiverSocket = new QUdpSocket(this);
    m_receiverSocket->bind(QHostAddress::Any, m_port);
    QObject::connect(m_receiverSocket, &QUdpSocket::readyRead, this, &Alleyway::onDataReady);
}

void Alleyway::onDataReady() {
    QByteArray buffer;
    buffer.resize(m_receiverSocket->pendingDatagramSize());
    m_receiverSocket->readDatagram(buffer.data(), buffer.size());
    emit dataReady(QString::fromUtf8(buffer));
}

quint16 Alleyway::fpgaPort() const
{
    return m_fpgaPort;
}

void Alleyway::setFpgaPort(quint16 newFpgaPort)
{
    if (m_fpgaPort == newFpgaPort)
        return;
    m_fpgaPort = newFpgaPort;
    emit fpgaPortChanged();
}

QString Alleyway::fpgaAddress() const
{
    return m_fpgaAddress.toString();
}

void Alleyway::setFpgaAddress(const QString &newFpgaAddress)
{
    QHostAddress fpgaAddress(newFpgaAddress);
    if (m_fpgaAddress == fpgaAddress)
        return;
    m_fpgaAddress = fpgaAddress;
    emit fpgaAddressChanged();
}
