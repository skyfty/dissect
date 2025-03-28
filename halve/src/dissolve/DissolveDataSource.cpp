#include "DissolveDataSource.h"
#include "catheter/CatheterDb.h"
#include "catheter/ElectrodeNode.h"
#include "combined/CatheterTrackPackage.h"
#include "combined/Combined.h"
#include "dissolve/DissolveDb.h"
#include "dissolve/DissolveOptions.h"
#include "halitus/BreathOptions.h"
#include "halitus/BreathSurvey.h"
#include "profile/Profile.h"
#include "utility/ConnectCheck.h"

#include <catheter/Catheter.h>

using namespace std::placeholders;
DissolveDataSource::DissolveDataSource(QObject *parent)
    : QObject{parent}
{}

void DissolveDataSource::setProfile(Profile* profile) {
    if (m_profile == profile) {
        return;
    }

    m_profile = profile;
    QObject::connect(m_profile, &Profile::centerPointChanged, this, &DissolveDataSource::onCenterPointChanged);
    m_catheterDb = m_profile->catheterDb();
    m_dissolveOptions = m_profile->dissolveOptions();
    m_dissolveDb = m_profile->dissolveDb();
    m_breathOptions =  m_profile->breathOptions();
    m_reproduceOptions =  m_profile->reproduceOptions();
    QObject::connect(m_reproduceOptions, &ReproduceOptions::catheterIdChanged, this, &DissolveDataSource::currentCatheterChanged);

    emit profileChanged();
}

Profile *DissolveDataSource::profile() const {
    return m_profile;
}

void DissolveDataSource::onCenterPointChanged() {
    m_dissolveDb->clean();
}

void DissolveDataSource::setCombined(Combined* combined) {
    Q_ASSERT(combined != nullptr);
    m_combined = combined;
    QObject::connect(m_combined, &Combined::catheterTrackChanged, this, std::bind(&DissolveDataSource::onCatheterTrackChanged, this, _1));
}

void DissolveDataSource::setBreathSurvey(BreathSurvey *newBreathSurvey)
{
    if (m_breathSurvey == newBreathSurvey)
        return;
    m_breathSurvey = newBreathSurvey;
}

void DissolveDataSource::onCatheterTrackChanged(const QSharedPointer<CatheterTrackPackage> &trackData) {
    if (m_profile == nullptr) {
        return;
    }
    m_catheterTrack = trackData;
}

Catheter *DissolveDataSource::getCatheter() const {
    return m_catheterDb->getData(m_profile->reproduceOptions()->catheterId());
}

void DissolveDataSource::getTrackPosition(const CatheterTrack &track, vtkVector3d &position) {
    track.getPosition(position);
    if (m_breathOptions->enabledCompensate() && m_profile->pantSampling()) {
        m_breathSurvey->compensatePosition(position);
    }
}
void DissolveDataSource::strik() {
    if (m_catheterTrack == nullptr || m_catheterTrack->isEmpty()) {
        return;
    }
    Catheter *catheter = getCatheter();
    if (catheter == nullptr) {
        return;
    }
    auto catheterTrack = getElectrodeTrack(catheter);
    if (catheterTrack) {
        DissolvePoint point;
        point.time = QDateTime::currentMSecsSinceEpoch();
        getTrackPosition(*catheterTrack, point.position);
        m_dissolveDb->add(point);
        emit m_profile->dissolved();
    }
}

std::optional<CatheterTrack> DissolveDataSource::getElectrodeTrack(Catheter *catheter) const {
    Q_ASSERT(catheter != nullptr);

    const QList<CatheterTrack>& tracks = m_catheterTrack->getTracks(catheter);
    for (auto& track : tracks) {
        if (track.seat() == m_dissolveOptions->electrodeIndex()) {
            return track;
        }
    }
    return std::nullopt;
}
