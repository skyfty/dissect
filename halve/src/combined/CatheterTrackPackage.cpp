#include "CatheterTrackPackage.h"

#include <catheter/Catheter.h>

CatheterTrackPackage::CatheterTrackPackage(QObject *parent)
    : QObject{parent}
{}
QList<CatheterTrack>& CatheterTrackPackage::getTracks(Catheter *catheter) {
    return m_map[catheter];
}
QList<Catheter*> CatheterTrackPackage::getCatheters() {
    return m_map.keys();
}
bool CatheterTrackPackage::isEmpty() const {
    return m_map.isEmpty();
}

bool CatheterTrackPackage::isTrackEmpty(Catheter *catheter) const {
    if (m_map.contains(catheter)) {
        return m_map[catheter].isEmpty();
    }
    return true;
}

std::optional<CatheterTrack> CatheterTrackPackage::getPant(Catheter* catheter, quint16 port) const {
    const QList<CatheterTrack>& catheterTracks = m_map[catheter];
    auto iter = std::find_if(catheterTracks.begin(), catheterTracks.end(), [&port](const CatheterTrack &track){return track.seat() == port;});
    if (iter == std::end(catheterTracks)) {
      return std::nullopt;
    }
    return std::make_optional(*iter);
}
std::optional<CatheterTrack> CatheterTrackPackage::getPant(quint16 port) const {
    for(Catheter* catheter:m_map.keys()) {
        if (catheter->isPant()) {
            return getPant(catheter, port);
        }
    }
    return std::nullopt;
}
bool CatheterTrackPackage::exist(Catheter *catheter) const {
    return m_map.contains(catheter);
}
