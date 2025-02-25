#include "BreathSurvey.h"
#include "combined/Combined.h"
#include "halitus/BreathOptions.h"

#include "utility/ConnectCheck.h"
#include <profile/Profile.h>

#include <halitus/BreathSurvey.h>

#include <EcgMethod/FindMaxPeak.h>
#include <algorithm>
#include "filter/BiDirectionFilterPipe.h"

#include <vtkMath.h>
#include <vtkPLYWriter.h>
#include <vtkParametricSpline.h>
#include <vtkPolyData.h>
#include <vtkPolyLine.h>
#include <vtkSplineFilter.h>
#include <breath/Breath.h>
#include <combined/CatheterTrackPackage.h>
#include <catheter/Catheter.h>
using namespace std::placeholders;


BreathSurvey::BreathSurvey(QObject* parent)
    : QObject{ parent }
{
    m_renovatTimer = new QTimer(this);
    connect(m_renovatTimer, &QTimer::timeout, this, &BreathSurvey::onRenovatTimerEvent);
    m_positions = vtkSmartPointer<vtkPoints>::New();
    m_pant1Positions = vtkSmartPointer<vtkPoints>::New();
}

void BreathSurvey::setCombined(Combined* combined) {
    Q_ASSERT(combined != nullptr);
    m_combined = combined;
    QObject::connect(m_combined, &Combined::catheterTrackChanged, this, std::bind(&BreathSurvey::onCatheterTrackChanged, this, _1));
}

std::vector<double> BreathSurvey::convertSamplings(const vtkSmartPointer<vtkPoints>& points, int idx) {
    std::vector<double> samplings;
    samplings.reserve(points->GetNumberOfPoints());
    for (vtkIdType i = 0; i < points->GetNumberOfPoints(); ++i) {
        vtkVector3d pos{ 0,0,0 };
        points->GetPoint(i, pos.GetData());
        samplings.push_back(pos[idx]);
    }
    return samplings;
}
std::tuple<size_t, size_t, size_t> BreathSurvey::getStrikingBreathSamplings(const vtkSmartPointer<vtkPoints>& points, double& maxBreath) {
    std::tuple<size_t, size_t, size_t> sampling{-1, -1, -1};
    for (int i = 0; i < 3; ++i) {
        std::vector<double> pointSamplings = convertSamplings(points, i);
        std::vector<size_t> peaks, valleys;
        ys::Breath<double> breath(m_samplingRate);
        breath.FindPeakValley(pointSamplings.begin(), pointSamplings.end(), peaks, valleys);
        if (valleys.size() < 2) {
            continue;
        }
        auto idx = (valleys.size() == 2 ? std::make_pair(0, 1) : std::make_pair(1, 2));
        auto lp = std::minmax_element(&pointSamplings[valleys[idx.first]], &pointSamplings[valleys[idx.second]]);
        auto mb = *(lp.second) - *(lp.first);
        if (mb > maxBreath) {
            maxBreath = mb;
            sampling = std::make_tuple(i, valleys[idx.first], valleys[idx.second]);
        }
    }
    return sampling;
}

vtkSmartPointer<vtkPoints> BreathSurvey::arrangeSamplings(const vtkSmartPointer<vtkPoints>& points) {
    vtkSmartPointer<vtkPoints> samplingFilterPoints = vtkSmartPointer<vtkPoints>::New();
    if (m_samplingFilter) {
        ys::BiDirectionFilterPipe<double> pipe;
        pipe.AddLowPass(8, m_samplingRate, 0.6);
        std::vector<std::vector<double>> pointSamplingsVec;
        for (int i = 0; i < 3; ++i) {
            std::vector<double> pointSamplings = convertSamplings(points, i);
            pointSamplingsVec.push_back(pipe.ProcessNoState(pointSamplings.begin(), pointSamplings.end()));
        }
        for (vtkIdType j = 0, num = points->GetNumberOfPoints(); j < num; ++j) {
            samplingFilterPoints->InsertNextPoint(pointSamplingsVec[0][j], pointSamplingsVec[1][j], pointSamplingsVec[2][j]);
        }
    }
    else {
        samplingFilterPoints = points;
    }

    double maxBreath = 0;
    auto [idx, first, second] = getStrikingBreathSamplings(samplingFilterPoints, maxBreath);
    vtkSmartPointer<vtkPoints> samplingPoints = vtkSmartPointer<vtkPoints>::New();
    if (first == -1 || second == -1 || maxBreath < 1.0) {
        return samplingPoints;
    }
    for (int j = first; j < second; ++j) {
        samplingPoints->InsertNextPoint(samplingFilterPoints->GetPoint(j));
    }

    return samplingPoints;
}
vtkSmartPointer<vtkPoints> BreathSurvey::getEdgeSamplings(const vtkSmartPointer<vtkPoints>& srcSamplingsPoints) {
    double breathThresholdRadio = m_breathGatingRatio / 100;
    auto srcPointsCount = srcSamplingsPoints->GetNumberOfPoints();
    auto samplingPointCount = srcPointsCount * breathThresholdRadio;
    vtkSmartPointer<vtkPoints> samplingPoints = vtkSmartPointer<vtkPoints>::New();
    for (int i = 0; i < samplingPointCount; ++i) {
        samplingPoints->InsertNextPoint(srcSamplingsPoints->GetPoint(i));
    }
    return samplingPoints;
}
constexpr int MiniumDataSize = 100;

