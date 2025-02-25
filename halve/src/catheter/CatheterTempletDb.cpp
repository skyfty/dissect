#include "CatheterTemplet.h"
#include "CatheterTempletDb.h"
#include "qjsonarray.h"
#include "qjsonobject.h"

#include <QFile>
#include <QJsonDocument>

CatheterTempletDb::CatheterTempletDb(const QString& path, QObject* parent)
    : QObject{ parent } {
    Q_ASSERT(!path.isEmpty());
    m_path = path + "/catheter_templet.json";
    load();
}


void CatheterTempletDb::fromJson(const QJsonArray &templetJson) {
    for(auto json:templetJson) {
        CatheterTemplet* templet = new CatheterTemplet(this);
        templet->fromJson(json.toObject());
        m_templets.push_front(templet);
    }
}

QJsonArray CatheterTempletDb::toJson() const {
    QJsonArray templetJson;
    for(const CatheterTemplet* templet:m_templets) {
        templetJson.append(templet->toJson());
    }
    return templetJson;
}

void CatheterTempletDb::load() {
    QFile f(m_path);
    if (f.open(QIODevice::ReadOnly)) {
        fromJson(QJsonDocument::fromJson(f.readAll()).array());
    }
}

bool CatheterTempletDb::save() {
    QFile saveFile(m_path);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        return false;
    }
    QJsonArray templetJson = toJson();
    return saveFile.write(QJsonDocument(templetJson).toJson());
}

qsizetype CatheterTempletDb::size() const
{
    return m_templets.size();
}

CatheterTemplet *CatheterTempletDb::getData(int row)
{
    if (row < 0 || row > m_templets.size()) {
        return nullptr;
    }
    return m_templets[row];
}

CatheterTemplet *CatheterTempletDb::getData(const QString &name) {
    for(CatheterTemplet* templet:m_templets) {
        if (templet->name() == name) {
            return templet;
        }
    }
    return nullptr;
}

int CatheterTempletDb::indexOfName(const QString &name) {
    for(qsizetype i = 0; i < m_templets.size(); ++i) {
        if (m_templets[i]->name() == name) {
            return i;
        }
    }
    return -1;
}

void CatheterTempletDb::remove(CatheterTemplet* templet) {
    Q_ASSERT(templet !=nullptr);
    auto iter = std::find(std::begin(m_templets), std::end(m_templets), templet);
    if (iter != std::end(m_templets)) {
        m_templets.erase(iter);
        save();
        templet->deleteLater();
        emit deleted(templet);
    }
}

void CatheterTempletDb::remove(int row) {
    if (row >= 0 && row < m_templets.size()) {
        remove(m_templets[row]);
    }
}

CatheterTemplet * CatheterTempletDb::add(const QString &name) {
    CatheterTemplet *templet = new CatheterTemplet(this);
    templet->setName(name);
    m_templets.append(templet);
    emit added(templet);
    return templet;
}
