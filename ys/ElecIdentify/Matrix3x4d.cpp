#pragma once

#include "Matrix3x4d.h"

QDataStream& operator<<(QDataStream& out, const Matrix3x4d& data)
{
    out.writeRawData((const char*)data.data(), sizeof(data));
    return out;
}
QDataStream& operator>>(QDataStream& in, Matrix3x4d& data)
{
    in.readRawData((char*)data.data(), sizeof(data));
    return in;
}
