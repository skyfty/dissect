#ifndef CHANNELBLENDMAGNETISM_H
#define CHANNELBLENDMAGNETISM_H

#include "combined/Blend.h"
#include <channel/ChannelTrack.h>

class ChannelTrackData;
class CatheterMagnetism;

class BlendMagnetism : public Blend
{
    Q_OBJECT
public:
    BlendMagnetism(Profile* profile, Catheter *catheter, QObject *parent = nullptr);
    QList<TrackData> process(const ChannelTrackData &dataBuffer);
    std::shared_ptr<ys::ElecIdentify> getElecIdentify();

    void startTrain();
    void finishTrain();
    void appendTrainData(const ChannelTrackData &dataBuffer);

    bool trained() const;
    void startTrainTimer(std::chrono::milliseconds interval);
    std::pair<quint16, quint16> getMagnetismSeat() const;
    CatheterMagnetism* getMagnetism() const;

private:
    void timerEvent(QTimerEvent *event) override;
    bool train(QVector<ys::InputParameter> &trainDatas);
    QList<TrackData> convert_20250319(
        quint16 port,
        quint16 consultSeat,
        quint16 targetSeat,
        const ChannelTrackData& dataBuffer);
    bool matrixPerfect(const Matrix3x4d& matrix);

private:
    bool m_trained = true;
    int m_timerId = -1;
    std::chrono::milliseconds m_trainInterval{std::chrono::seconds(5)};
    std::shared_ptr<ys::ElecIdentify> m_elecIdentify;
    QVector<ys::InputParameter> m_trainDatas;
    CatheterMagnetism *m_magnetism;    
};

#endif // CHANNELBLENDMAGNETISM_H
