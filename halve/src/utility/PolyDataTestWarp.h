#ifndef POLYDATAWARP_H
#define POLYDATAWARP_H

#include "utility/VtkDataWarp.h"

class PolyDataTestWarp:PolyDataWarp
{
public:
    typedef QSharedPointer<PolyDataTestWarp> Ptr;

public:
    PolyDataTestWarp() = default;
public:
    qint64 m_expendTime = 0;
    int m_numberOfPoints = 0;
};

#endif // POLYDATAWARP_H