typedef std::vector<std::pair<size_t, double>> DataIndexVector;
bool BreathSurvey::computeRenovatingData() {
    if (m_pant1Positions->GetNumberOfPoints() < MiniumDataSize || m_positions->GetNumberOfPoints() < MiniumDataSize) {
        return false;
    }
    auto pant1PositionSamplings = convertSamplings(m_pant1Positions, 2);
    DataIndexVector pant1PositionSamplingsIndexData;
    for (size_t i = 0; i < pant1PositionSamplings.size(); ++i) {
        pant1PositionSamplingsIndexData.push_back(std::make_pair(i, pant1PositionSamplings[i]));
    }
    std::vector<size_t> peaks, valleys;
    ys::Breath<double> breath(m_samplingRate);
    breath.FindPeakValley(pant1PositionSamplings.begin(), pant1PositionSamplings.end(), peaks, valleys);
    if (valleys.size() < 2 || peaks.size() < 2) {
        return false;
    }
    double breathThresholdRadio = m_breathGatingRatio / 100;

    std::vector<double> gatingSamplingsIndexData = pant1PositionSamplings;
    std::sort(gatingSamplingsIndexData.begin(), gatingSamplingsIndexData.end());
    gatingSamplingsIndexData.erase(std::unique(gatingSamplingsIndexData.begin(), gatingSamplingsIndexData.end(),[](double l , double r){
       int nl = l * 10;
       int nr = r * 10;
       return nl == nr;
    }), gatingSamplingsIndexData.end());
    std::vector<double> edgeGatingBreathData;
    auto gatingSamplingPointCount = gatingSamplingsIndexData.size() * breathThresholdRadio;
    auto gatingminmax = std::minmax_element(gatingSamplingsIndexData.begin(), gatingSamplingsIndexData.begin() + gatingSamplingPointCount);
    m_breathOptions->setGatingValid(std::make_pair(*(gatingminmax.first), *(gatingminmax.second)));

    auto minmaxval = std::minmax_element(pant1PositionSamplings.begin(), pant1PositionSamplings.end());
    m_breathOptions->setBreatheValley(*(minmaxval.first));
    m_breathOptions->setBreathePeak(*(minmaxval.second));
    m_breathOptions->setBreatheBase((m_breathOptions->breathePeak() + m_breathOptions->breatheValley()) / 2);
    onBreathBoundaryRatioChanged();

    auto breatheLimit = std::abs(m_breathOptions->breathePeak() - m_breathOptions->breatheValley());
    m_breathOptions->setBreatheLimit(breatheLimit);

    double peakAvg = 0;
    for (size_t i = 0; i < peaks.size(); ++i) {
        peakAvg += pant1PositionSamplings[peaks[i]];
    }
    peakAvg /= peaks.size();
    peaks.erase(std::remove_if(peaks.begin(), peaks.end(), [&pant1PositionSamplings, peakAvg](size_t idx) {
        auto validPeakValue = peakAvg - peakAvg * 5 / 100;
        return pant1PositionSamplings[idx] < validPeakValue;
    }), peaks.end());
    if (peaks.empty()) {
        return false;
    }
    size_t breathCount = peaks.size() - 1;
    size_t breathPointCount = 0;
    for (size_t i = 0; i < breathCount; ++i) {
        breathPointCount += (peaks[i] - peaks[i + 1]);
    }
    breathPointCount /= breathCount;
    if (breathPointCount >= pant1PositionSamplingsIndexData.size()) {
      return false;
    }

    vtkVector3d compensateBasePoint{};
    for (size_t i = 0; i < breathCount; ++i) {
        DataIndexVector breathIndexData;
        auto beginIter = pant1PositionSamplingsIndexData.begin() + i * breathPointCount;
        auto endIter = beginIter + breathPointCount;
        std::copy(beginIter, endIter, std::back_inserter(breathIndexData));
        std::sort(breathIndexData.begin(), breathIndexData.end(), [](const auto& l, const auto& r) {
            return l.second < r.second;
        });
        DataIndexVector edgeBreathData;
        auto samplingPointCount = breathIndexData.size() * breathThresholdRadio;
        std::copy(breathIndexData.begin(), breathIndexData.begin() + samplingPointCount, std::back_inserter(edgeBreathData));

        vtkSmartPointer<vtkPoints> edgeSamplingPoints = vtkSmartPointer<vtkPoints>::New();
        for (auto& data : edgeBreathData) {
            edgeSamplingPoints->InsertNextPoint(m_positions->GetPoint(data.first));
        }
        vtkVector3d edgetBreathBasePoint{};
        for (int i = 0; i < edgeSamplingPoints->GetNumberOfPoints(); ++i) {
            vtkMath::Add(edgeSamplingPoints->GetPoint(i), edgetBreathBasePoint, edgetBreathBasePoint);
        }
        vtkMath::MultiplyScalar(edgetBreathBasePoint.GetData(), 1.0 / edgeSamplingPoints->GetNumberOfPoints());
        vtkMath::Add(edgetBreathBasePoint, compensateBasePoint, compensateBasePoint);
    }
    vtkMath::MultiplyScalar(compensateBasePoint.GetData(), 1.0 / breathCount);
    m_breathOptions->setCompensateBasePoint(compensateBasePoint);

    vtkSmartPointer<vtkPoints> compensatePoints = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkPoints> samplingPoints = arrangeSamplings(m_positions);
    auto numOfPoint = samplingPoints->GetNumberOfPoints();
    for (auto i = 0; i < numOfPoint / 2; ++i) {
        vtkVector3d pos{ 0,0,0 }, pos2{ 0,0,0 };
        samplingPoints->GetPoint(i, pos.GetData());
        samplingPoints->GetPoint(numOfPoint - 1 - i, pos2.GetData());
        vtkMath::Add(pos, pos2, pos);
        vtkMath::MultiplyScalar(pos.GetData(), 0.5);
        compensatePoints->InsertNextPoint(pos.GetData());
    }
    setSplinePoints(compensatePoints);
    m_breathOptions->setCompensatePoints(compensatePoints);
    return compensatePoints->GetNumberOfPoints() >= 0 && m_breathOptions->breatheLimit() >= m_breathSamplingValidLimit;
}

