#include <ElecIdentify.h>
#include <Elec2WorldUpdater.h>
#include <iostream>

bool identifySuccess = false;
bool updaterInited = false;
QList<ys::InputParameter> list;

ys::ElecIdentify identify;
ys::Elec2WorldUpdater updater;

bool IsTrainPointsOK(const QList<ys::InputParameter>& points)
{
    //这里需要添加判断算法。
    //可以先暂时用现在的方法，也就是收集固定数量的点就开始训练。
    return true;
}

void NewDataArrive()
{
    ys::InputParameter ip;
    ip.mQ = Eigen::Quaterniond(1, 2, 3, 4);//磁wxyz，这些用接收的数据包中的对应数值填充。
    ip.m0 << 1, 2, 3;//磁xyz
    ip.eReference << 1, 1, 1;//参考电极坐标
    ip.eTarget << 5, 7, 8;//目标电极坐标

    int referenceIndex = 6;

    //没有训练成功，需要选点填充list
    if (!identifySuccess)
    {
        //收集比较广泛的数据，加入list
        list.push_back(ip);

        //list中的点是否满足训练条件？
        if (IsTrainPointsOK(list))
        {
            identify.SetMReference({0, 0, 0});
            identify.SetMTarget({0, 0, -5});
            identifySuccess = identify.Train(list.begin(), (int)list.size());
        }
        return;
    }

    //没有初始化updater，先初始化
    if (!updaterInited)
    {
        ////////////////下面部分是updater的初始化代码
        updater.SetSize();//用默认参数就行，只要大于实际路数48或者56就行。
        updater.SetRefIndex(referenceIndex);//参考电极是第几路，就设置为第几路的index
        for (int i=0; i<20; i++)//导管盒连接了几路电极，就设置几路，不连接的没有必要设置。
        {
            ip.ePoint << 10, 8, 3;//第i路电极的x，y，z坐标，这个是某个数据包pack中的上传数据。
            Eigen::Vector3d mpos = identify.E2W(ip);//ip是数据包pack中解包出来的对应数据。
            updater.InitValue(i, ip.ePoint(0), ip.ePoint(1), ip.ePoint(2), mpos(0), mpos(1), mpos(2));
        }
        ///////////updater 初始化结束
        updaterInited = true;
        return;
    }

    //先输入参考电极数据，计算updater参数
    //更新参考电极
    {
        ip.ePoint = ip.eReference;
        Eigen::Vector3d mpos = identify.E2W(ip);
        updater.SetRefValue(ip.ePoint(0), ip.ePoint(1), ip.ePoint(2),
                          mpos(0), mpos(1), mpos(2));
    }

    //参考点计算的参数有效吗？
    if (!updater.GetRefValueValid())
        return;

    //更新所有电极坐标    
    for (int i = 0; i < 20; ++i)
    {
        ip.ePoint << 10, 8, 3;//第i路电极的x，y，z坐标，这个是某个数据包pack中的上传数据。
        const auto& param = updater.GetMagXYZ(i, ip.ePoint.x(), ip.ePoint.y(), ip.ePoint.z());
        std::cout << "换算的磁坐标：" << param.mcurr << std::endl;
    }
}
