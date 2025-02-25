#include "ElectrodeSurveyor.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

ElectrodeSurveyor::ElectrodeSurveyor(QObject *parent)
    : QObject{parent}
{}

void ElectrodeSurveyor::fromJson(const QJsonArray &jsonArray) {
    for(auto json:jsonArray) {
        QJsonObject o = json.toObject();
        m_list[o["id"].toString()] = o["value"].toDouble();
    }
}


QJsonValue ElectrodeSurveyor::toJson() const {
    QJsonArray json;
    for (auto i = m_list.cbegin(), end = m_list.cend(); i != end; ++i) {
        QJsonObject o;
        o["id"] = i.key();
        o["value"] = i.value();
        json.push_back(o);
    }
    return json;
}

double ElectrodeSurveyor::getData(const QString &id, double def) const
{
    return m_list.contains(id)?m_list[id]:def;
}

bool ElectrodeSurveyor::setData(const QString &id, double val) {
    if (m_list.contains(id) && m_list[id] == val) {
        return false;
    }
    m_list[id] = val;
    emit changed(id);
    return true;
}

void ElectrodeSurveyor::remove(const QString &id)
{
    m_list.remove(id);
}
