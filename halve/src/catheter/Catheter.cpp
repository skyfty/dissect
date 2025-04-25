#include "Catheter.h"
#include "catheter/CatheterMesh.h"
#include "catheter/ElectrodeNode.h"
#include <QJsonDocument>
#include <qjsonobject.h>
#include <QJsonArray>
#include <vtkBox.h>
#include <vtkMath.h>
#include <QFile>
#include "utility/VtkUtil.h"
#include <utility/Thread.h>

Catheter::Catheter(QObject *parent)
    :QObject(parent)
{
    m_catheterMagnetism = new CatheterMagnetism(this);
}

void Catheter::fromJson(const QJsonObject &json) {

    if (json.contains("id")) {
        m_id = json["id"].toString();
        m_isPant = m_id == PantCatheterID;
    }
    if (json.contains("name"))
        m_name = json["name"].toString();
    if (json.contains("amount"))
        m_amount = json["amount"].toInt();
    if (json.contains("bseat"))
        m_bseat = json["bseat"].toInt();
    if (json.contains("type"))
        m_type = json["type"].toString();
    if (json.contains("manufacturer"))
        m_manufacturer = json["manufacturer"].toString();
    if (json.contains("version"))
        m_version = json["version"].toString();
    if (json.contains("gap")) {
        auto gapJson = json["gap"];
        if (gapJson.isArray()) {
            for(const auto v:json["gap"].toArray()) {
                m_gap.append(v.toInt());
            }
        } else {
            m_gap.resize(m_amount - 1, gapJson.toInt());
        }
    }
    if (json.contains("fr"))
        m_fr = json["fr"].toString();
    if (json.contains("diameter"))
        m_diameter = json["diameter"].toDouble();
    if (json.contains("far"))
        m_far = json["far"].toDouble();
    if (json.contains("builtin"))
        m_builtin = json["builtin"].toBool();
    if (json.contains("destroyed"))
        m_destroyed = json["destroyed"].toBool();
    if (json.contains("employ"))
        m_employ = json["employ"].toBool();
    if (json.contains("dyestuff"))
        m_dyestuff = QColor::fromString(json["dyestuff"].toString());
    if (json.contains("multiPassEnable"))
        m_multiPassEnable = json["multiPassEnable"].toBool();
    if (json.contains("multiLowPass"))
        m_multiLowPass = json["multiLowPass"].toDouble();
    if (json.contains("multiHeighPass"))
        m_multiHeighPass = json["multiHeighPass"].toDouble();
    if (json.contains("singlePassEnable"))
        m_singlePassEnable = json["singlePassEnable"].toBool();
    if (json.contains("singleLowPass"))
        m_singleLowPass = json["singleLowPass"].toDouble();
    if (json.contains("singleHeighPass"))
        m_singleHeighPass = json["singleHeighPass"].toDouble();
    if (json.contains("showPassData"))
        m_showPassData = json["showPassData"].toBool();
    if (json.contains("electrode"))
        m_electrode = (Halve::ElectrodeType)json["electrode"].toInt();
    if (json.contains("port"))
        m_port = json["port"].toInt();
    if (json.contains("centrality"))
        m_centrality = json["centrality"].toInt();
    if (json.contains("magnetismOption")) {
        catheterMagnetism()->fromJson(json["magnetismOption"].toObject());
    }
    if (json.contains("mesh")) {
        m_meshName = json["mesh"].toString();
    }

    if (json.contains("apparent"))
        m_apparent = json["apparent"].toBool();
    if (json.contains("prefab"))
        m_prefab = json["prefab"].toBool();
    if (json.contains("electrodeLength"))
        m_electrodeLength = json["electrodeLength"].toDouble();

    if (json.contains("pair")) {
        for(auto item : json["pair"].toArray()) {
            ElectrodeNode *node = new ElectrodeNode(this, this);
            node->fromJson(item.toObject());
            m_pairNodes.push_back(node);
        }
    }
    if (json.contains("both")) {
        for(auto item : json["both"].toArray()) {
            ElectrodeNode *node = new ElectrodeNode(this, this);
            node->fromJson(item.toObject());
            m_bothNodes.push_back(node);
        }
    }
    if (json.contains("single")) {
        for(auto item : json["single"].toArray()) {
            ElectrodeNode *node = new ElectrodeNode(this, this);
            node->fromJson(item.toObject());
            m_singleNodes.push_back(node);
        }
    }
}

