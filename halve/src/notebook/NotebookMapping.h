#ifndef NOTEBOOKMAPPING_H
#define NOTEBOOKMAPPING_H

#include "Notebook.h"

class NotebookMapping : public Notebook
{
    Q_OBJECT
public:
    explicit NotebookMapping(const QString &path,QObject *parent = nullptr);
    Halve::NotebookType type() const override;

    void fromJson(const QJsonObject &v) override;
    void toJson(QJsonObject &json) const override;
    QString mappingPointGroup() const;
    void setMappingPointGroup(const QString &newMappingPointGroup);

signals:

private:
    QString m_mappingPointGroup;
};

#endif // NOTEBOOKMAPPING_H
