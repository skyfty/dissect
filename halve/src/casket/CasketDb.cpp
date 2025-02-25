#include "Casket.h"
#include "CasketDb.h"

#include "qdir.h"
#include "qjsonobject.h"
#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QResource>
#include <catheter/Catheter.h>
#include <catheter/CatheterDb.h>

CasketDb::CasketDb(const QString &path, CatheterDb *catheterDb,QObject *parent)
    : QObject{parent},m_catheterDb(catheterDb)
{
    Q_ASSERT(!path.isEmpty());
    m_path = getStoragePath(path);
    if (QFile::exists(m_path)) {
        load();
    } else {
        initDefault();
    }
}
QString CasketDb::getStoragePath(const QString &path) {
    return path + "/casket.json";
}

void CasketDb::fromJson(const QJsonObject &casketJson) {
    for(auto key:casketJson.keys()) {
        Casket* casket = load(casketJson[key].toObject());;
        m_seats[casket->index()] = casket->catheterId();
        m_caskets[key] =casket;
    }
}

QJsonObject CasketDb::toJson() const {
    QJsonObject casketJson;
    for (auto i = m_caskets.cbegin(), end = m_caskets.cend(); i != end; ++i) {
        QJsonObject json;
        i.value()->toJson(json);
        casketJson[i.key()] = json;
    }
    return casketJson;
}

void CasketDb::import(const QJsonObject &catheterJson) {
    for(size_t i =0; i < std::size(m_seats); ++i) {
        m_seats[i] = "";
    }
    for (auto i = m_caskets.cbegin(), end = m_caskets.cend(); i != end; ++i) {
        i.value()->deleteLater();
    }
    m_caskets.clear();
    fromJson(catheterJson);
    emit imported();
    emit m_catheterDb->alined();
}

void CasketDb::load(const QByteArray &casketJson) {
    fromJson(QJsonDocument::fromJson(casketJson.data()).object());
}

Casket* CasketDb::load(const QJsonObject &json) {
    Casket* casket = new Casket(this);
    casket->fromJson(json);
    return casket;
}

void CasketDb::initDefault() {
    load(QResource(":/assets/configures/casket.json").uncompressedData());
}

void CasketDb::load() {
    QFile f(m_path);
    if (f.open(QIODevice::ReadOnly)) {
        load(f.readAll());
    }
}

qsizetype CasketDb::size() const
{
    return m_caskets.size();
}

Casket *CasketDb::getData(int row) const {
    for (auto i = m_caskets.cbegin(), end = m_caskets.cend(); i != end; ++i) {
        if (i.value()->index() == row) {
            return i.value();
        }
    }
    return nullptr;
}

void CasketDb::occupy(quint16 bseat, const QString &catheterId) {
    Q_ASSERT(bseat >= 0 && bseat < std::size(m_seats));

    Catheter *catheter = m_catheterDb->getData(catheterId);
    if (catheter == nullptr) {
        return;
    }
    catheter->resetElectrodeSeat();
    reset(catheterId);
    catheter->setBseat(bseat);
    catheter->setEmploy(true);
    catheter->setElectrodeSeat();
    m_catheterDb->aline();
    m_catheterDb->save();

    quint16 eseat = bseat + catheter->getAmount();
    for(quint16 seat = bseat; seat < eseat; ++seat ) {
        m_seats[seat] = catheterId;
        getData(seat)->setCatheterId(catheterId);
    }
    save();
}

void CasketDb::reset(const QString &catheterId) {
    for(size_t i =0; i < std::size(m_seats); ++i) {
        if (m_seats[i] == catheterId) {
            getData(i)->setCatheterId("");
            m_seats[i] = "";
        }
    }
}

void CasketDb::reset(quint16 idx) {
    Q_ASSERT(idx >= 0 && idx < std::size(m_seats));
    QString catheterId = m_seats[idx];
    if (catheterId.isEmpty()) {
        return;
    }
    Catheter *catheter = m_catheterDb->getData(catheterId);
    if (catheter != nullptr) {
        catheter->resetElectrodeSeat();
        catheter->setEmploy(false);
    }
    reset(catheterId);

    m_catheterDb->aline();
    m_catheterDb->save();
    save();
}

int CasketDb::vacancyFrom(quint16 idx) const {
    Q_ASSERT(idx >= 0 && idx < std::size(m_seats));
    int cnt = 0;
    for(int i =idx; i < std::size(m_seats); ++i) {
        if (m_seats[i].isEmpty()) {
            cnt++;
        } else {
            break;
        }
    }
    return cnt;
}

QList<Casket *> CasketDb::getCasketsAtBox(int box)
{
    QList<Casket *> caskets;
    for (auto i = m_caskets.cbegin(), end = m_caskets.cend(); i != end; ++i) {
        if (i.value()->box() == box) {
            caskets.push_back(i.value());
        }
    }
    return caskets;
}

bool CasketDb::save() {
    QFile saveFile(m_path);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        return false;
    }
    QJsonObject casketJson = toJson();
    return saveFile.write(QJsonDocument(casketJson).toJson());
}

void CasketDb::backup(const QString &path)
{
    QString backupFile = getStoragePath(path);
    QFile::copy(m_path, backupFile);
}