void Catheter::toJson(QJsonObject &json) const {
    json["id"] = m_id;
    json["name"] = m_name;
    json["destroyed"] = m_destroyed;
    json["amount"] = m_amount;
    json["bseat"] = m_bseat;
    json["dyestuff"] = m_dyestuff.name();
    json["type"] = m_type;
    json["manufacturer"] = m_manufacturer;
    json["version"] = m_version;
    QJsonArray gapJson;
    for(quint16 v:m_gap) {
        gapJson.append(v);
    }
    json["gap"] = gapJson;
    json["fr"] = m_fr;
    json["diameter"] = m_diameter;
    json["far"] = m_far;
    json["builtin"] = m_builtin;
    json["employ"] = m_employ;
    json["dyestuff"] = m_dyestuff.name();
    json["multiPassEnable"] = m_multiPassEnable;
    json["multiLowPass"] = m_multiLowPass;
    json["multiHeighPass"] = m_multiHeighPass;
    json["singlePassEnable"] = m_singlePassEnable;
    json["singleLowPass"] = m_singleLowPass;
    json["singleHeighPass"] = m_singleHeighPass;
    json["showPassData"] = m_showPassData;
    json["electrode"] = m_electrode;
    json["centrality"] = m_centrality;
    json["port"] = m_port;
    if (m_catheterMagnetism != nullptr) {
        QJsonObject magnetismOption;
        m_catheterMagnetism->toJson(magnetismOption);
        json["magnetismOption"] = magnetismOption;
    }

    QJsonObject catheterMouldJson;
    json["mesh"] = m_meshName;
    json["apparent"] = m_apparent;
    json["prefab"] = m_prefab;
    json["electrodeLength"] = m_electrodeLength;
    {
        QJsonArray jsonArray;
        for(auto item : m_pairNodes) {
            QJsonObject json;
            item->toJson(json);
            jsonArray.append(json);
        }
        json["pair"] = jsonArray;
    }

    {
        QJsonArray jsonArray;
        for(auto item : m_bothNodes) {
            QJsonObject json;
            item->toJson(json);
            jsonArray.append(json);
        }
        json["both"] = jsonArray;
    }

    {
        QJsonArray jsonArray;
        for(auto item : m_singleNodes) {
            QJsonObject json;
            item->toJson(json);
            jsonArray.append(json);
        }
        json["single"] = jsonArray;
    }

}
quint16 Catheter::bseat() const
{
    return m_bseat;
}


void Catheter::setBseat(quint16 newBseat)
{
    if (m_bseat == newBseat)
        return;
    m_bseat = newBseat;
    emit bseatChanged();
}

CatheterMesh *Catheter::mesh() {
    if (m_mesh == nullptr) {
        Q_ASSERT(Thread::currentlyOn(Thread::UI));
        m_mesh = new CatheterMesh(this);
    }
    return m_mesh;
}


quint16 Catheter::centrality() const
{
    return m_centrality;
}

void Catheter::setCentrality(quint16 newCentrality)
{
    if (m_centrality == newCentrality)
        return;
    m_centrality = newCentrality;
    emit centralityChanged();
}

bool Catheter::valid() const
{
    return m_valid;
}

void Catheter::setValid(bool newValid)
{
    if (m_valid == newValid)
        return;
    m_valid = newValid;
    emit validChanged();
}

bool Catheter::isPant() const
{
    return m_isPant;
}

bool Catheter::magnetism() const
{
    return catheterMagnetism()->magnetism();
}

