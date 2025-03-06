#include "Elec2WorldUpdater.h"
#include <qdebug.h>

namespace ys
{

bool Elec2WorldUpdater::GetRefValueValid() const
{
    return refValueValid;
}

void Elec2WorldUpdater::SetSize(const int newSize)
{
    elecParams.resize(newSize);
}

void Elec2WorldUpdater::SetRefIndex(const int newRefIndex)
{
    this->refIndex = newRefIndex;
}

void Elec2WorldUpdater::SetRefValue(double ex, double ey, double ez, double mx, double my, double mz)
{
    auto& param = elecParams[this->refIndex];
    param.ecurr << ex, ey, ez;
    param.mcurr << mx, my, mz;

    //参考电极
    Eigen::Vector3d de = param.ecurr - param.eprev;
    Eigen::Vector3d dm = param.mcurr - param.mprev;

    refValueValid = de.norm() > 1e-1;

    if (refValueValid)
    {
        Eigen::Vector3d dot = de.array() * dm.array();
        double k = dm.norm() / de.norm();
        refK << (dot(0) > 0 ? k : -k),
            (dot(1) > 0 ? k : -k),
            (dot(2) > 0 ? k : -k);
    }
}

void Elec2WorldUpdater::InitValue(int index, double ex, double ey, double ez, double mx, double my, double mz)
{
    auto& param = elecParams[index];
    param.eprev << ex, ey, ez;
    param.mprev << mx, my, mz;
    param.inited = true;
}

void Elec2WorldUpdater::ResetInitFlag()
{
    for (auto& e : elecParams)
    {
        e.inited = false;
    }
}

void Elec2WorldUpdater::ResetInitFlag(int index)
{
    if (index < 0 || index >= elecParams.size())
        return;
    elecParams[index].inited = false;
}

bool Elec2WorldUpdater::IsInited(int index) const
{
    if (index < 0 || index >= elecParams.size())
        return false;
    return elecParams[index].inited;
}

const ElecParameter &Elec2WorldUpdater::GetMagXYZ(int index, double ex, double ey, double ez)
{
    auto& param = elecParams[index];
    param.ecurr << ex, ey, ez;

    param.mcurr = param.mprev + ((param.ecurr - param.eprev).array() * refK.array()).matrix();

    if (refValueValid && autoUpdatePrev)
    {
        param.eprev = param.ecurr;
        param.mprev = param.mcurr;
    }

    // qDebug() << index << refValueValid << refK
    //          << param.eprev.x() << param.eprev.y() << param.eprev.z()
    //          << param.ecurr.x() << param.ecurr.y() << param.ecurr.z()
    //          << param.mprev.x() << param.mprev.y() << param.mprev.z()
    //          << param.mcurr.x() << param.mcurr.y() << param.mcurr.z();

    return param;
}

}
