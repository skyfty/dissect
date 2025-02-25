#include <QDateTime>
#include "ChannelDataCalculate.h"

#include <catheter/ElectrodeNode.h>

typedef QHash<QString, std::function<ChannelData::DataType(const ChannelData::DataType[])>> BodySurfaceFunctionList;

static BodySurfaceFunctionList BodySurfaceFunction = {
    {"bodysurface.bodysurface.ecg1", [](const ChannelData::DataType da[]){
         return da[0];
     }},
    {"bodysurface.bodysurface.ecg2", [](const ChannelData::DataType da[]){
         return da[1];
     }},
    {"bodysurface.bodysurface.ecg3", [](const ChannelData::DataType da[]){
         return da[1] - da[0];
     }},
    {"bodysurface.bodysurface.ecgavr", [](const ChannelData::DataType da[]){
         return 0.5 * -(da[0] + da[1]);
     }},
    {"bodysurface.bodysurface.ecgavl", [](const ChannelData::DataType da[]){
         return 0.5 * (2 * da[0] - da[1]);
     }},
    {"bodysurface.bodysurface.ecgavf", [](const ChannelData::DataType da[]){
         return 0.5 * (2 * da[1] - da[0]);
     }},
    {"bodysurface.bodysurface.ecgv1", [](const ChannelData::DataType da[]){
         return 0.333 * (3 * da[2] - da[0] - da[1]);
     }},
    {"bodysurface.bodysurface.ecgv2", [](const ChannelData::DataType da[]){
         return 0.333 * (3 * da[3] - da[0] - da[1]);
     }},
    {"bodysurface.bodysurface.ecgv3", [](const ChannelData::DataType da[]){
         return 0.333 * (3 * da[4] - da[0] - da[1]);
     }},
    {"bodysurface.bodysurface.ecgv4", [](const ChannelData::DataType da[]){
         return 0.333 * (3 * da[5] - da[0] - da[1]);
     }},
    {"bodysurface.bodysurface.ecgv5", [](const ChannelData::DataType da[]){
         return 0.333 * (3 * da[6] - da[0] - da[1]);
     }},
    {"bodysurface.bodysurface.ecgv6", [](const ChannelData::DataType da[]){
         return 0.333 * (3 * da[7] - da[0] - da[1]);
     }},
};

ChannelDataCalculate::ChannelDataCalculate(const ElectrodeNode* electrodeNode)
    :m_electrodeNode(electrodeNode){
    if (BodySurfaceFunction.contains(electrodeNode->id())) {
        m_bodySurfaceFunc = BodySurfaceFunction[electrodeNode->id()];
    }
}

ChannelData::DataType ChannelDataCalculate::operator()(const ChannelData &data) const {
    return reckon(&data, m_electrodeNode);
}

ChannelData::DataType ChannelDataCalculate::reckon(const ChannelData *data, const ElectrodeNode *electrodeNode) const
{
    Q_ASSERT(data != nullptr);
    Q_ASSERT(electrodeNode != nullptr);

    if (m_bodySurfaceFunc) {
        return (*m_bodySurfaceFunc)(data->eda);
    }
    return reckon(data->da, electrodeNode);
}

ChannelData::DataType ChannelDataCalculate::reckon(const ChannelData::DataType data[],const ElectrodeNode* electrodeNode) const {
    Q_ASSERT(data != nullptr);
    Q_ASSERT(electrodeNode != nullptr);

    switch(electrodeNode->type()) {
    case Halve::SINGLE: {
        return data[electrodeNode->d1()];
    }
    default: {
        return data[electrodeNode->d1()] - data[electrodeNode->d2()];
    }
    }
}
