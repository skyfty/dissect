#include "FrameRate.h"

FrameRate::FrameRate(QObject *parent)
    : QObject{parent}
{
    m_lastTime = QDateTime::currentDateTime();
}
bool FrameRate::charge() {
    QDateTime currentTime = QDateTime::currentDateTime();
    if (m_lastTime.msecsTo(currentTime) > 1000) {
        m_frequency = std::max((int)(m_lastFrames / m_rate), 1);
        m_lastFrames = 0;
        m_lastTime = currentTime;
    }
    m_lastFrames++;
    return (m_totalFrames++) % m_frequency !=0;
}

quint64 FrameRate::rate() const
{
    return m_rate;
}

void FrameRate::setRate(quint64 newRate)
{
    m_rate = newRate;
}

quint64 FrameRate::frequency() const
{
    return m_frequency;
}
