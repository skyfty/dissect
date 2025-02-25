#include "FpgaTest.h"

#include <QUdpSocket>
#include <QFile>
#include <QDateTime>

FpgaTest::FpgaTest(QObject *parent)
    : QObject{parent}
{}
bool FpgaTest::s6008() const {
    return m_receiverSocket6008 != nullptr;
}
void FpgaTest::setS6008(bool b) {
    if (m_receiverSocket6008 != nullptr) {
        m_receiverSocket6008->close();
        m_receiverSocket6008->deleteLater();
        m_receiverSocket6008 = nullptr;
    }
    if (m_file6008 != nullptr) {
        m_file6008->close();
        m_file6008->deleteLater();
        m_file6008 = nullptr;

    }
    if (b) {
        m_file6008 = new QFile(QString("6008_%1").arg(QDateTime::currentSecsSinceEpoch()));
        m_file6008->open(QIODevice::WriteOnly);
        m_receiverSocket6008 = new QUdpSocket(this);
        m_receiverSocket6008->bind(QHostAddress::Any, 6008);
        QObject::connect(m_receiverSocket6008, &QUdpSocket::readyRead, this, &FpgaTest::on6008DataReady);
    }
    emit s6008Changed();
}
bool FpgaTest::s6009()const {
    return m_receiverSocket6009 != nullptr;
}
void FpgaTest::setS6009(bool b) {
    if (m_receiverSocket6009 != nullptr) {
        m_receiverSocket6009->close();
        m_receiverSocket6009->deleteLater();
        m_receiverSocket6009 = nullptr;
    }
    if (m_file6009 != nullptr) {
        m_file6009->close();
        m_file6009->deleteLater();
        m_file6009 = nullptr;

    }
    if (b) {
        m_file6009 = new QFile(QString("6009_%1").arg(QDateTime::currentSecsSinceEpoch()));
        m_file6009->open(QIODevice::WriteOnly);

        m_receiverSocket6009 = new QUdpSocket(this);
        m_receiverSocket6009->bind(QHostAddress::Any, 6009);
        QObject::connect(m_receiverSocket6009, &QUdpSocket::readyRead, this, &FpgaTest::on6009DataReady);
    }
    emit s6009Changed();
}

void FpgaTest::on6008DataReady() {
    QByteArray buffer;
    buffer.resize(m_receiverSocket6008->pendingDatagramSize());
    m_receiverSocket6008->readDatagram(buffer.data(), buffer.size());
    m_file6008->write(buffer);
}
void FpgaTest::on6009DataReady() {
    QByteArray buffer;
    buffer.resize(m_receiverSocket6009->pendingDatagramSize());
    m_receiverSocket6009->readDatagram(buffer.data(), buffer.size());
    m_file6009->write(buffer);
}