void BreathSurvey::setSplinePoints(vtkPoints* points) {
    if (points->GetNumberOfPoints() > 0) {
        if (m_spline == nullptr) {
            m_spline = vtkSmartPointer<vtkParametricSpline>::New();
            m_spline->ClosedOff();
        }
        m_spline->SetPoints(points);
    }
    else {
        m_spline = nullptr;
    }
}

void BreathSurvey::setProfile(Profile* newProfile) {
    if (m_profile == newProfile) {
        return;
    }
    m_profile = newProfile;
    QObject::connect(m_profile, &Profile::renovatingChanged, this, &BreathSurvey::onRenovatingChanged);
    m_breathOptions = m_profile->breathOptions();
    QObject::connect(m_breathOptions, &BreathOptions::breathBoundaryRatioChanged, this, &BreathSurvey::onBreathBoundaryRatioChanged);

    setSplinePoints(m_breathOptions->compensatePoints());
    emit profileChanged();
}
void BreathSurvey::onBreathBoundaryRatioChanged() {
    auto breatheLimit = m_breathOptions->breatheLimit() / 2;
    auto breathBoundary = breatheLimit * m_breathOptions->breathBoundaryRatio() / 100;
    m_breathOptions->setBreatheLimitTop(m_breathOptions->breathePeak() + breathBoundary);
    m_breathOptions->setBreatheLimitBottom(m_breathOptions->breatheValley() - breathBoundary);
}

