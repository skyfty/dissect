#ifndef ELECTRODENODESELECTLISTMODEL_H
#define ELECTRODENODESELECTLISTMODEL_H

#include "catheter/ElectrodeNodeListModel.h"

class ElectrodeNode;

class ElectrodeNodeSelectListModel : public ElectrodeNodeListModel
{
    Q_OBJECT
public:
    explicit ElectrodeNodeSelectListModel(QObject *parent = nullptr);
    void reset() override;
    void setProfile(Profile* profile) override;

signals:
    void currentElectrodeChanged();

public slots:
    void onCatheterEmployChanged(Catheter *catheter);
    void onCatheterElectrodeChanged(Catheter *catheter);
};

#endif // ELECTRODENODESELECTLISTMODEL_H
