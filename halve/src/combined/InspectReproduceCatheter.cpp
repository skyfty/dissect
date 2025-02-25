#include "CatheterTrackPackage.h"
#include "Combined.h"
#include <channel/Channel.h>
#include "catheter/CatheterDb.h"
#include <catheter/Catheter.h>
#include "mesh/ReproduceOptions.h"
#include "CheckEnvironmentHelper.h"


void Combined::inspectMagneticReproduceCatheter(const QSharedPointer<CatheterTrackPackage> &catheterTracks) {
    CheckEnvironmentHelper flags(m_environmentFlags);
    QString catheterId = m_reproduceOptions->catheterId();
    if (m_catheterDb->size() != 0 && !catheterId.isEmpty()) {
        Catheter* reproduceCatheter = m_catheterDb->getData(catheterId);
        if (reproduceCatheter != nullptr ) {
            bool reproduceCatheterCasketState = reproduceCatheter->bseat() != CatheterNoSeta;
            if (reproduceCatheterCasketState) {
                if (catheterTracks->isTrackEmpty(reproduceCatheter)) {
                    flags(Halve::AN_REPRODUCE_CATHETER_NOT_RECEICE_DATA_ERROR, true);
                } else {
                    const QList<CatheterTrack>& tracks = catheterTracks->getTracks(reproduceCatheter);
                    Halve::TrackStatus status = tracks.front().status();
                    if (status != Halve::TrackStatus_Valid) {
                        if (status == Halve::TrackStatus_Missing) {
                            m_reproductCatheterMissedCount++;
                            if (m_reproductCatheterMissedCount > 1000) {
                                flags(Halve::AN_REPRODUCE_CATHETER_MISSED_ERROR, true);
                            }
                        } else {
                            flags(Halve::AN_REPRODUCE_CATHETER_NOT_RECEICE_DATA_ERROR, true);
                        }
                    } else {
                        m_reproductCatheterMissedCount = 0;
                        flags(Halve::AN_REPRODUCE_CATHETER_TRACKDATA_ERROR, false);
                    }
                }
            }
            flags(Halve::AN_REPRODUCE_CATHETER_NOT_IN_CASKET_ERROR, !reproduceCatheterCasketState);
        }
    }
    setEnvironmentFlags(flags);
    setReproductCatheterStatus(flags & Halve::AN_REPRODUCE_CATHETER_ERROR ? Halve::TrackStatus_Invalid:Halve::TrackStatus_Valid);
}

void Combined::inspectElectricalReproduceCatheter(const QSharedPointer<CatheterTrackPackage> &catheterTracks) {
    CheckEnvironmentHelper flags(m_environmentFlags);
    QString catheterId = m_reproduceOptions->catheterId();
    if (m_catheterDb->size() != 0 && !catheterId.isEmpty()) {
        Catheter* reproduceCatheter = m_catheterDb->getData(catheterId);
        if (reproduceCatheter != nullptr) {
            if (reproduceCatheter->bseat() != CatheterNoSeta) {
                flags(Halve::AN_REPRODUCE_CATHETER_NOT_RECEICE_DATA_ERROR,!catheterTracks->exist(reproduceCatheter));
            }
        }
    }
    setEnvironmentFlags(flags);
    setReproductCatheterStatus(flags & Halve::AN_REPRODUCE_CATHETER_ERROR ? Halve::TrackStatus_Invalid:Halve::TrackStatus_Valid);

}
void Combined::inspectBlendReproduceCatheter(const QSharedPointer<CatheterTrackPackage> &catheterTracks) {
    CheckEnvironmentHelper flags(m_environmentFlags);
    setEnvironmentFlags(flags);
    setReproductCatheterStatus(flags & Halve::AN_REPRODUCE_CATHETER_ERROR ? Halve::TrackStatus_Invalid:Halve::TrackStatus_Valid);

}
void Combined::inspectReproduceCatheter(const QSharedPointer<CatheterTrackPackage> &catheterTracks) {
    switch(m_channel->mode()) {
    case Halve::CHANNELMODE_MAGNETIC: {
        inspectMagneticReproduceCatheter(catheterTracks);
        break;
    }
    case Halve::CHANNELMODE_ELECTRICAL: {
        inspectElectricalReproduceCatheter(catheterTracks);
        break;
    }
    case Halve::CHANNELMODE_BLEND: {
        inspectBlendReproduceCatheter(catheterTracks);
        break;
    }
    }
}
