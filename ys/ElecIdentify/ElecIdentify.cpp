#include "ElecIdentify.h"

namespace ys
{
Eigen::Vector3d ElecIdentify::GetMReference() const
{
    return mReference;
}

void ElecIdentify::SetMReference(const Eigen::Vector3d &newReference)
{
    mReference = newReference;
}

Eigen::Vector3d ElecIdentify::GetMTarget() const
{
    return mTarget;
}

void ElecIdentify::SetMTarget(const Eigen::Vector3d &newTarget)
{
    mTarget = newTarget;
}

const Eigen::Matrix3d& ElecIdentify::GetE2W() const
{
    return e2w;
}
void ElecIdentify::SetE2W(const Eigen::Matrix3d&vec) {
    e2w = vec;
}
void ElecIdentify::SetSingularThreshold(double newSingularThreshold)
{
    singularThreshold = newSingularThreshold;
}

bool ElecIdentify::Train(InputParameter *input, const int elementCount)
{
    return Train(elementCount, input);
}

Eigen::Vector3d ElecIdentify::E2W(InputParameter &input)
{
    //公式：e2w * (ei-er) + M * mr + m0w

    Eigen::Vector3d ed = input.eTarget - input.eReference;
    Eigen::Vector3d ep = input.ePoint - input.eReference;
    Eigen::Vector3d md = mTarget - mReference;
    ep /= (ed.norm() / md.norm());

    Eigen::Vector3d de = ep;
    return e2w *de + input.mQ.toRotationMatrix() * mReference + input.m0;
}
}
