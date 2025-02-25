#pragma once

namespace ys
{

    //二阶节定义
    class Sec2
    {
    public:
        double b0, b1, b2;      //分子
        double a0, a1, a2;      //分母
        double gain{1};         //缩放因子
    };
}