Profile* BreathSurvey::profile() const {
    return m_profile;
}

bool BreathSurvey::isOverflow(double gurgitation) const
{
    return gurgitation > m_breathOptions->breatheLimitTop() || gurgitation < m_breathOptions->breatheLimitBottom();
}
double BreathSurvey::breathGatingRatio() const
{
    return m_breathGatingRatio;
}

void BreathSurvey::setBreathGatingRatio(double newBreathThreshold)
{
    if (qFuzzyCompare(m_breathGatingRatio, newBreathThreshold))
        return;
    m_breathGatingRatio = newBreathThreshold;
    emit breathGatingRatioChanged();
}

bool BreathSurvey::isSuccessive(const std::vector<int> &v) {
    for(qsizetype i = 0; i < v.size() - 1; ++i) {
        if (v[i] + 1 != v[i+1]) {
            return false;
        }
    }
    return true;
}


void BreathSurvey::checkBreathState() {
    std::vector<size_t> peaks, valleys;
    ys::Breath<double> breath(m_samplingRate);
    breath.FindPeakValley(m_breathZPosition.begin(), m_breathZPosition.end(), peaks, valleys);
    //peaks.insert(peaks.end(), std::make_move_iterator(valleys.begin()), std::make_move_iterator(valleys.end()));
    std::vector<int> overflowValleys;
    for(auto d: valleys) {
        if (isOverflow(m_breathZPosition[d])) {
            overflowValleys.push_back(d);
        }
    }
    m_breathZPosition.clear();
    setAbnormal(overflowValleys.size() > 2 && isSuccessive(overflowValleys));
    m_profile->setPantSampling(!abnormal());
}

void BreathSurvey::disposePantTrackData(const vtkVector3d& position) {
    double zPos = position.GetZ();

    setOverflow(isOverflow(zPos));
    setGurgitation(zPos - m_breathOptions->breatheBase());

    auto gatingValid = m_breathOptions->gatingValid();
    setGating((zPos > gatingValid.first && zPos < gatingValid.second) ? 0 : -1);

    vtkVector3d compensationPoint{ 0,0,0 };
    if (m_spline != nullptr) {
        double ratio = (zPos - m_breathOptions->breatheValley()) / (m_breathOptions->breathePeak() - m_breathOptions->breatheValley());
        double u[] = { ratio, 0, 0 };
        m_spline->Evaluate(u, compensationPoint.GetData(), nullptr);
    }
    setPantZPosition(zPos);
    setCurrentCompensation(compensationPoint);
}

void BreathSurvey::onRenovatTimerEvent() {
    m_renovatTimer->stop();
    m_profile->setRenovating(true);
}

bool BreathSurvey::abnormal() const
{
    return m_abnormal;
}

void BreathSurvey::setAbnormal(bool newAbnormal)
{
    if (m_abnormal == newAbnormal)
        return;
    m_abnormal = newAbnormal;
    emit abnormalChanged();
}

double BreathSurvey::breathSamplingValidLimit() const
{
    return m_breathSamplingValidLimit;
}

void BreathSurvey::setBreathSamplingValidLimit(double newBreathSamplingValidLimit)
{
    if (qFuzzyCompare(m_breathSamplingValidLimit, newBreathSamplingValidLimit))
        return;
    m_breathSamplingValidLimit = newBreathSamplingValidLimit;
    emit breathSamplingValidLimitChanged();
}

bool BreathSurvey::samplingFilter() const
{
    return m_samplingFilter;
}

void BreathSurvey::setSamplingFilter(bool newSamplingFilter)
{
    if (m_samplingFilter == newSamplingFilter)
        return;
    m_samplingFilter = newSamplingFilter;
    emit samplingFilterChanged();
}

double BreathSurvey::pantZPosition() const
{
    return m_pantZPosition;
}

void BreathSurvey::setPantZPosition(double newPantZPosition)
{
    if (qFuzzyCompare(m_pantZPosition, newPantZPosition))
        return;
    m_pantZPosition = newPantZPosition;
    emit pantZPositionChanged();
}