void Catheter::setMagnetism(bool newMagnetism)
{
    if (magnetism() == newMagnetism)
        return;
    catheterMagnetism()->setMagnetism(newMagnetism);
    emit magnetismChanged();
}

bool Catheter::isBodySurface() const
{
    return m_id == BudySurfaceCatheterID;
}

void Catheter::setName(const QString &v) {
    if (m_name != v) {
        m_name = v.trimmed();
        emit nameChanged();
    }
}
QString Catheter::getName() const {
    return m_name;
}

void Catheter::setAmount(int v) {
    if (m_amount != v) {
        m_amount = v;
        emit amountChanged();
    }
}
int Catheter::getAmount() const {
    return m_amount;

}

void Catheter::setDiameter(double v) {
    if (m_diameter != v) {
        m_diameter = v;
        emit diameterChanged();
    }
}
double Catheter::getDiameter() const {
    return m_diameter;

}

void Catheter::setFar(double v) {
    if (m_far != v) {
        m_far = v;
        emit farChanged();
    }
}
double Catheter::getFar() const {
    return m_far;

}

void Catheter::setType(const QString &v) {
    if (m_type != v) {
        m_type = v;
        resetElectrodeNames();
        emit typeChanged();
    }
}
QString Catheter::getType() const {
    return m_type;

}

void Catheter::setManufacturer(const QString& v) {
    if (m_manufacturer != v) {
        m_manufacturer = v;
        emit manufacturerChanged();
    }
}
QString Catheter::getManufacturer() const {
    return m_manufacturer;

}

void Catheter::setGap(const QList<quint16>& v) {
    Q_ASSERT(v.size() == m_amount -1);
    if (m_gap != v) {
        m_gap = v;
        if (m_catheterMould != nullptr) {
            m_catheterMould->load(m_meshName, m_gap, m_electrodeLength);
        }
        emit gapChanged();
    }
}
const QList<quint16>& Catheter::getGap() const {
    return m_gap;
}

void Catheter::setVersion(const QString& v) {
    if (m_version != v) {
        m_version = v;
        emit versionChanged();
    }
}
QString Catheter::getVersion() const {
    return m_version;

}

void Catheter::setFr(const QString& v) {
    if (m_fr != v) {
        m_fr = v;
        emit frChanged();
    }
}
QString Catheter::getFr() const {
    return m_fr;

}

QColor Catheter::getDyestuff() const {
    return m_dyestuff;
}


vtkColor3ub Catheter::getDyestuff3ub() const {
    return vtkutil::qColorToVtkColor3ub(m_dyestuff);
}
void Catheter::setDyestuff(QColor v) {
    QColor orgDyestuff = m_dyestuff;
    if (m_dyestuff != v) {
        m_dyestuff = v;
        resetElectrodeDyestuff(orgDyestuff);
        emit dyestuffChanged();
    }
}

bool Catheter::getMultiPassEnable() const {
    return m_multiPassEnable;
}

void Catheter::setMultiPassEnable(bool v) {
    if (m_multiPassEnable != v) {
        m_multiPassEnable = v;
        emit multiPassEnableChanged();
    }
}

double Catheter::getMultiLowPass() const {
    return m_multiLowPass;
}

void Catheter::setMultiLowPass(double v) {
    if (m_multiLowPass != v) {
        m_multiLowPass = v;
        emit multiLowPassChanged();
    }
}

double Catheter::getMultiHeighPass() const {
    return m_multiHeighPass;
}

void Catheter::setMultiHeighPass(double v) {
    if (m_multiHeighPass != v) {
        m_multiHeighPass = v;
        emit multiHeighPassChanged();
    }
}

bool Catheter::getSinglePassEnable() const {
    return m_singlePassEnable;
}

void Catheter::setSinglePassEnable(bool v) {
    if (m_singlePassEnable != v) {
        m_singlePassEnable = v;
        emit singlePassEnableChanged();
    }
}


double Catheter::getSingleLowPass() const {
    return m_singleLowPass;
}

