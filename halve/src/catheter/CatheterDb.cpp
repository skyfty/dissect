#include "Catheter.h"
#include "CatheterDb.h"
#include "catheter/ElectrodeNode.h"
#include "qdir.h"
#include "qjsonarray.h"
#include "qjsonobject.h"
#include <QApplication>
#include <QJsonDocument>
#include <QResource>
#include "utility/FileUtils.h"

CatheterDb::CatheterDb(const QString &path,QObject *parent)
    : QObject{parent}{
    Q_ASSERT(!path.isEmpty());
    m_path = getStoragePath(path);
    load();
    loadBodySurface();
    loadPant();
}

QString CatheterDb::getStoragePath(const QString &path) {
    return path + "/catheter.json";
}

void CatheterDb::load(const QByteArray &catheterJson) {
    fromJson(QJsonDocument::fromJson(catheterJson.data()).array());
}

Catheter* CatheterDb::fromJson(const QJsonObject &json) {
    Catheter* catheter = new Catheter(this);
    catheter->fromJson(json);
    connectSignals(catheter);
    return catheter;
}

void CatheterDb::connectSignals(Catheter *catheter) {
    QObject::connect(catheter, &Catheter::employChanged, this, std::bind(&CatheterDb::employChanged, this, catheter));
    QObject::connect(catheter, &Catheter::electrodeChanged, this, std::bind(&CatheterDb::electrodeChanged, this, catheter));
    QObject::connect(catheter, &Catheter::electrodeNodesChanged, this, std::bind(&CatheterDb::electrodeNodesChanged, this, catheter));
    QObject::connect(catheter, &Catheter::typeChanged, this, std::bind(&CatheterDb::typeChanged, this, catheter));
    QObject::connect(catheter, &Catheter::dyestuffChanged, this, std::bind(&CatheterDb::dyestuffChanged, this, catheter));
    QObject::connect(catheter, &Catheter::apparentChanged, this, std::bind(&CatheterDb::apparentChanged, this, catheter));
    QObject::connect(catheter, &Catheter::portChanged, this, std::bind(&CatheterDb::portChanged, this, catheter));
    QObject::connect(catheter, &Catheter::gapChanged, this, std::bind(&CatheterDb::gapChanged, this, catheter));
    QObject::connect(catheter, &Catheter::amountChanged, this, std::bind(&CatheterDb::amountChanged, this, catheter));

}

void CatheterDb::loadBodySurface() {
    auto data = QResource(":/assets/configures/bodysurface.json").uncompressedData();
    m_bodysurface = fromJson(QJsonDocument::fromJson(data).object());
}
void CatheterDb::loadPant() {
    auto data = QResource(":/assets/configures/pant.json").uncompressedData();
    m_pant = fromJson(QJsonDocument::fromJson(data).object());
}

void CatheterDb::loadTest() {
    auto data = QResource(":/assets/configures/catheter.json").uncompressedData();
    load(data);
}

void CatheterDb::fromJson(const QJsonArray &catheterJson) {
    for(auto json:catheterJson) {
        m_catheters.append(fromJson(json.toObject()));
    }
}

QJsonArray CatheterDb::toJson() const {
    QJsonArray catheterJson;
    for(Catheter* catheter:m_catheters) {
        QJsonObject json;
        catheter->toJson(json);
        catheterJson.append(json);
    }
    return catheterJson;
}

void CatheterDb::load() {
    QFile f(m_path);
    if (f.open(QIODevice::ReadOnly)) {
        load(f.readAll());
    }
}

bool CatheterDb::save() {
    QFile saveFile(m_path);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        return false;
    }
    QJsonArray catheterJson = toJson();
    return saveFile.write(QJsonDocument(catheterJson).toJson());
}

void CatheterDb::remove(Catheter* catheter, bool force) {
    auto iter = std::find(std::begin(m_catheters), std::end(m_catheters), catheter);
    if (iter != std::end(m_catheters)) {
        m_catheters.erase(iter);
        save();
        catheter->setDestroyed(true);
        if (force) {
            catheter->deleteLater();
        }
        emit deleted(catheter);
    }
}

void CatheterDb::remove(int row,bool force) {
    if (row < m_catheters.size()) {
        remove(m_catheters[row], force);
    }
}

void CatheterDb::import(const QJsonArray &catheterJson) {
    for (qsizetype i = 0; i < m_catheters.size(); ++i) {
        Catheter* catheter = m_catheters[i];
        catheter->setDestroyed(true);
        emit deleted(catheter);
    }
    m_catheters.clear();

    for(auto json:catheterJson) {
        Catheter* catheter = fromJson(json.toObject());
        m_catheters.append(catheter);
        emit added(catheter);
    }
    emit imported();
}

Catheter* CatheterDb::add() {
    Catheter *newCatheter = new Catheter(this);
    m_catheters.push_front(newCatheter);
    connectSignals(newCatheter);
    emit added(newCatheter);
    return newCatheter;
}

qsizetype CatheterDb::size() const
{
    return m_catheters.size();
}

Catheter* CatheterDb::getData(int row) const
{
    if (row<0 || row > m_catheters.size()) {
        return nullptr;
    }
    return m_catheters[row];
}

Catheter* CatheterDb::getData(const QString &id) {
    if (m_bodysurface != nullptr && m_bodysurface->id() == id) {
        return m_bodysurface;
    }
    if (m_pant != nullptr && m_pant->id() == id) {
        return m_pant;
    }
    for(qsizetype i = 0; i < m_catheters.size(); ++i) {
        Catheter *catheter = m_catheters[i];
        if (catheter->id() == id) {
            return catheter;
        }
    }
    return nullptr;
}

