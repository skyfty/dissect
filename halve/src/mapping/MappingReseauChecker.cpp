#include "MappingReseauChecker.h"
#include "mapping/MappingPointsDb.h"
#include "mapping/MappingSetting.h"
#include "profile/Profile.h"
#include "reseau/ReseauDb.h"

#include <QTimer>

class PointValidCheckerState {
public:
    PointValidCheckerState(Profile *profile)
        :m_profile(profile) {
        m_profile->setPointValidCheck(false);
    }

    ~PointValidCheckerState() {
        m_profile->setPointValidCheck(true);
    }
private:
    Profile *m_profile;
};


struct MappingPointComp {
    MappingPointComp(QList<MappingPoint>& mappingPointList)
        : m_mappingPointList(mappingPointList)
    {
    }
    auto operator()(std::function<bool(const MappingPoint& r, const MappingPoint &l)> fun) const
    {
        return std::max_element(m_mappingPointList.begin(), m_mappingPointList.end(), fun);
    }
    QList<MappingPoint>& m_mappingPointList;
};


MappingReseauChecker::MappingReseauChecker(QObject *parent)
    : QObject{parent}
{
    m_reseauTimer = new QTimer(this);
    connect(m_reseauTimer, &QTimer::timeout, this, &MappingReseauChecker::onReseauChangedTimerEvent);
    m_checkOvercomTimer = new QTimer(this);
    connect(m_checkOvercomTimer, &QTimer::timeout, this, &MappingReseauChecker::onCheckOvercomTimerEvent);
}

Profile *MappingReseauChecker::profile() const {
    return m_profile;
}

void MappingReseauChecker::setProfile(Profile* profile) {
    if (m_profile == profile) {
        return;
    }
    m_profile = profile;
    m_mappingPointsDb = m_profile->mappingPointsDb();
    connect(m_mappingPointsDb, &MappingPointsDb::sizeChanged, this, [this] {
        m_checkOvercomTimer->start(std::chrono::milliseconds(500));
    });
    m_reseauDb = m_profile->reseauDb();
    QObject::connect(m_reseauDb, &ReseauDb::changed, this, &MappingReseauChecker::onReseauChanged);
    QObject::connect(m_reseauDb, &ReseauDb::deleted, this, &MappingReseauChecker::onReseauChanged);
    QObject::connect(m_reseauDb, &ReseauDb::added, this, &MappingReseauChecker::onReseauChanged);
    QObject::connect(m_reseauDb, &ReseauDb::apparentChanged, this, &MappingReseauChecker::onReseauChanged);

    emit profileChanged();
}

MappingSetting *MappingReseauChecker::mappingSetting() const
{
    return m_mappingSetting;
}

void MappingReseauChecker::setMappingSetting(MappingSetting *newMeltSetting)
{
    if (m_mappingSetting == newMeltSetting)
        return;
    QObject::disconnect(m_mappingSetting, nullptr, nullptr, nullptr);
    m_mappingSetting = newMeltSetting;
    QObject::connect(m_mappingSetting, &MappingSetting::outsideThrowChanged, this, &MappingReseauChecker::onMappingOptionsChanged);
    QObject::connect(m_mappingSetting, &MappingSetting::insideThrowChanged, this, &MappingReseauChecker::onMappingOptionsChanged);
    QObject::connect(m_mappingSetting, &MappingSetting::gapChanged, this, [this] {
        m_checkOvercomTimer->start(std::chrono::milliseconds(1000));
    });
    emit mappingSettingChanged();
}

QList<MappingPoint>::iterator MappingReseauChecker::getMappingPointCompIterator(QList<MappingPoint> &mappingPointList) {
    MappingPointComp comp(mappingPointList);
    if (m_mappingSetting->useDuplicatePoints() == Halve::Last) {
        return comp([](const auto& l, const auto& r) { return l.time < r.time; });
    } else {
        switch (m_profile->mappingType()) {
        case Halve::Voltage: {
            return comp([](const auto& l, const auto& r) { return l.maxVoltage < r.maxVoltage; });
            break;
        }
        case Halve::Lat: {
            return comp([](const auto& l, const auto& r) { return l.lat < r.lat; });
            break;
        }
        }
    }
    return mappingPointList.end();
}