void Catheter::setSingleLowPass(double v) {
    if (m_singleLowPass != v) {
        m_singleLowPass = v;
        emit singleLowPassChanged();
    }
}

double Catheter::getSingleHeighPass() const {
    return m_singleHeighPass;
}

void Catheter::setSingleHeighPass(double v) {
    if (m_singleHeighPass != v) {
        m_singleHeighPass = v;
        emit singleHeighPassChanged();
    }
}


bool Catheter::getShowPassData() const {
    return m_showPassData;
}

void Catheter::setShowPassData(bool v) {
    if (m_showPassData != v) {
        m_showPassData = v;
        emit showPassDataChanged();
    }
}

Halve::ElectrodeType Catheter::getElectrode () const {
    return m_electrode;
}

void Catheter::setElectrode(Halve::ElectrodeType v) {
    if (m_electrode != v) {
        m_electrode = v;
        emit electrodeChanged();
    }
}

bool Catheter::employ() const
{
    return m_employ;
}

void Catheter::setEmploy(bool newEmploy)
{
    if (m_employ == newEmploy)
        return;
    m_employ = newEmploy;
    emit employChanged();
}

class ResetElectrodeSeatHelper {
public:
    void operator()(ElectrodeNode* node) const {
        node->setSeat(ElectrodeNode::UNSET);
        node->setD1(ElectrodeNode::UNSET);
        node->setD2(ElectrodeNode::UNSET);
    }
};

void Catheter::resetElectrodeSeat() {
    ResetElectrodeSeatHelper helper;
    std::for_each(m_pairNodes.begin(), m_pairNodes.end(), helper);
    std::for_each(m_bothNodes.begin(), m_bothNodes.end(), helper);
    std::for_each(m_singleNodes.begin(), m_singleNodes.end(), helper);
    m_bseat = CatheterNoSeta;
}
class ResetElectrodeDyestuffHelper {
public:
    ResetElectrodeDyestuffHelper(const QColor &orgDyestuff, const QColor &dyestuff):m_orgDyestuff(orgDyestuff),m_dyestuff(dyestuff){}
    void operator()(ElectrodeNode* node) const {
        node->setDyestuff(m_dyestuff);
    }
    const QColor &m_orgDyestuff;
    const QColor &m_dyestuff;
};

void Catheter::resetElectrodeDyestuff(const QColor &orgDyestuff)
{
    ResetElectrodeDyestuffHelper helper(orgDyestuff, m_dyestuff);
    std::for_each(m_pairNodes.begin(), m_pairNodes.end(), helper);
    std::for_each(m_bothNodes.begin(), m_bothNodes.end(), helper);
    std::for_each(m_singleNodes.begin(), m_singleNodes.end(), helper);
}

QList<ElectrodeNode *> Catheter::getElectrodeNodes(Halve::ElectrodeType electrode) const
{
    switch(electrode) {
    case Halve::PAIR:return m_pairNodes;
    case Halve::BOTH:return m_bothNodes;
    default:return m_singleNodes;
    }
}

QList<ElectrodeNode *> Catheter::getElectrodeNodes() const {
    return getElectrodeNodes(m_electrode);
}

QList<ElectrodeNode *> Catheter::getAllElectrodeNodes() const
{
    QList<ElectrodeNode *> nodeList;
    nodeList.append(m_pairNodes);
    nodeList.append(m_bothNodes);
    nodeList.append(m_singleNodes);
    return nodeList;
}

ElectrodeNode *Catheter::getElectrodeNode(Halve::ElectrodeType electrode, const QString &id)
{
    QList<ElectrodeNode *> nodeList = getElectrodeNodes(electrode);
    return findElectrodeNode(nodeList, id);
}

ElectrodeNode *Catheter::findElectrodeNode(const QList<ElectrodeNode *> &nodes, const QString &id)
{
    auto iter = std::find_if(nodes.begin(), nodes.end(), [&id](ElectrodeNode *node){
        return id == node->id();
    });
    return iter != nodes.end()?*iter:nullptr;
}


