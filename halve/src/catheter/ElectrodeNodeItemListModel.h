#ifndef CHANNELCHARTLISTMODEL_H
#define CHANNELCHARTLISTMODEL_H

#include "catheter/ElectrodeNodeListModelBase.h"

class ElectrodeSurveyor;

class ElectrodeNodeItemListModel : public ElectrodeNodeListModelBase
{
    Q_OBJECT
public:
    explicit ElectrodeNodeItemListModel(QObject *parent = nullptr);
    Q_INVOKABLE bool exist(const ElectrodeNode* node) const;

public slots:
    void onElectrodeAdded(ElectrodeNode* node);
    void onElectrodeDeleted(ElectrodeNode* node);
    void onElectrodeDyestuffChanged(ElectrodeNode* node);

protected:
    void setElectrodeNodeSurveyor(ElectrodeNode* node, double v);

protected:
    QPointer<ElectrodeSurveyor> m_electrodeSurveyor;
};

#endif // CHANNELCHARTLISTMODEL_H
