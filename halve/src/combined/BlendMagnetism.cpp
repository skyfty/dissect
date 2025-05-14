#include "combined/BlendMagnetism.h"
#include <profile/Profile.h>
#include <channel/ChannelTrackData.h>
#include <catheter/Catheter.h>
#include <vtkMath.h>

BlendMagnetism::BlendMagnetism(Profile* profile, Catheter* catheter, QObject* parent)
    : Blend(profile, catheter, parent)
{
    m_magnetism = catheter->catheterMagnetism();
    m_elecIdentify = std::make_shared<ys::ElecIdentify>();
    auto matrix = m_magnetism->matrix();
    m_trained = matrix != Matrix3x4d::Zero();
    m_elecIdentify->SetE2W(matrix);
}

bool BlendMagnetism::train(QVector<ys::InputParameter>& trainDatas) {
    return m_elecIdentify->Train(trainDatas.data(), (int)trainDatas.size());
}

void BlendMagnetism::timerEvent(QTimerEvent*) {
    train(m_trainDatas);
    m_trainDatas.clear();
}

std::pair<quint16, quint16> BlendMagnetism::getMagnetismSeat() const {
    auto bseat = m_catheter->bseat();
    return std::make_pair(bseat + m_magnetism->consult(), bseat + m_magnetism->target());
}

CatheterMagnetism *BlendMagnetism::getMagnetism() const
{
    return m_magnetism;
}

QList<TrackData> BlendMagnetism::process(const ChannelTrackData& dataBuffer) {
    QList<TrackData> trackDatas;
    auto [consultSeat, targetSeat] = getMagnetismSeat();
    quint16 port = m_catheter->port();
    trackDatas = convert_20250319(port, consultSeat, targetSeat, dataBuffer);
    return trackDatas;
}

QList<TrackData> BlendMagnetism::convert_20250319(quint16 port, quint16 consultSeat, quint16 targetSeat, const ChannelTrackData &dataBuffer)
{
    if (m_catheter == nullptr || !m_trained)
        return QList<TrackData>();
    return convert(dataBuffer, getElecIdentify().get());
}

bool BlendMagnetism::matrixPerfect(const Matrix3x4d &matrix)
{
    std::cout << matrix << std::endl << std::endl;

    // 方法：
    //   每列中所有元素的3倍小于最大绝对值的元素
    //   3列中绝对值最大值行号不能相等
    //   3列中绝对值最大值之间差别不能超过1.5倍
    double elemMax[3]{0};
    int maxRowIndex[3]{0};
    for (int col = 0; col < 3; ++col) {
        double colMax = std::numeric_limits<double>::lowest();
        int indexMax = -1;
        for (int row = 0; row < 3; ++row) {
            if (std::abs(matrix(row, col)) > colMax) {
                colMax = std::abs(matrix(row, col));
                indexMax = row;
            }
        }

        elemMax[col] = colMax;
        maxRowIndex[col] = indexMax;

        // 每列元素3倍 小于 最大元素，否则结果是不好的
        for (int row = 0; row < 3; ++row) {
            if (std::abs(matrix(row, col)) * 3.0 >= std::abs(colMax))
                return false;
        }
    }

    if (maxRowIndex[0] == maxRowIndex[1] || maxRowIndex[0] == maxRowIndex[2] || maxRowIndex[1] == maxRowIndex[2])
        return false;

    std::sort(elemMax, elemMax + 3);
    if (elemMax[0] * 2 < elemMax[2] || elemMax[1] * 2 < elemMax[2])
        return false;

    return true;
}

void BlendMagnetism::appendTrainData(const ChannelTrackData &dataBuffer) {
    auto [consultSeat, targetSeat] = getMagnetismSeat();
    auto ip = makeInputParameter(dataBuffer, m_catheter->port(), consultSeat, targetSeat);
    if (m_magnetism)
        ip.mLocalRef << 0, 0, m_magnetism->consultDistance();
    m_trainDatas.append(ip);

    // 每添加80个数据计算1次。
    // 如果训练不成功，如果数据量<1200，不处理。
    //               如果数据量>=1200, 清空缓冲。
    // 如果训练成功，结果好，提示训练ok。              --是否不再向缓冲区中加入数据？
    //             结果坏，如果数据量<1200，不处理。
    //                     数据量>=1200，随机抽取300个保留，其它删除。
    if (m_trainDatas.size() % 40 == 0)
    {
        bool ok = train(m_trainDatas); qDebug() << ok << m_trainDatas.size();
        if (!ok)
        {
            if (m_trainDatas.size() >= 1200)
                m_trainDatas.clear();
            return;
        }

        const auto& matrix = m_elecIdentify->GetE2W();
        ok = matrixPerfect(matrix); qDebug() << ok << m_trainDatas.size();
        if (!ok)
        {
            if (m_trainDatas.size() >= 1200)
                m_trainDatas.erase(m_trainDatas.begin(), m_trainDatas.end() - 300);
            return;
        }

        m_profile->alarmDb()->add(Halve::AN_MAGNETIC_ELECTRIC_TRAINNING_OK);
    }
}

std::shared_ptr<ys::ElecIdentify> BlendMagnetism::getElecIdentify() {
    return m_elecIdentify;
}

bool BlendMagnetism::trained() const {
    return m_trained;
}

void BlendMagnetism::startTrainTimer(std::chrono::milliseconds interval) {
    if (m_trained) {
        if (m_timerId != -1) {
            killTimer(m_timerId);
            m_timerId = -1;
        }
        if (interval.count() != 0) {
            //m_timerId = startTimer(interval);
        }
    }
    m_trainInterval = interval;
}
void BlendMagnetism::startTrain() {
    m_trained = false;
    m_trainDatas.clear();
}

void BlendMagnetism::finishTrain() {
    m_profile->alarmDb()->remove(Halve::AN_MAGNETIC_ELECTRIC_TRAINNING_OK);
    m_trained = train(m_trainDatas);
    if (m_trained) {
        m_timerId = -1;
        if (m_trainInterval.count() != 0) {
            //m_timerId = startTimer(m_trainInterval);
        }
        m_magnetism->setMatrix(m_elecIdentify->GetE2W());
    }
    m_trainDatas.clear();
}
