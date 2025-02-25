#ifndef APPOINTELECTRODE_H
#define APPOINTELECTRODE_H

#include "profile/ExposedElectrode.h"

class AppointElectrode : public ExposedElectrode
{
    Q_OBJECT
public:
    explicit AppointElectrode(CatheterDb* catheterDb,QObject *parent = nullptr);

};

#endif // APPOINTELECTRODE_H
