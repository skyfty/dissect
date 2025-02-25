#ifndef CHANNELDATACALCULATE_H
#define CHANNELDATACALCULATE_H

#include "channel/ChannelData.h"

class ElectrodeNode;

class ChannelDataCalculate
{
    typedef std::function<ChannelData::DataType(const ChannelData::DataType[])> BodySurfaceFunc;

public:
    explicit ChannelDataCalculate(const ElectrodeNode* electrodeNode);

    ChannelData::DataType operator()(const ChannelData &data) const;
    ChannelData::DataType reckon(const ChannelData *data,const ElectrodeNode* electrodeNode) const;
    ChannelData::DataType reckon(const ChannelData::DataType data[],const ElectrodeNode* electrodeNode) const;

private:
    const ElectrodeNode* m_electrodeNode = nullptr;
    std::optional<BodySurfaceFunc> m_bodySurfaceFunc;
};


#endif // CHANNELDATACALCULATE_H
