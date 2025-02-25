#ifndef SAMPLELINE_H
#define SAMPLELINE_H

#include <QQuickItem>
#include <QPointer>

class ElectrodeNode;

class MeltSampleLine : public QQuickItem
{
    Q_OBJECT;
    Q_PROPERTY(qint32 lat READ lat WRITE setLat NOTIFY latChanged FINAL)
    Q_PROPERTY(qint32 latSeriesIndex READ latSeriesIndex WRITE setLatSeriesIndex NOTIFY latSeriesIndexChanged FINAL)
    Q_PROPERTY(qint32 voltageSeriesIndex READ voltageSeriesIndex WRITE setVoltageSeriesIndex NOTIFY voltageSeriesIndexChanged FINAL)
    Q_PROPERTY(double minVoltage READ minVoltage WRITE setMinVoltage NOTIFY minVoltageChanged FINAL)
    Q_PROPERTY(double maxVoltage READ maxVoltage WRITE setMaxVoltage NOTIFY maxVoltageChanged FINAL)
    Q_PROPERTY(bool adjust READ adjust WRITE setAdjust NOTIFY adjustChanged FINAL)
    Q_PROPERTY(quint64 rowId READ rowId  WRITE setRowId NOTIFY rowIdChanged FINAL)


public:
    explicit MeltSampleLine(QQuickItem *parent = nullptr);
    ~MeltSampleLine() = default;

    qint32 lat() const;
    void setLat(qint32 newCurrentLat);

    qint32 latDataIndex() const;
    void setLatDataIndex(qint32 newDataIndex);
    qint32 voltageDataIndex() const;
    void setVoltageDataIndex(qint32 newDataIndex);

    quint64 rowId() const;
    void setRowId(quint64 newId);

    qint32 latSeriesIndex() const;
    void setLatSeriesIndex(qint32 newLatIndex);

    qint32 voltageSeriesIndex() const;
    void setVoltageSeriesIndex(qint32 newVoltageIndex);

    double minVoltage() const;
    void setMinVoltage(double newMinVoltage);

    double maxVoltage() const;
    void setMaxVoltage(double newMaxVoltage);


    bool adjust() const;
    void setAdjust(bool newAdjust);

signals:
    void latSeriesIndexChanged();
    void voltageSeriesIndexChanged();
    void latChanged();
    void voltageChanged();

    void rowIdChanged();

    void electrodeNodeChanged();

    void latIndexChanged();

    void voltageIndexChanged();

    void minVoltageChanged();

    void maxVoltageChanged();


    void adjustChanged();

private:
    quint64 m_id = 0;
    qint32 m_lat = -1;
    qint32 m_latSeriesIndex = 0;
    qint32 m_latDataIndex = -1;
    double m_minVoltage = 0.0;
    double m_maxVoltage = 0.0;
    qint32 m_voltageSeriesIndex = 0;
    qint32 m_voltageDataIndex = -1;
    bool m_adjust = false;
};

#endif // SAMPLELINE_H
