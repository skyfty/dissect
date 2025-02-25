#ifndef MELTELECTRODE_H
#define MELTELECTRODE_H

#include "profile/ExposedElectrode.h"
#include <HalveType.h>

class CatheterDb;
class ElectrodeNode;
class QXYSeries;
class Catheter;

class MeltSamplingElectrode : public ExposedElectrode
{
    Q_OBJECT
    Q_PROPERTY(Halve::Perception samplingPerception READ samplingPerception WRITE setSamplingPerception NOTIFY samplingPerceptionChanged FINAL)

public:
    MeltSamplingElectrode(CatheterDb* catheterDb,QObject *parent = nullptr);
    void fromJson(const QJsonObject &v);
    QJsonValue toJson()  const;

    Halve::Perception samplingPerception() const;
    void setSamplingPerception(Halve::Perception newSamplingPerception);

signals:
    void samplingPerceptionChanged();

public slots:
    void onCatheterElectrodeChanged(Catheter *catheter);
    void onCatheterElectrodeNodesChanged(Catheter* catheter);
private:
    void resetSeriesSize(QXYSeries *xyseries);
private:
    Halve::Perception m_samplingPerception = Halve::Max;
};

#endif // MELTELECTRODE_H
