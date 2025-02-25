#ifndef PANTELECTRODELISTNODE_H
#define PANTELECTRODELISTNODE_H


#include "catheter/ElectrodeNodeListModel.h"

class ElectrodeNode;
class Profile;

class PantElectrodeListNode : public ElectrodeNodeListModel
{
    Q_OBJECT

public:
    explicit PantElectrodeListNode(QObject *parent = nullptr);
    void setProfile(Profile* profile) override;
public slots:
    void reset() override;
};

#endif // PANTELECTRODELISTNODE_H
