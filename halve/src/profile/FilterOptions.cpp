#include <qjsonobject.h>
#include <QJsonDocument>
#include "FilterOptionItem.h"
#include "FilterOptions.h"

FilterOptions::FilterOptions(QObject *parent)
    : QObject{parent}
{

}

FilterOptionItem *FilterOptions::getEcg() {
    if (m_ecg == nullptr) {
        m_ecg = new FilterOptionItem(this);
        m_ecg->setHighPass(0.05);
        m_ecg->setLowPass(30);
        m_ecg->setNotch10Hz(false);
        QObject::connect(m_ecg, &FilterOptionItem::highPassChanged, this, &FilterOptions::changed);
        QObject::connect(m_ecg, &FilterOptionItem::lowPassChanged, this, &FilterOptions::changed);
    }
    return m_ecg;
}

FilterOptionItem *FilterOptions::getCatheterDouble()
{
    if (m_catheterDouble == nullptr) {
        m_catheterDouble = new FilterOptionItem(this);
        m_catheterDouble->setHighPass(30);
        m_catheterDouble->setLowPass(300);
        m_catheterDouble->setNoise(false);
        m_catheterDouble->setMagnetic(false);
        m_catheterDouble->setNotch10Hz(false);
        QObject::connect(m_catheterDouble, &FilterOptionItem::highPassChanged, this, &FilterOptions::changed);
        QObject::connect(m_catheterDouble, &FilterOptionItem::lowPassChanged, this, &FilterOptions::changed);
    }
    return m_catheterDouble;
}


FilterOptionItem *FilterOptions::getCatheterSingle()
{
    if (m_catheterSingle == nullptr) {
        m_catheterSingle = new FilterOptionItem(this);
        m_catheterSingle->setHighPass(0.05);
        m_catheterSingle->setLowPass(30);
        m_catheterSingle->setNotch10Hz(false);
        QObject::connect(m_catheterSingle, &FilterOptionItem::highPassChanged, this, &FilterOptions::changed);
        QObject::connect(m_catheterSingle, &FilterOptionItem::lowPassChanged, this, &FilterOptions::changed);
    }
    return m_catheterSingle;
}


void FilterOptions::fromJson(const QJsonObject &json)
{
    if (json.contains("ecg")) {
        getEcg()->fromJson(json["ecg"].toObject());
    }
    if (json.contains("double"))
        getCatheterDouble()->fromJson(json["double"].toObject());

    if (json.contains("single"))
        getCatheterSingle()->fromJson(json["single"].toObject());
}

QJsonValue FilterOptions::toJson()
{
    QJsonObject json;
    json["ecg"] = getEcg()->toJson();
    json["double"] = getCatheterDouble()->toJson();
    json["single"] = getCatheterSingle()->toJson();
    return json;
}
