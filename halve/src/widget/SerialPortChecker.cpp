#include "SerialPortChecker.h"

#include <QSerialPortInfo>

SerialPortChecker::SerialPortChecker(QObject *parent)
    : QObject{parent}
{}

bool SerialPortChecker::running() const
{
    return m_running;
}

void SerialPortChecker::setRunning(bool newRunning)
{
    if (m_running == newRunning)
        return;
    m_running = newRunning;
    emit runningChanged();
    if (m_running) {

    }
}