qint32 BreathSurvey::samplingRate() const
{
    return m_samplingRate;
}

void BreathSurvey::setSamplingRate(qint32 newSamplingRate)
{
    if (m_samplingRate == newSamplingRate)
        return;
    m_samplingRate = newSamplingRate;
    emit samplingRateChanged();
}

qint32 BreathSurvey::renovateDelay() const
{
    return m_renovateDelay;
}

void BreathSurvey::setRenovateDelay(qint32 newRenovateDelay)
{
    if (m_renovateDelay == newRenovateDelay)
        return;
    m_renovateDelay = newRenovateDelay;
    emit renovateDelayChanged();
}


bool BreathSurvey::overflow() const
{
    return m_overflow;
}

void BreathSurvey::setOverflow(bool newOverflow)
{
    if (m_overflow == newOverflow)
        return;
    m_overflow = newOverflow;
    emit overflowChanged();
}

bool BreathSurvey::compensatePosition(vtkVector3d& pos) {
    if (m_breathOptions->breatheGate() && (gating() < 0)) {
        return false;
    } else if (m_breathOptions->breatheCompensation()) {
        vtkVector3d compensationPoint{};
        vtkMath::Subtract(m_currentCompensation, m_breathOptions->compensateBasePoint(), compensationPoint);
        vtkMath::Subtract(pos, compensationPoint, pos);
        return !(m_breathOptions->breatheRestrictRenovate() && overflow());
    }
    return true;
}

vtkVector3d BreathSurvey::currentCompensation() const
{
    return m_currentCompensation;
}

void BreathSurvey::setCurrentCompensation(const vtkVector3d& newCurrentCompensation)
{
    if (m_currentCompensation == newCurrentCompensation)
        return;
    m_currentCompensation = newCurrentCompensation;
    emit currentCompensationChanged();
}

void BreathSurvey::onRenovatingChanged() {
    setAbnormal(false);
    if (m_profile->renovating()) {
        m_breathZPosition.clear();
        m_positions->Reset();
        emit positionChanged();
        m_pant1Positions->Reset();
        emit pant1PositionChanged();
    } else {
        m_breathOptions->setEnabledCompensate(true);
        m_profile->setPantSampling(computeRenovatingData());
        emit pantSampleFinished();
    }
}

const vtkSmartPointer<vtkPoints>& BreathSurvey::pant1Positions() const
{
    return m_pant1Positions;
}

const vtkSmartPointer<vtkPoints>& BreathSurvey::positions() const
{
    return m_positions;
}

double BreathSurvey::gurgitation() const
{
    return m_gurgitation;
}

void BreathSurvey::setGurgitation(double newGurgitation)
{
    if (qFuzzyCompare(m_gurgitation, newGurgitation))
        return;
    m_gurgitation = newGurgitation;
    emit gurgitationChanged();
}

qint32 BreathSurvey::gating() const
{
    return m_gating;
}

void BreathSurvey::setGating(qint32 newGating)
{
    if (m_gating == newGating)
        return;
    m_gating = newGating;
    emit gatingChanged();
}

void BreathSurvey::onCatheterTrackChanged(const QSharedPointer<CatheterTrackPackage> &trackData) {
    auto pantTrack = trackData->getPant(MagnetismPant1Port);
    if (pantTrack) {
        vtkVector3d position;
        pantTrack->getPosition(position);
        if (m_profile->renovating()) {
            m_pant1Positions->InsertNextPoint(position.GetData());
            emit pant1PositionChanged();
        } else if (m_profile->pantSampling()) {
            m_breathZPosition.push_back(position.GetZ());
            if (m_breathZPosition.size() > 300) {
                checkBreathState();
            }
            disposePantTrackData(position);
        }
    }

    for(Catheter* catheter : trackData->getCatheters()) {
        if (catheter->isPant()) {
            continue;
        }
        for (const CatheterTrack& track:trackData->getTracks(catheter)) {
            if (track.status() != Halve::TrackStatus_Valid) {
                continue;
            }
            vtkVector3d position;
            track.getPosition(position);
            quint16 seatIdx = track.seat() - catheter->bseat();
            if (m_profile->renovating() && seatIdx == 0) {
                m_positions->InsertNextPoint(position.GetData());
                emit positionChanged();
            }
        }
    }
}