void MappingReseauChecker::checkMappingPointOvercome(const MappingPoint &mappingPoint) {
    QList<MappingPoint> mappingPointList;
    m_mappingPointsDb->getDataPoints(m_mappingSetting->gap(), mappingPoint.position, mappingPointList);
    if (mappingPointList.isEmpty()) {
        return;
    }
    auto maxIter = getMappingPointCompIterator(mappingPointList);
    if (maxIter == mappingPointList.end()) {
        return;
    }
    QList<MappingPoint> changedMappingPointList;
    for (int i = 0; i < mappingPointList.size(); ++i) {
        if (mappingPointList[i].id != maxIter->id && mappingPointList[i].overcome == false) {
            mappingPointList[i].overcome = true;
            changedMappingPointList.append(mappingPointList[i]);
        }
    }
    if (maxIter->overcome) {
        maxIter->overcome = false;
        changedMappingPointList.append(*maxIter);
    }
    if (changedMappingPointList.size() > 0) {
        m_mappingPointsDb->updateOvercome(changedMappingPointList);
    }
}

void MappingReseauChecker::onCheckOvercomTimerEvent() {
    PointValidCheckerState stateChecker(m_profile);
    m_checkOvercomTimer->stop();
    for (const MappingPoint& mappingPoint : m_mappingPointsDb->getDatas()) {
        if (mappingPoint.valid && mappingPoint.type == MappingPoint::SAMPLE) {
            checkMappingPointOvercome(mappingPoint);
        }
    }
}

bool MappingReseauChecker::checkMappingPoint(Reseau *reseau,double radius, MappingPoint &mappingPoint) {
    bool changed = false;
    auto [pointId, dist] = reseau->getClosestPointWithinRadius(mappingPoint.location, radius);
    auto valid = (pointId != -1 && (quint32)dist <= radius);
    if (valid != mappingPoint.valid) {
        mappingPoint.valid = valid;
        changed = true;
    }
    if (valid) {
        vtkVector3d position;
        reseau->getPoint(pointId, position);
        if (mappingPoint.position != position) {
            mappingPoint.position = position;
            changed = true;
        }
    }
    return changed;
}

bool MappingReseauChecker::checkMappingPoint(MappingPoint &mappingPoint) {
    bool changed = false;
    QList<QPair<Reseau*, double>> reseaus = m_reseauDb->getDatas(mappingPoint.location);
    if (reseaus.size() > 0) {
        double radius = reseaus[0].second < 0 ? m_mappingSetting->insideThrow() : m_mappingSetting->outsideThrow();
        changed = checkMappingPoint(reseaus[0].first, radius, mappingPoint);
    } else {
        mappingPoint.valid = false;
        changed = true;
    }
    return changed;
}

void MappingReseauChecker::onReseauChangedTimerEvent() {
    PointValidCheckerState stateChecker(m_profile);
    m_reseauTimer->stop();
    QList<MappingPoint> changedMappingPointList;
    bool currentMappingChanged = false;
    for (auto mappingPoint : m_mappingPointsDb->getDatas()) {
        if (mappingPoint.type != MappingPoint::SAMPLE) {
            continue;
        }
        bool changed = checkMappingPoint(mappingPoint);
        if (changed) {
            if (mappingPoint.id == m_profile->currentMappingPointId()) {
                currentMappingChanged = true;
            }
            changedMappingPointList.append(mappingPoint);
        }
    }
    if (changedMappingPointList.size() > 0) {
        m_mappingPointsDb->update(changedMappingPointList);
    }
    if (currentMappingChanged) {
        emit m_profile->currentMappingPointIdChanged();
    }
}

void MappingReseauChecker::onMappingOptionsChanged() {
    m_reseauTimer->start(std::chrono::milliseconds(300));
}

void MappingReseauChecker::onReseauChanged(Reseau *reseau) {
    m_reseauTimer->start(std::chrono::milliseconds(1500));
    m_checkOvercomTimer->start(std::chrono::milliseconds(2000));
}
