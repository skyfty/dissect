#ifndef NOTEBOOKOPTIONS_H
#define NOTEBOOKOPTIONS_H

#include <QObject>
#include "HalveType.h"

class NotebookOptions : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Halve::NotebookSortType sortType READ sortType WRITE setSortType NOTIFY sortTypeChanged FINAL)

public:
    explicit NotebookOptions(QObject *parent = nullptr);
    void fromJson(const QJsonObject &v);
    QJsonValue toJson();

    Halve::NotebookSortType sortType() const;
    void setSortType(const Halve::NotebookSortType &newSortType);

signals:

    void sortTypeChanged();

private:
    Halve::NotebookSortType m_sortType = Halve::NOTEBOOK_SORT_CATEGORY;

};

#endif // NOTEBOOKOPTIONS_H
