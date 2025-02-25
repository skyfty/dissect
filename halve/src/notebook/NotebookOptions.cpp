#include "NotebookOptions.h"

#include <QJsonObject>

NotebookOptions::NotebookOptions(QObject *parent)
    : QObject{parent}
{}

void NotebookOptions::fromJson(const QJsonObject &json)
{

    if (json.contains("sortType"))
        m_sortType = (Halve::NotebookSortType)json["sortType"].toInt();
}

QJsonValue NotebookOptions::toJson()
{
    QJsonObject json;
    json["sortType"] = m_sortType;
    return json;
}

Halve::NotebookSortType NotebookOptions::sortType() const
{
    return m_sortType;
}

void NotebookOptions::setSortType(const Halve::NotebookSortType &newSortType)
{
    if (m_sortType == newSortType)
        return;
    m_sortType = newSortType;
    emit sortTypeChanged();
}
