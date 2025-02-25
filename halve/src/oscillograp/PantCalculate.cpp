#include "PantCalculate.h"
#include "qdebug.h"

#include <catheter/ElectrodeNode.h>

PantCalculate::PantCalculate(const ElectrodeNode* electrodeNode)
    :m_electrodeNode(electrodeNode){
}

ChannelData::DataType PantCalculate::operator()(const ChannelData &data) const {
    Q_ASSERT(m_electrodeNode != nullptr);
    if(m_electrodeNode->id() == Pant1ID) {
        return data.m_p0;
    }
    if(m_electrodeNode->id() == Pant2ID) {
        return data.m_p1;
    }
    return 0;
}
