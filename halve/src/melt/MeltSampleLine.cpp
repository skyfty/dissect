#include "MeltSampleLine.h"

MeltSampleLine::MeltSampleLine(QQuickItem *parent)
    :QQuickItem(parent)
{

}

qint32 MeltSampleLine::lat() const
{
    return m_lat;
}

void MeltSampleLine::setLat(qint32 newCurrentLat)
{
    if (m_lat == newCurrentLat)
        return;
    m_lat = newCurrentLat;
    emit latChanged();
}

qint32 MeltSampleLine::latDataIndex() const
{
    return m_latDataIndex;
}

void MeltSampleLine::setLatDataIndex(qint32 newDataIndex)
{
    m_latDataIndex = newDataIndex;
}

quint64 MeltSampleLine::rowId() const
{
    return m_id;
}

qint32 MeltSampleLine::voltageDataIndex() const {
    return m_voltageDataIndex;
}

void MeltSampleLine::setVoltageDataIndex(qint32 newDataIndex) {
    m_voltageDataIndex = newDataIndex;
}

void MeltSampleLine::setRowId(quint64 newId)
{
    if (newId == m_id) {
        return;
    }
    m_id = newId;
    emit rowIdChanged();
}

qint32 MeltSampleLine::latSeriesIndex() const
{
    return m_latSeriesIndex;
}

void MeltSampleLine::setLatSeriesIndex(qint32 newLatIndex)
{
    if (m_latSeriesIndex == newLatIndex)
        return;
    m_latSeriesIndex = newLatIndex;
    emit latSeriesIndexChanged();
}

qint32 MeltSampleLine::voltageSeriesIndex() const
{
    return m_voltageSeriesIndex;
}

void MeltSampleLine::setVoltageSeriesIndex(qint32 newVoltageIndex)
{
    if (m_voltageSeriesIndex == newVoltageIndex)
        return;
    m_voltageSeriesIndex = newVoltageIndex;
    emit voltageSeriesIndexChanged();
}

double MeltSampleLine::minVoltage() const
{
    return m_minVoltage;
}

void MeltSampleLine::setMinVoltage(double newMinVoltage)
{
    if (qFuzzyCompare(m_minVoltage, newMinVoltage))
        return;
    m_minVoltage = newMinVoltage;
    emit minVoltageChanged();
}

double MeltSampleLine::maxVoltage() const
{
    return m_maxVoltage;
}

void MeltSampleLine::setMaxVoltage(double newMaxVoltage)
{
    if (qFuzzyCompare(m_maxVoltage, newMaxVoltage))
        return;
    m_maxVoltage = newMaxVoltage;
    emit maxVoltageChanged();
}

bool MeltSampleLine::adjust() const
{
    return m_adjust;
}

void MeltSampleLine::setAdjust(bool newAdjust)
{
    if (m_adjust == newAdjust)
        return;
    m_adjust = newAdjust;
    emit adjustChanged();
}
