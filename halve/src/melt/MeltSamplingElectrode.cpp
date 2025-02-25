#include "MeltSamplingElectrode.h"
#include <qjsonobject.h>
#include <QJsonArray>
#include <catheter/Catheter.h>
#include <catheter/CatheterDb.h>
#include <catheter/ElectrodeNode.h>

MeltSamplingElectrode::MeltSamplingElectrode(CatheterDb* catheterDb,QObject *parent)
    : ExposedElectrode(catheterDb, parent)
{
    QObject::connect(m_catheterDb, &CatheterDb::electrodeNodesChanged, this, &MeltSamplingElectrode::onCatheterDeleted);
    QObject::connect(m_catheterDb, &CatheterDb::electrodeChanged, this, &MeltSamplingElectrode::onCatheterElectrodeChanged);
}

void MeltSamplingElectrode::fromJson(const QJsonObject &jsonObject) {
    if (jsonObject.contains("samplingElectrode")) {
        ExposedElectrode::fromJson(jsonObject["samplingElectrode"].toArray());
    }
    if (jsonObject.contains("samplingPerception"))
        m_samplingPerception = (Halve::Perception)jsonObject["samplingPerception"].toInt();
}

QJsonValue MeltSamplingElectrode::toJson() const
{
    QJsonObject jsonObject;
    jsonObject["samplingElectrode"] = ExposedElectrode::toJson();
    jsonObject["samplingPerception"] = m_samplingPerception;
    return jsonObject;
}

Halve::Perception MeltSamplingElectrode::samplingPerception() const
{
    return m_samplingPerception;
}

void MeltSamplingElectrode::setSamplingPerception(Halve::Perception newSamplingPerception)
{
    if (m_samplingPerception == newSamplingPerception)
        return;
    m_samplingPerception = newSamplingPerception;
    emit samplingPerceptionChanged();
}

void MeltSamplingElectrode::onCatheterElectrodeNodesChanged(Catheter* catheter) {
    for (auto electrodeNode : catheter->getElectrodeNodes()) {
        if (m_electrodeNodes.contains(electrodeNode) || electrodeNode->seat() == ElectrodeNode::UNSET) {
            continue;
        }
        appendElectrodeNode(electrodeNode);
        emit added(electrodeNode);
    }
}

void MeltSamplingElectrode::onCatheterElectrodeChanged(Catheter *catheter) {
    Q_ASSERT(catheter != nullptr);
    removeCatheterElectrodeNodes(catheter);
    emit sizeChanged();
}
