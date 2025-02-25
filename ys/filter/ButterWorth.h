///
/// 参考信号与系统教科书，知道从s平面到归一化的低通滤波器p平面之间的映射关系。
/// 带入归一化低通滤波器表达式，就可以推导出其他各种滤波器参数。
///
#pragma once

#include <vector>
#include <cmath>
#include <complex>
#include <cassert>
#include "Sec2.h"

#ifdef _MSC_VER
#include <corecrt_math_defines.h>
#endif

typedef std::complex<double> complex_double;

namespace ys
{
    class ButterWorth
    {
    public:
        bool LowPass(std::vector<Sec2>& coeffs, double& gain, int filterOrder, double fs, double f1)
        {
            assert(filterOrder > 0);
            assert(fs > 0 && f1 > 0);
            assert(fs >= f1 * 2);

            gain = 1;
            coeffs.clear();

            auto aps = NormalizeAnalogLP(filterOrder);
            auto omega = tan(M_PI* f1 / fs);
            auto omega2 = omega* omega;

            // LP分子为gain，无零点
            gain = pow(omega, filterOrder);

            for (int i = 0; i < (int)aps.size(); i += 2)
            {
                const auto& ap = aps[i];

                if ((i + 1) == (int)aps.size())
                {
                    //最后剩余的1阶节
                    auto den = omega* ap.real() - 1;
                    auto subGain = ap.real() * omega / den;

                    Sec2 sec;
                    sec.gain = subGain;
                    sec.b0 = 1;
                    sec.b1 = 1;
                    sec.b2 = 0;
                    sec.a0 = 1;
                    sec.a1 = (omega* ap.real() + 1) / den;
                    sec.a2 = 0;
                    coeffs.push_back(sec);

                }
                else
                {
                    //由共轭复数根组成的2阶节
                    auto rnew = omega* ap.real();
                    auto den = 1 + omega2 - 2 * rnew;
                    auto subGain = omega2 / den;

                    Sec2 sec;
                    sec.gain = subGain;
                    sec.b0 = 1;
                    sec.b1 = 2;
                    sec.b2 = 1;
                    sec.a0 = 1;
                    sec.a1 = (2 * omega2 - 2) / den;
                    sec.a2 = (1 + omega2 + 2 * rnew) / den;
                    coeffs.push_back(sec);
                }
            }
            return true;
        }

        bool HighPass(std::vector<Sec2>& coeffs, double& gain, int filterOrder, double fs, double f1)
        {
            assert(filterOrder > 0);
            assert(fs > 0 && f1 > 0);
            assert(fs >= f1 * 2);

            gain = 1;
            coeffs.clear();

            auto aps = NormalizeAnalogLP(filterOrder);
            auto omega = tan(M_PI* f1 / fs);
            auto omega2 = omega* omega;

            for (int i = 0; i < (int)aps.size(); i += 2)
            {
                const auto& ap = aps[i];

                if ((i + 1) == (int)aps.size())
                {
                    //最后配对剩余的1阶节
                    auto den = omega - ap.real();
                    auto subGain = 1 / den;

                    Sec2 sec;
                    sec.gain = subGain;
                    sec.b0 = 1;
                    sec.b1 = -1;
                    sec.b2 = 0;
                    sec.a0 = 1;
                    sec.a1 = (omega + ap.real()) / den;
                    sec.a2 = 0;
                    coeffs.push_back(sec);

                    gain *= subGain;

                }
                else
                {
                    //配对形成的2阶节
                    auto rnew = omega* ap.real();
                    auto den = 1 + omega2 - 2 * rnew;
                    auto subGain = 1 / den;

                    Sec2 sec;
                    sec.gain = subGain;
                    sec.b0 = 1;
                    sec.b1 = -2;
                    sec.b2 = 1;
                    sec.a0 = 1;
                    sec.a1 = (2 * omega2 - 2) / den;
                    sec.a2 = (1 + omega2 + 2 * rnew) / den;
                    coeffs.push_back(sec);

                    gain *= subGain;
                }
            }
            return true;
        }

        bool BandPass(std::vector<Sec2>& coeffs, double& gain, int filterOrder, double fs, double f1, double f2)
        {
            assert(filterOrder > 0 && (filterOrder % 2 == 0));//必须
            assert(fs > 0 && f1 > 0 && f2 > 0);
            assert(fs >= f1 * 2);
            assert(fs >= f2 * 2);

            gain = 1;
            coeffs.clear();

            filterOrder /= 2;

            auto aps = NormalizeAnalogLP(filterOrder);

            auto omega1 = tan(M_PI* f1 / fs);
            auto omega2 = tan(M_PI* f2 / fs);
            auto B = omega2 - omega1;
            auto C = omega2* omega1;

            for (int i = 0; i < (int)aps.size(); i += 2)
            {
                const auto& ap = aps[i];

                if ((i + 1) == (int)aps.size())
                {
                    //配对剩余的单个2阶节, 只能落于实数负半轴上，p肯定是实数，其实是-1
                    auto denE = 1 + B + C;
                    auto gainE = B / denE;

                    Sec2 sece;
                    sece.gain = gainE;
                    sece.b0 = 1;
                    sece.b1 = 0;
                    sece.b2 = -1;
                    sece.a0 = 1;
                    sece.a1 = (-2.0 + 2 * C) / denE;
                    sece.a2 = (1.0 - B + C) / denE;
                    coeffs.push_back(sece);

                    gain *= gainE;
                }
                else
                {
                    //配对形成的4阶节，再重新分解配对后形成2组2阶节
                    auto E = (B* ap + sqrt(B* B* ap* ap - 4 * C)) * 0.5;
                    auto F = (B* ap - sqrt(B* B* ap* ap - 4 * C)) * 0.5;

                    ////////////第一个2阶节
                    auto E2 = abs(E) * abs(E);
                    auto denE = 1 - 2 * E.real() + E2;
                    auto gainE = B / denE;

                    Sec2 sece;
                    sece.gain = gainE;
                    sece.b0 = 1;
                    sece.b1 = 0;
                    sece.b2 = -1;
                    sece.a0 = 1;
                    sece.a1 = (-2.0 + 2 * E2) / denE;
                    sece.a2 = (1.0 + 2 * E.real() + E2) / denE;

                    gain *= gainE;
                    ///////////////////////////////////

                    ////////////第二个2阶节
                    auto F2 = abs(F) * abs(F);
                    auto denF = 1 - 2 * F.real() + F2;
                    auto gainF = B / denF;

                    Sec2 secf;
                    secf.gain = gainF;
                    secf.b0 = 1;
                    secf.b1 = 0;
                    secf.b2 = -1;
                    secf.a0 = 1;
                    secf.a1 = (-2.0 + 2 * F2) / denF;
                    secf.a2 = (1 + 2 * F.real() + F2) / denF;

                    gain *= gainF;
                    ///////////////////////////////////

                    sece.gain = secf.gain = sqrt(gain);
                    coeffs.push_back(sece);
                    coeffs.push_back(secf);
                }
            }
            return true;
        }

