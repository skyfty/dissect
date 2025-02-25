#ifndef MAPPINGPOINTGROUPCONSULTFILTERLISTMODEL_H
#define MAPPINGPOINTGROUPCONSULTFILTERLISTMODEL_H

#include <QPointer>
#include <QSortFilterProxyModel>
class MeltSetting;

class MappingPointGroupConsultFilterListModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QString currentMappingPointGroup READ currentMappingPointGroup WRITE setCurrentMappingPointGroup NOTIFY currentMappingPointGroupChanged FINAL)

public:
    explicit MappingPointGroupConsultFilterListModel(QObject *parent = nullptr);
    QString currentMappingPointGroup() const;
    void setCurrentMappingPointGroup(const QString &newCurrentMappingPointGroup);

    Q_INVOKABLE int clone(const QString &groupId);
signals:
    void currentMappingPointGroupChanged();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    QString m_currentMappingPointGroup;
};

#endif // MAPPINGPOINTGROUPCONSULTFILTERLISTMODEL_H
