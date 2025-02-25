#ifndef BODYSURFACECATHETERLISTMODEL_H
#define BODYSURFACECATHETERLISTMODEL_H

#include "ElectrodeNodeListModel.h"

class Profile;

class BodySurfaceElectrodeNodeListModel : public ElectrodeNodeListModel
{
    Q_OBJECT

public:
    explicit BodySurfaceElectrodeNodeListModel(QObject *parent = nullptr);
    void setProfile(Profile* profile) override;

protected:
    void reset() override;

};

#endif // BODYSURFACECATHETERLISTMODEL_H
