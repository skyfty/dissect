#include "iir_filter.h"
#include <string.h>

IIR_Filter::IIR_Filter(double* num, double* den)
{
	memset(Xin_Reg, 0, sizeof(Xin_Reg));
	memset(Yin_Reg, 0, sizeof(Yin_Reg));
	memcpy(num_p, num, sizeof(num_p));
	memcpy(den_p, den, sizeof(den_p));
}


double IIR_Filter::filter(double data_in)
{
    double dout = 0;
    //shift Xin_Reg
    for (int j = 4; j >= 1; j--)
    {
        Xin_Reg[j] = Xin_Reg[j - 1];
    }
    Xin_Reg[0] = data_in;
    //convolution with feedback
    for (int j = 0; j < 5; j++)
    {
        if (j < 4)
            dout += num_p[j] * Xin_Reg[j] - den_p[j + 1] * Yin_Reg[j];
        else
            dout += num_p[j] * Xin_Reg[j];
    }
    for (int j = 3; j >= 1; j--)
    {
        Yin_Reg[j] = Yin_Reg[j - 1];
    }
    Yin_Reg[0] = dout;

    return dout;

}
