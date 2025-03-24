#ifndef CHANNELBLENDMAGNETISM_H
#define CHANNELBLENDMAGNETISM_H

#include "combined/Blend.h"
#include <channel/ChannelTrack.h>

namespace ys
{
    struct KNNCell;
    class DynamicNearestNeighbor;
    class ScaleFactorGetter;
}
class ChannelTrackData;
class CatheterMagnetism;

class BlendMagnetism : public Blend
{
    Q_OBJECT
public:
    BlendMagnetism(Profile* profile, Catheter *catheter, QObject *parent = nullptr);
    QList<TrackData> process(const ChannelTrackData &dataBuffer, ys::DynamicNearestNeighbor *dnn);
    std::shared_ptr<ys::ElecIdentify> getElecIdentify();
    std::shared_ptr<ys::Elec2WorldUpdater> getUpdater();

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
    void initUpdater(const ChannelTrackData& dataBuffer, quint16 consultSeat, quint16 targetSeat);
    QList<TrackData> convert_20250226(
        quint16 port,
        quint16 consultSeat,
        quint16 targetSeat,
        const ChannelTrackData& dataBuffer,
        ys::DynamicNearestNeighbor *dnn);
    QList<TrackData> convert_20250319(
        quint16 port,
        quint16 consultSeat,
        quint16 targetSeat,
        const ChannelTrackData& dataBuffer);

    bool fillCell(
        quint16 port,
        quint16 consultSeat,
        quint16 targetSeat,
        const ChannelTrackData& dataBuffer,
        ys::KNNCell *refCell,
        ys::KNNCell *tgtCell);
    void addPoints(
        ys::DynamicNearestNeighbor *dnn,
        ys::KNNCell *cell,
        const float distanceThreshold);
private:
    bool m_trained = true;
    int m_timerId = -1;
    std::chrono::milliseconds m_trainInterval{std::chrono::seconds(5)};
    std::shared_ptr<ys::ElecIdentify> m_elecIdentify;
    std::shared_ptr<ys::Elec2WorldUpdater> m_updater;
    QVector<ys::InputParameter> m_trainDatas;
    CatheterMagnetism *m_magnetism;
    std::unique_ptr<ys::ScaleFactorGetter> m_scaleFactor;
};

#endif // CHANNELBLENDMAGNETISM_H