ElectrodeNode* Catheter::getElectrodeNode(Halve::ElectrodeType electrode, quint16 d) {
    QList<ElectrodeNode*> nodeList = getElectrodeNodes(electrode);
    auto iter = std::find_if(nodeList.begin(), nodeList.end(), [d](ElectrodeNode* node) {
        return node->d1() == d || node->d2() == d;
    });
    return iter != nodeList.end() ? *iter : nullptr;
}

ElectrodeNode* Catheter::getElectrodeNode(quint16 d) {
    return getElectrodeNode(m_electrode, d);
}

QString Catheter::meshName() const
{
    return m_meshName;
}

void Catheter::setMeshName(const QString &newMeshName)
{
    if (m_meshName == newMeshName)
        return;
    m_meshName = newMeshName;
    if (m_catheterMould != nullptr) {
        m_catheterMould->load(m_meshName, m_gap, m_electrodeLength);
    }
    emit meshNameChanged();
}

void Catheter::deleteMesh(const QString &meshPath) {
    if (!meshPath.isEmpty() && QFile::exists(meshPath)) {
        QFile::remove(meshPath);
    }
    m_meshName.clear();
    delete m_catheterMould;
    delete m_mesh;
}

CatheterMould *Catheter::catheterMould() {
    if(m_catheterMould == nullptr) {
        Q_ASSERT(Thread::currentlyOn(Thread::UI));
        m_catheterMould = new CatheterMould(m_meshName, m_gap, m_electrodeLength, this);
    }
    return m_catheterMould;
}

quint16 Catheter::port() const
{
    return m_port;
}

void Catheter::setPort(quint16 newPortIdx)
{
    if (m_port == newPortIdx)
        return;
    m_port = newPortIdx;
    emit portChanged();
}

CatheterMagnetism *Catheter::catheterMagnetism() const
{
    return m_catheterMagnetism;
}


CatheterType *Catheter::catheterType() const
{
    return m_catheterType;
}

void Catheter::setCatheterType(CatheterType *newCatheterType)
{
    m_catheterType = newCatheterType;
}

ElectrodeNode *Catheter::getElectrodeNode(const QString &id) {
    ElectrodeNode *electrodeNode = findElectrodeNode(m_pairNodes, id);
    if (electrodeNode == nullptr) {
        electrodeNode = findElectrodeNode(m_bothNodes, id);
        if (electrodeNode == nullptr) {
            electrodeNode = findElectrodeNode(m_singleNodes, id);
        }
    }
    return electrodeNode;
}

bool Catheter::isBelongTo(const ElectrodeNode *node) const
{
    return m_pairNodes.contains(node) || m_bothNodes.contains(node) || m_singleNodes.contains(node);
}

void Catheter::setPairNodes(quint16 bseat) {
    int seatIdx = bseat;
    for (ElectrodeNode* node : m_pairNodes) {
        node->setSeat(seatIdx++);
        node->setD1(bseat);
        node->setD2(++bseat);
        bseat++;
    }
}

void Catheter::setBothNodes(quint16 bseat) {
    int seatIdx = bseat;
    for (ElectrodeNode* node : m_bothNodes) {
        node->setSeat(seatIdx++);
        node->setD1(bseat);
        node->setD2(++bseat);
    }
}

void Catheter::setSingleNodes(quint16 bseat) {
    int seatIdx = bseat;
    for (ElectrodeNode* node : m_singleNodes) {
        node->setSeat(seatIdx++);
        node->setD1(bseat++);
    }
}

bool Catheter::destroyed() const
{
    return m_destroyed;
}

void Catheter::setDestroyed(bool newDestroyed)
{
    if (m_destroyed == newDestroyed)
        return;
    m_destroyed = newDestroyed;
    emit destroyedChanged();
}

bool Catheter::prefab() const
{
    return m_prefab;
}

void Catheter::setPrefab(bool newPrefab)
{
    if (m_prefab == newPrefab)
        return;
    m_prefab = newPrefab;
    emit prefabChanged();
}

