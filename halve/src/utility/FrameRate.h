#ifndef FRAMERATE_H
#define FRAMERATE_H

#include <QDateTime>
#include <QObject>

class FrameRate : public QObject
{
    Q_OBJECT
public:
    explicit FrameRate(QObject *parent = nullptr);
    bool charge();

    quint64 rate() const;
    void setRate(quint64 newRate);

    quint64 frequency() const;

private:
    QDateTime m_lastTime;
    quint64 m_lastFrames = 0;
    quint64 m_totalFrames = 0;
    quint64 m_frequency = 1;
    quint64 m_rate = 4;
};

#endif // FRAMERATE_H
