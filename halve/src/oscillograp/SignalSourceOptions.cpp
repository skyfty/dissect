#include "SignalSourceOptions.h"
#include <qjsonarray.h>
#include <qjsonobject.h>

SignalSourceOptions::SignalSourceOptions(QObject *parent)
    : QObject{parent}
{

}

void SignalSourceOptions::fromJson(const QJsonArray &json)
{
    for(auto item:json) {
        m_sources.append(item.toInt());
    }

}

QJsonValue SignalSourceOptions::toJson() {
    QJsonArray arr;
    for(auto item:m_sources) {
        arr.append(item);
    }
    return arr;
}

bool SignalSourceOptions::itemSelected(int v)
{
    return m_sources.indexOf(v) != -1;
}

void SignalSourceOptions::setItemSelected(int v, bool b) {
    int idx = m_sources.indexOf(v);
    if (b) {
        if (idx == -1) {
            m_sources.push_back(v);
            emit itemSelectedChanged();
        }
    } else if (idx != -1) {
        m_sources.removeAt(idx);
        emit itemSelectedChanged();
    }
}