double Catheter::electrodeLength() const
{
    return m_electrodeLength;
}

void Catheter::setElectrodeLength(double newElectrodeLength)
{
    if (qFuzzyCompare(m_electrodeLength, newElectrodeLength))
        return;
    m_electrodeLength = newElectrodeLength;
    emit electrodeLengthChanged();
}

void Catheter::setElectrodeSeat() {
    setPairNodes(m_bseat);
    setBothNodes(m_bseat);
    setSingleNodes(m_bseat);
    resetElectrodeNames();
    emit electrodeNodesChanged();
}
bool Catheter::isOccupied(quint16 bseat) {
    if (m_bseat == CatheterNoSeta) {
        return false;
    }
    quint16 eseat = m_bseat + m_amount;
    return m_bseat <= bseat && bseat < eseat;
}

void Catheter::resetDefaultElectrodeNodes(){
    for(int i = 0; i < m_amount; i+=2) {
        ElectrodeNode* node = new ElectrodeNode(this, this);
        node->setId(QString("%1.pair.%2.%3").arg(m_id).arg(QDateTime::currentMSecsSinceEpoch()).arg(i));
        node->setType(Halve::PAIR);
        node->setDyestuff(m_dyestuff);
        m_pairNodes.push_back(node);
    }
    for (int i = 0; i < (m_amount - 1); i++) {
        ElectrodeNode* node = new ElectrodeNode(this, this);
        node->setId(QString("%1.both.%2.%3").arg(m_id).arg(QDateTime::currentMSecsSinceEpoch()).arg(i));
        node->setType(Halve::BOTH);
        node->setDyestuff(m_dyestuff);
        m_bothNodes.push_back(node);
    }

    for (int i = 0; i < m_amount; ++i) {
        ElectrodeNode* node = new ElectrodeNode(this, this);
        node->setId(QString("%1.single.%2.%3").arg(m_id).arg(QDateTime::currentMSecsSinceEpoch()).arg(i));
        node->setType(Halve::SINGLE);
        node->setDyestuff(m_dyestuff);
        m_singleNodes.push_back(node);
    }
}

void Catheter::resetElectrodeNames() {
    int i = 1;
    for(auto node:m_pairNodes) {
        QString nodeName;
        if (i == 1) {
            nodeName = QString("%1 D-%2").arg(m_type).arg(i + 1);
        } else {
            nodeName = QString("%1 %2-%3").arg(m_type).arg(i).arg(i + 1);
        }
        node->setName(nodeName);
        i +=2;
    }

    i = 1;
    for(auto node:m_bothNodes) {
        QString nodeName;
        if (i == 1) {
            nodeName = QString("%1 D-%2").arg(m_type).arg(i + 1);
        } else {
            nodeName = QString("%1 %2-%3").arg(m_type).arg(i).arg(i + 1);
        }
        node->setName(nodeName);
        i+=1;
    }

    i = 1;
    for(auto node:m_singleNodes) {
        node->setName(QString("%1 %2").arg(m_type).arg(i));
        ++i;
    }
}

QString Catheter::getElectrodeName(int j) const
{
    if (j == 0) {
        return QString("%1 D").arg(m_name);

    }
    return QString("%1 D").arg(m_name);

}

bool Catheter::apparent() const
{
    return m_apparent;
}

void Catheter::setApparent(bool newApparent)
{
    if (m_apparent == newApparent)
        return;
    m_apparent = newApparent;
    emit apparentChanged();
}

void Catheter::setElectrodeLastPoint(const QString &id, const vtkVector3d &pos) {
    m_electrodeLastPos[id] = pos;
}

vtkVector3d Catheter::getElectrodeLastPoint(const QString &id) {
    vtkVector3d result(-1,-1, -1);
    if (m_electrodeLastPos.contains(id)) {
        result = m_electrodeLastPos[id];
    }
    return result;
}

void Catheter::clearLastPoint()
{
    m_electrodeLastPos.clear();
}
