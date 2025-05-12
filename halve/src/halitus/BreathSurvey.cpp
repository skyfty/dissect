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
// 计算翻新数据
bool BreathSurvey::computeRenovatingData() {
    // 检查数据点数量是否小于最小数据大小
    if (m_pant1Positions->GetNumberOfPoints() < MiniumDataSize || m_positions->GetNumberOfPoints() < MiniumDataSize) {
        return false; // 数据不足，返回false
    }
    auto pant1PositionSamplings = convertSamplings(m_pant1Positions, 2);

    // 存储采样值的索引
    DataIndexVector pant1PositionSamplingsIndexData;
    for (size_t i = 0; i < pant1PositionSamplings.size(); ++i) {
        pant1PositionSamplingsIndexData.push_back(std::make_pair(i, pant1PositionSamplings[i])); // 将索引和采样值配对
    }
    
    std::vector<size_t> peaks, valleys; // 存储峰值和谷值
    ys::Breath<double> breath(m_samplingRate); // 创建呼吸对象
    breath.FindPeakValley(pant1PositionSamplings.begin(), pant1PositionSamplings.end(), peaks, valleys); // 查找峰值和谷值
    if (valleys.size() < 2) {
        return false; // 如果峰值或谷值数量不足，返回false
    }

    // 处理采样数据以获取边缘呼吸数据
    std::vector<double> gatingSamplingsIndexData = pant1PositionSamplings;
    std::sort(gatingSamplingsIndexData.begin(), gatingSamplingsIndexData.end()); // 排序
    gatingSamplingsIndexData.erase(std::unique(gatingSamplingsIndexData.begin(), gatingSamplingsIndexData.end(),[](double l , double r){
       int nl = l * 10;
       int nr = r * 10;
       return nl == nr; // 去重
    }), gatingSamplingsIndexData.end());
    
    std::vector<double> edgeGatingBreathData;
    double breathThresholdRadio = m_breathGatingRatio / 100; // 计算呼吸阈值比例
    auto gatingSamplingPointCount = gatingSamplingsIndexData.size() * breathThresholdRadio; // 计算边缘采样点数量
    auto gatingminmax = std::minmax_element(gatingSamplingsIndexData.begin(), gatingSamplingsIndexData.begin() + gatingSamplingPointCount);
    m_breathOptions->setGatingValid(std::make_pair(*(gatingminmax.first), *(gatingminmax.second))); // 设置有效的呼吸阈值

    // 计算呼吸的谷值和峰值
    auto minmaxval = std::minmax_element(pant1PositionSamplings.begin(), pant1PositionSamplings.end());
    m_breathOptions->setBreatheValley(*(minmaxval.first)); // 设置呼吸谷值
    m_breathOptions->setBreathePeak(*(minmaxval.second)); // 设置呼吸峰值
    m_breathOptions->setBreatheBase((m_breathOptions->breathePeak() + m_breathOptions->breatheValley()) / 2); // 计算呼吸基线
    auto breatheLimit = std::abs(m_breathOptions->breathePeak() - m_breathOptions->breatheValley());
    m_breathOptions->setBreatheLimit(breatheLimit); // 设置呼吸限制
    onBreathBoundaryRatioChanged(); // 通知呼吸边界比例已改变

    double peakAvg = 0; // 存储峰值平均值
    for (size_t i = 0; i < peaks.size(); ++i) {
        peakAvg += pant1PositionSamplings[peaks[i]]; // 计算峰值的平均值
    }
    peakAvg /= peaks.size(); // 计算平均值
    peaks.erase(std::remove_if(peaks.begin(), peaks.end(), [&pant1PositionSamplings, peakAvg](size_t idx) {
        auto validPeakValue = peakAvg - peakAvg * 5 / 100; // 计算有效峰值
        return pant1PositionSamplings[idx] > validPeakValue; // 移除低于有效峰值的峰
    }), peaks.end());
    if (peaks.empty()) {
        return false; // 如果没有有效峰值，返回false
    }
    
    size_t breathCount = peaks.size() - 1; // 计算呼吸次数
    size_t breathPointCount = 0; // 存储呼吸点数量
    for (size_t i = 0; i < breathCount; ++i) {
        breathPointCount += (peaks[i + 1] - peaks[i]);
    }
    breathPointCount /= breathCount; // 计算平均呼吸点数
    if (breathPointCount >= pant1PositionSamplingsIndexData.size()) {
      return false; // 如果呼吸点数超过采样数据，返回false
    }

    vtkVector3d compensateBasePoint{}; // 存储补偿基点
    for (size_t i = 0; i < breathCount; ++i) {
        DataIndexVector breathIndexData;
        auto beginIter = pant1PositionSamplingsIndexData.begin() + i * breathPointCount; // 获取当前呼吸的起始迭代器
        auto endIter = beginIter + breathPointCount; // 获取当前呼吸的结束迭代器
        std::copy(beginIter, endIter, std::back_inserter(breathIndexData)); // 复制数据
        std::sort(breathIndexData.begin(), breathIndexData.end(), [](const auto& l, const auto& r) {
            return l.second < r.second; // 按照值排序
        });
        DataIndexVector edgeBreathData;
        auto samplingPointCount = breathIndexData.size() * breathThresholdRadio; // 计算边缘采样点数量
        std::copy(breathIndexData.begin(), breathIndexData.begin() + samplingPointCount, std::back_inserter(edgeBreathData)); // 复制边缘数据

        vtkSmartPointer<vtkPoints> edgeSamplingPoints = vtkSmartPointer<vtkPoints>::New(); // 创建边缘采样点
        for (auto& data : edgeBreathData) {
            edgeSamplingPoints->InsertNextPoint(m_positions->GetPoint(data.first)); // 插入边缘采样点
        }
        vtkVector3d edgetBreathBasePoint{}; // 存储边缘呼吸基点
        for (int i = 0; i < edgeSamplingPoints->GetNumberOfPoints(); ++i) {
            vtkMath::Add(edgeSamplingPoints->GetPoint(i), edgetBreathBasePoint, edgetBreathBasePoint); // 计算边缘呼吸基点
        }
        vtkMath::MultiplyScalar(edgetBreathBasePoint.GetData(), 1.0 / edgeSamplingPoints->GetNumberOfPoints()); // 计算平均值
        vtkMath::Add(edgetBreathBasePoint, compensateBasePoint, compensateBasePoint); // 更新补偿基点
    }
    vtkMath::MultiplyScalar(compensateBasePoint.GetData(), 1.0 / breathCount); // 计算最终补偿基点
    m_breathOptions->setCompensateBasePoint(compensateBasePoint); // 设置补偿基点

    vtkSmartPointer<vtkPoints> compensatePoints = vtkSmartPointer<vtkPoints>::New(); // 创建补偿点
    vtkSmartPointer<vtkPoints> samplingPoints = arrangeSamplings(m_positions); // 获取采样点
    auto numOfPoint = samplingPoints->GetNumberOfPoints(); // 获取采样点数量
    for (auto i = 0; i < numOfPoint / 2; ++i) {
        vtkVector3d pos{ 0,0,0 }, pos2{ 0,0,0 }; // 创建两个位置向量
        samplingPoints->GetPoint(i, pos.GetData()); // 获取第i个采样点
        samplingPoints->GetPoint(numOfPoint - 1 - i, pos2.GetData()); // 获取对称的采样点
        vtkMath::Add(pos, pos2, pos); // 计算两个点的和
        vtkMath::MultiplyScalar(pos.GetData(), 0.5); // 计算平均值
        compensatePoints->InsertNextPoint(pos.GetData()); // 插入补偿点
    }
    setSplinePoints(compensatePoints); // 设置样条点
    m_breathOptions->setCompensatePoints(compensatePoints); // 设置补偿点
    return compensatePoints->GetNumberOfPoints() >= 0 && m_breathOptions->breatheLimit() >= m_breathSamplingValidLimit; // 返回是否满足条件
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
        vtkVector3d position{};
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
    if (m_profile->renovating()) {
        for (Catheter* catheter : trackData->getCatheters()) {
            if (catheter->isPant()) {
                continue;
            }
            for (const CatheterTrack& track : trackData->getTracks(catheter)) {
                if (track.status() != Halve::TrackStatus_Valid) {
                    continue;
                }
                vtkVector3d position{};
                track.getPosition(position);
                quint16 seatIdx = track.seat() - catheter->bseat();
                if (seatIdx == 0) {
                    m_positions->InsertNextPoint(position.GetData());
                    emit positionChanged();
                }
            }
        }
    }
}