        bool BandStop(std::vector<Sec2>& coeffs, double& gain, int filterOrder, double fs, double f1, double f2)
        {
            assert(filterOrder > 0 && (filterOrder % 2 == 0));//必须
            assert(fs > 0 && f1 > 0 && f2 > 0);
            assert(fs >= f1 * 2);
            assert(fs >= f2 * 2);

            gain = 1;
            coeffs.clear();

            filterOrder /= 2;

            auto aps = NormalizeAnalogLP(filterOrder);

            auto omega1 = tan(M_PI* f1 / fs);
            auto omega2 = tan(M_PI* f2 / fs);
            auto B = omega2 - omega1;
            auto C = omega2* omega1;

            for (int i = 0; i < (int)aps.size(); i += 2)
            {
                const auto& ap = aps[i];

                if ((i + 1) == (int)aps.size())
                {
                    //配对剩余的单个2阶节, 只能落于实数负半轴上，p肯定是实数，其实是-1
                    auto denE = 1 + B + C;
                    auto numE = 1 + C;
                    auto gainE = numE / denE;

                    Sec2 sece;
                    sece.gain = gainE;
                    sece.b0 = 1;
                    sece.b1 = (-2 + 2 * C) / numE;
                    sece.b2 = 1;
                    sece.a0 = 1;
                    sece.a1 = (-2.0 + 2 * C) / denE;
                    sece.a2 = (1.0 - B + C) / denE;
                    coeffs.push_back(sece);

                    gain *= gainE;
                }
                else
                {
                    //配对形成的4阶节，再重新分解配对后形成2组2阶节
                    auto E = (-B + sqrt(B* B - 4 * C* ap* ap)) / (2.0 * ap);
                    auto F = (-B - sqrt(B* B - 4 * C* ap* ap)) / (2.0 * ap);

                    ////////////第一个2阶节
                    auto E2 = abs(E) * abs(E);
                    auto denE = 1 + 2 * E.real() + E2;
                    auto numE = 1 + C;
                    auto gainE = numE / denE;

                    Sec2 sece;
                    sece.gain = gainE;
                    sece.b0 = 1;
                    sece.b1 = (-2 + 2 * C) / numE;
                    sece.b2 = 1;
                    sece.a0 = 1;
                    sece.a1 = (-2.0 + 2 * E2) / denE;
                    sece.a2 = (1.0 - 2 * E.real() + E2) / denE;

                    gain *= gainE;
                    ///////////////////////////////////

                    ////////////第二个2阶节
                    auto F2 = abs(F) * abs(F);
                    auto denF = 1 + 2 * F.real() + F2;
                    auto numF = 1 + C;
                    auto gainF = numF / denF;

                    Sec2 secf;
                    secf.gain = gainF;
                    secf.b0 = 1;
                    secf.b1 = (-2 + 2 * C) / numF;
                    secf.b2 = 1;
                    secf.a0 = 1;
                    secf.a1 = (-2.0 + 2 * F2) / denF;
                    secf.a2 = (1.0 - 2 * F.real() + F2) / denF;

                    gain *= gainF;
                    ///////////////////////////////////

                    sece.gain = secf.gain = sqrt(gain);
                    coeffs.push_back(sece);
                    coeffs.push_back(secf);
                }
            }
            return true;
        }

    private:
        //低通滤波器p平面极点，肯定是左半平面的，否则不稳定
        std::vector<complex_double> NormalizeAnalogLP(int order)
        {
            bool isOdd = order % 2;
            int last = (order + 1) / 2 - 1;

            std::vector <complex_double> polar;
            //共轭复数，求取一半极点，另一半用共轭复数补充
            for (int i = 0; i <= last; i++)
            {
                double angle = (double)(0.5 + (0.5 + i) / order) * M_PI;
                double real = cos(angle);
                double image = sin(angle);
                polar.push_back(complex_double(real,  image));
                if (isOdd && i == last)//order是奇数时，最后一个极点是-1
                {
                    break;    //-pi，实数，非复数
                }
                polar.push_back(complex_double(real, -image)); // conjugate
            }
            return polar;
        }
    };
}
