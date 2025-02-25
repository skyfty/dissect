#include "CatheterTypeDb.h"

#include "catheter/CatheterType.h"
#include "qjsonarray.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <qresource.h>
#include <QFile>
#include <QResource>

using namespace std::placeholders;
CatheterTypeDb::CatheterTypeDb(const QString &path, QObject *parent)
    : QObject{parent}
{
    m_filePath = path + "/catheter_type.json";
    if (QFile::exists(m_filePath)) {
        load();
    } else {
        initDefault();
    }
}

void CatheterTypeDb::connectSignals(CatheterType *type) {
    QObject::connect(type, &CatheterType::nameChanged, this, std::bind(&CatheterTypeDb::nameChanged, this, type, _1));
}
void CatheterTypeDb::initDefault() {
    auto data = QResource(":/assets/configures/catheter_type.json").uncompressedData();
    load(data.data());
}

void CatheterTypeDb::fromJson(const QJsonArray &jsonArray) {
    for(auto json:jsonArray) {
        CatheterType *type = new CatheterType(this);
        connectSignals(type);
        type->fromJson(json.toObject());
        m_lists.append(type);
    }
}

QJsonArray CatheterTypeDb::toJson() const {
    QJsonArray typeSortJson;
    for(const CatheterType* type:m_lists) {
        typeSortJson.append( type->toJson());
    }
    return typeSortJson;
}
void CatheterTypeDb::load(const QByteArray &jsonContent) {
    QJsonArray jsonArray = QJsonDocument::fromJson(jsonContent).array();
    fromJson(jsonArray);
}

void CatheterTypeDb::load() {
    QFile f(m_filePath);
    if (f.open(QIODevice::ReadOnly)) {
        load(f.readAll());
    }
}

bool CatheterTypeDb::save() {
    QFile saveFile(m_filePath);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        return false;
    }
    QJsonArray typeSortJson =toJson();
    return saveFile.write(QJsonDocument(typeSortJson).toJson());
}

void CatheterTypeDb::remove(int row) {
    if (row != -1) {
        QString name = m_lists[row]->name();
        m_lists.removeAt(row);
        emit deleted(name);
        save();
    }
}

void CatheterTypeDb::add(const QString &name) {
    CatheterType *type = new CatheterType(this);
    type->setName(name);
    connectSignals(type);
    m_lists.push_front(type);
    save();
    emit added(name);
}
bool CatheterTypeDb::update(int row, const QString &name) {
    if (row < m_lists.size()) {
        if (m_lists[row]->name() != name) {
            m_lists[row]->setName(name);
            save();
            return true;
        }
    }
    return false;
}

CatheterType* CatheterTypeDb::getData(int row) {
    if (row > m_lists.size()) {
        return nullptr;
    }
    return m_lists[row];
}

CatheterType *CatheterTypeDb::getData(const QString &name) {
    for(CatheterType* type:m_lists) {
        if (type->name() == name) {
            return type;
        }
    }
    return nullptr;
}

qsizetype CatheterTypeDb::size() const
{
    return m_lists.size();
}
int CatheterTypeDb::indexOfName(const QString &name) {
    for(int i = 0; i < m_lists.size(); ++i) {
        if (m_lists[i]->name() == name) {
            return i;
        }
    }
    return -1;
}
