#ifndef OSCILLOGRAPELECTRODE_H
#define OSCILLOGRAPELECTRODE_H

#include "profile/ExposedElectrode.h"

class OscillograpElectrode : public ExposedElectrode
{
    Q_OBJECT
public:
    explicit OscillograpElectrode(CatheterDb* catheterDb,QObject *parent = nullptr);
    void loadPant();
    void unloadPant();
protected:
    void initElectrodeNodes();

public slots:
    void onCatheterElectrodeNodesChanged(Catheter* catheter);
    void onCatheterElectrodeChanged(Catheter *catheter);


};

#endif // OSCILLOGRAPELECTRODE_H
