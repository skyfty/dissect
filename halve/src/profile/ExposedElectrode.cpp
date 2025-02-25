#include "ExposedElectrode.h"
#include <qjsonobject.h>
#include <QJsonArray>
#include <catheter/Catheter.h>
#include <catheter/CatheterDb.h>
#include <catheter/ElectrodeNode.h>

ExposedElectrode::ExposedElectrode(CatheterDb* catheterDb,QObject *parent)
    : QObject{parent},m_catheterDb(catheterDb)
{
    QObject::connect(m_catheterDb, &CatheterDb::deleted, this, &ExposedElectrode::onCatheterDeleted);
    QObject::connect(m_catheterDb, &CatheterDb::added, this, &ExposedElectrode::onCatheterAdded);
    QObject::connect(m_catheterDb, &CatheterDb::employChanged, this, &ExposedElectrode::onCatheterEmployChanged);
    QObject::connect(m_catheterDb, &CatheterDb::typeChanged, this, &ExposedElectrode::onCatheterTypeChanged);
    QObject::connect(m_catheterDb, &CatheterDb::dyestuffChanged, this, &ExposedElectrode::onCatheterDyestuffChanged);
}

void ExposedElectrode::fromJson(const QJsonArray &jsonArray) {
    m_electrodeNodes.clear();
    for(auto json:jsonArray) {
        ElectrodeNode *electrodeNode = m_catheterDb->getElectrodeNode(json.toString());
        if (electrodeNode != nullptr) {
            m_electrodeNodes.append(electrodeNode);
        }
    }
}

QJsonValue ExposedElectrode::toJson()  const  {
    QJsonArray json;
    for(ElectrodeNode *node: m_electrodeNodes) {
        json.append(node->id());
    }
    return json;
}
QList<ElectrodeNode *> ExposedElectrode::getDatas() const
{
    return m_electrodeNodes;
}

void ExposedElectrode::add(ElectrodeNode *node)
{
    Q_ASSERT(node != nullptr);
    if (m_electrodeNodes.contains(node)) {
        return;
    }
    appendElectrodeNode(node);
    emit added(node);
    emit sizeChanged();
}

void ExposedElectrode::remove(ElectrodeNode* node) {
    Q_ASSERT(node != nullptr);
    int row = m_electrodeNodes.indexOf(node);
    if (row != -1) {
        m_electrodeNodes.removeAt(row);
        emit deleted(node);
        emit sizeChanged();
    }
}

void ExposedElectrode::add(const QString& id) {
    ElectrodeNode* electrodeNode = m_catheterDb->getElectrodeNode(id);
    if (electrodeNode != nullptr) {
        add(electrodeNode);
    }
}

void ExposedElectrode::remove(const QString& id) {
    ElectrodeNode* electrodeNode = m_catheterDb->getElectrodeNode(id);
    if (electrodeNode != nullptr) {
        remove(electrodeNode);
    }
}

ElectrodeNode *ExposedElectrode::get(const QString &id) {
    for(ElectrodeNode *node: m_electrodeNodes) {
        if (node->id() == id) {
            return node;
        }
    }
    return nullptr;
}

bool ExposedElectrode::exist(const ElectrodeNode *node)
{
    return node != nullptr && m_electrodeNodes.contains(node);
}

bool ExposedElectrode::exist(const QString &id)
{
    return get(id) != nullptr;
}

qint32 ExposedElectrode::indexOfName(const QString &name)
{
    for(qint32 i = 0; i < m_electrodeNodes.size(); ++i) {
        if (m_electrodeNodes[i]->name() == name) {
            return i;
        }
    }
    return -1;
}

void ExposedElectrode::onCatheterAdded(Catheter* catheter) {
    Q_ASSERT(catheter != nullptr);
    emit sizeChanged();
}

void ExposedElectrode::removeCatheterElectrodeNodes(Catheter* catheter) {
    Q_ASSERT(catheter != nullptr);    
    m_electrodeNodes.removeIf([this, catheter](ElectrodeNode* node) {
        bool ret = catheter->isBelongTo(node);
        if (ret) {
            emit deleted(node);
        }
        return ret;
    });
}

void ExposedElectrode::appendElectrodeNode(ElectrodeNode *node) {
    QObject::connect(node, &ElectrodeNode::dyestuffChanged, this, std::bind(&ExposedElectrode::dyestuffChanged, this, node));
    m_electrodeNodes.append(node);
}

void ExposedElectrode::onCatheterDeleted(Catheter* catheter) {
    removeCatheterElectrodeNodes(catheter);
    emit sizeChanged();
}

void ExposedElectrode::onCatheterEmployChanged(Catheter *catheter)
{
    if (catheter->employ()) {
        return;
    }
    onCatheterDeleted(catheter);
}


qint32 ExposedElectrode::size() const
{
    return m_electrodeNodes.size();
}

ElectrodeNode *ExposedElectrode::get(int index)
{
    if (m_electrodeNodes.empty() || index > m_electrodeNodes.size()) {
        return nullptr;
    }
    return m_electrodeNodes[index];
}

void ExposedElectrode::onCatheterTypeChanged(Catheter* catheter) {
    for (auto electrodeNode : catheter->getElectrodeNodes()) {
        if (m_electrodeNodes.contains(electrodeNode)) {
            emit typeChanged(electrodeNode);
        }
    }
}

void ExposedElectrode::onCatheterDyestuffChanged(Catheter* catheter) {
    for (auto electrodeNode : catheter->getElectrodeNodes()) {
        if (m_electrodeNodes.contains(electrodeNode)) {
            emit dyestuffChanged(electrodeNode);
        }
    }
}