Catheter *CatheterDb::getDataFrom(const ElectrodeNode *node) const
{
    if (m_bodysurface != nullptr && m_bodysurface->isBelongTo(node)) {
        return m_bodysurface;
    }
    if (m_pant != nullptr && m_pant->isBelongTo(node)) {
        return m_pant;
    }
    for(qsizetype i = 0; i < m_catheters.size(); ++i) {
        Catheter *catheter = m_catheters[i];
        if (catheter->isBelongTo(node)) {
            return catheter;
        }
    }
    return nullptr;
}
QList<Catheter *> CatheterDb::getDatasAtPort(quint16 port) {
    QList<Catheter *> catheterList;
    for(qsizetype i = 0; i < m_catheters.size(); ++i) {
        Catheter *catheter = m_catheters[i];
        if (catheter->port() == port) {
            catheterList.append(catheter);
        }
    }
    return catheterList;
}
Catheter* CatheterDb::getDataAtSeat(quint16 seat) {
    for(qsizetype i = 0; i < m_catheters.size(); ++i) {
        Catheter *catheter = m_catheters[i];
        if (catheter->isOccupied(seat)) {
            return catheter;
        }
    }
    return nullptr;
}

QList<Catheter *> CatheterDb::getEmployDatas() const
{
    QList<Catheter *> catheterList;
    for(qsizetype i = 0; i < m_catheters.size(); ++i) {
        Catheter *catheter = m_catheters[i];
        if (catheter->employ()) {
            catheterList.append(catheter);
        }
    }
    return catheterList;
}
QList<Catheter*> CatheterDb::getMagnetismDatas() const {
    QList<Catheter *> catheterList;
    for(qsizetype i = 0; i < m_catheters.size(); ++i) {
        Catheter *catheter = m_catheters[i];
        if (catheter->magnetism()) {
            catheterList.append(catheter);
        }
    }
    return catheterList;
}
QList<Catheter*> CatheterDb::getCSDatas(bool e) {
    QList<Catheter *> catheterList;
    for(qsizetype i = 0; i < m_catheters.size(); ++i) {
        Catheter *catheter = m_catheters[i];
        if ( catheter->getType() == CSCatheterType) {
            if ((e || catheter->employ())) {
                catheterList.append(catheter);
            }
        }
    }
    return catheterList;
}

QList<Catheter *> CatheterDb::getDatas() const
{
    return m_catheters;
}

int CatheterDb::index(const Catheter *catheter) const
{
    if (m_bodysurface == catheter) {
        return -2;
    }
    return m_catheters.indexOf(catheter);
}

QList<ElectrodeNode *> CatheterDb::getAllElectrodeNodes(quint16 type)
{
    QList<ElectrodeNode *> electrodeNodes;
    if (m_bodysurface != nullptr && (type & ElectrodeType::SURFACE)) {
        electrodeNodes.append(m_bodysurface->getElectrodeNodes());
    }
    if (m_pant != nullptr && (type & ElectrodeType::PANT)) {
        electrodeNodes.append(m_pant->getElectrodeNodes());
    }
    for(qsizetype i = 0; i < m_catheters.size(); ++i) {
        Catheter *catheter = m_catheters[i];
        if (!catheter->employ()) {
            continue;
        }
        if ((type & ElectrodeType::INSIDE)) {
            electrodeNodes.append(catheter->getElectrodeNodes());
        }
    }
    return electrodeNodes;
}

ElectrodeNode *CatheterDb::getElectrodeNode(const QString &id)
{
    ElectrodeNode *node = nullptr;
    if (m_bodysurface != nullptr) {
        node = m_bodysurface->getElectrodeNode(m_bodysurface->getElectrode(), id);
    }
    if (node == nullptr && m_pant != nullptr) {
        node = m_pant->getElectrodeNode(m_pant->getElectrode(), id);
    }
    if (node == nullptr) {
        for(qsizetype i = 0; i < m_catheters.size(); ++i) {
            Catheter *catheter = m_catheters[i];
            if (!catheter->employ()) {
                continue;
            }
            node = catheter->getElectrodeNode(id);
            if (node != nullptr) {
                break;
            }
        }
    }
    return node;
}

void CatheterDb::mix(Catheter *catheter) {
    if (catheter == nullptr) {
        return;
    }
    QJsonObject json;
    catheter->toJson(json);

    Catheter *newCatheter = add();
    newCatheter->fromJson(json);
    newCatheter->setId(QString("catheter.id.%1").arg(QDateTime::currentMSecsSinceEpoch()));
    newCatheter->resetDefaultElectrodeNodes();
    if (newCatheter->magnetism()) {
        newCatheter->setPort(getMagnetismVacancyPort());
    }

    QFile f(file_utils::getCatheterMeshPath(catheter->meshName()));
    if (f.open(QIODevice::ReadOnly)) {
        newCatheter->setMeshName(f.readAll());
    }

    save();
    emit mixed(newCatheter);
}

void CatheterDb::aline() {
    std::sort(m_catheters.begin(), m_catheters.end(), [](Catheter *left, Catheter *right){
        return left->getElectrodeNodes()[0]->seat() < right->getElectrodeNodes()[0]->seat();
    });
    emit alined();
}

void CatheterDb::backup(const QString &path)
{
    QString backupFile = getStoragePath(path);
    QFile::copy(m_path, backupFile);
}
int CatheterDb::getMagnetismVacancyPort() const {
    int port = 2;
    for(;port < MagnetismPortAmount; ++port) {
        bool result = false;
        for(Catheter *catheter : m_catheters) {
            if (catheter->magnetism()) {
                if (catheter->port() == port) {
                    result = true;
                    break;
                }
            }
        }
        if (!result) {
            break;
        }
    }
    return port;
}
void CatheterDb::clearLastPoint() {
    for(Catheter *catheter : m_catheters) {
        catheter->clearLastPoint();
    }
}
