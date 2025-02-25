#include "MeltOptions.h"
#include "catheter/ElectrodeSurveyor.h"
#include "melt/AppointElectrode.h"
#include "melt/MeltSamplingElectrode.h"
#include "qjsonarray.h"

#include <QJsonObject>

#include <catheter/CatheterDb.h>

MeltOptions::MeltOptions(CatheterDb* catheterDb,QObject *parent)
    : QObject{parent},m_catheterDb(catheterDb)
{}

void MeltOptions::fromJson(const QJsonObject &json) {
    if (json.contains("id"))
        m_id = json["id"].toInteger();
    if (json.contains("consultDataIndex"))
        m_consultDataIndex = json["consultDataIndex"].toInt();
    if (json.contains("appointElectrode")) {
        appointElectrode()->fromJson(json["appointElectrode"].toArray());
    }
    if (json.contains("meltSamplingElectrode")) {
        meltSamplingElectrode()->fromJson(json["meltSamplingElectrode"].toObject());
    }
    if (json.contains("electrodeSurveyor")) {
        electrodeSurveyor()->fromJson(json["electrodeSurveyor"].toArray());
    }
    if (json.contains("consultDataIndex")) {
        QJsonArray jsonArray = json["consultDataIndex"].toArray();
        for(qsizetype i = 0; i < jsonArray.size(); ++i) {
            m_consultDataIndexList.push_back(jsonArray[i].toInt());
        }
    }
}


QJsonObject MeltOptions::toJson() {
    QJsonObject json;
    json["id"] = m_id;
    json["consultDataIndex"] = m_consultDataIndex;
    json["appointElectrode"] = appointElectrode()->toJson();
    json["electrodeSurveyor"] = electrodeSurveyor()->toJson();
    json["meltSamplingElectrode"] = meltSamplingElectrode()->toJson();
    QJsonArray jsonArray;
    for(auto i:m_consultDataIndexList) {
        jsonArray.append(QJsonValue(i));
    }
    json["consultDataIndex"] = jsonArray;

    return json;
}

qint64 MeltOptions::id() const
{
    return m_id;
}

void MeltOptions::setId(qint64 newId)
{
    if (m_id == newId)
        return;
    m_id = newId;
    emit idChanged();
}

qint32 MeltOptions::consultDataIndex() const
{
    return m_consultDataIndex;
}

void MeltOptions::setConsultDataIndex(qint32 newConsultDataIndex)
{
    if (m_consultDataIndex == newConsultDataIndex)
        return;
    m_consultDataIndex = newConsultDataIndex;
    emit consultDataIndexChanged();
}


qint32 MeltOptions::nextConsultIndex()
{
    qint32 idx = indexConsultData(m_consultDataIndex);
    if (idx != -1 && idx + 1 < m_consultDataIndexList.size()) {
        m_consultDataIndex = m_consultDataIndexList[idx + 1];
        emit consultDataIndexChanged();
    }
    return m_consultDataIndex;
}

qint32 MeltOptions::previousConsultIndex()
{
    qint32 idx = indexConsultData(m_consultDataIndex);
    if (idx != -1 && idx - 1 >= 0) {
        m_consultDataIndex = m_consultDataIndexList[idx -1];
        emit consultDataIndexChanged();
    }
    return m_consultDataIndex;
}

qint32 MeltOptions::indexConsultData(int d) const
{
    for(qsizetype i = 0; i < m_consultDataIndexList.size(); ++i) {
        if (m_consultDataIndexList[i] ==d) {
            return i;
        }
    }
    return -1;
}

AppointElectrode *MeltOptions::appointElectrode()
{
    if (m_appointElectrode == nullptr) {
        m_appointElectrode = new AppointElectrode(m_catheterDb,this);

    }
    return m_appointElectrode;
}

ElectrodeSurveyor *MeltOptions::electrodeSurveyor() {
    if (m_electrodeSurveyor == nullptr) {
        m_electrodeSurveyor = new ElectrodeSurveyor(this);
    }
    return m_electrodeSurveyor;
}


const std::vector<qint32>& MeltOptions::consultDataIndexList() const
{
    return m_consultDataIndexList;
}

void MeltOptions::setConsultDataIndexList(const std::vector<qint32>& newConsultDataIndexList)
{
    m_consultDataIndexList = newConsultDataIndexList;
    emit consultListCountChanged();
}


MeltSamplingElectrode *MeltOptions::meltSamplingElectrode()
{
    if (m_meltSamplingElectrode == nullptr) {
        m_meltSamplingElectrode = new MeltSamplingElectrode(m_catheterDb,this);
    }
    return m_meltSamplingElectrode;
}

qint32 MeltOptions::consultListIndex() const
{
    return indexConsultData(m_consultDataIndex);
}
qint32 MeltOptions::consultListCount() const {
    return m_consultDataIndexList.size();
}
