#include "ElecIdentify.h"

namespace ys
{

const Matrix3x4d &ElecIdentify::GetE2W() const
{
    return e2w;
}

void ElecIdentify::SetE2W(const Matrix3x4d &newE2W)
{
    if (e2w == newE2W)
        return;
    e2w = newE2W;
}

}
