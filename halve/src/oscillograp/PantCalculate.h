#ifndef PANTCALCULATE_H
#define PANTCALCULATE_H
#include "channel/ChannelData.h"

class ElectrodeNode;

class PantCalculate
{
public:
    typedef std::function<ChannelData::DataType(const ChannelData::DataType[])> Func;

    explicit PantCalculate(const ElectrodeNode* electrodeNode);

    ChannelData::DataType operator()(const ChannelData &data) const;

private:
    const ElectrodeNode* m_electrodeNode = nullptr;
};

#endif // PANTCALCULATE_H
