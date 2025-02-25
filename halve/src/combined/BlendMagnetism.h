#ifndef CHANNELBLENDMAGNETISM_H
#define CHANNELBLENDMAGNETISM_H

#include "combined/Blend.h"
class ChannelTrackData;
class CatheterMagnetism;


class BlendMagnetism : public Blend
{
    Q_OBJECT
public:
    BlendMagnetism(Catheter *catheter, QObject *parent = nullptr);
    QList<TrackData> process(const ChannelTrackData &dataBuffer);
    std::shared_ptr<ys::ElecIdentify> getElecIdentify();
    std::shared_ptr<ys::Elec2WorldUpdater> getUpdater();

    void startTrain();
    void finishTrain();
    void appendTrainData(const ChannelTrackData &dataBuffer);

    bool trained() const;
    void startTrainTimer(std::chrono::milliseconds interval);
    std::pair<quint16, quint16> getMagnetismSeat() const;

private:
    void timerEvent(QTimerEvent *event) override;
    bool train(QVector<ys::InputParameter> &trainDatas);
    void initUpdater(const ChannelTrackData& dataBuffer, quint16 consultSeat, quint16 targetSeat);
    QList<TrackData> convert_20250221(
        quint16 portIdx,
        quint16 consultSeat,
        quint16 targetSeat,
        const ChannelTrackData& dataBuffer);
private:
    bool m_trained = false;
    int m_timerId = -1;
    std::chrono::milliseconds m_trainInterval{std::chrono::seconds(5)};
    std::shared_ptr<ys::ElecIdentify> m_elecIdentify;
    std::shared_ptr<ys::Elec2WorldUpdater> m_updater;
    QVector<ys::InputParameter> m_trainDatas;
    CatheterMagnetism *m_magnetism;
    QList<ChannelTrackData> m_inputBuffer;
};

#endif // CHANNELBLENDMAGNETISM_H
