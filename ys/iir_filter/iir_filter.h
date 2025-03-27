#ifndef _IIR_Filter_H_
#define _IIR_Filter_H_

class IIR_Filter {
public:
    IIR_Filter(double* num, double* den);
    double filter(double data_in);
private:
    double num_p[5];
    double den_p[5];
    double Xin_Reg[5] = { 0 };
    double Yin_Reg[4] = { 0 };
};

#endif	///_IIR_Filter_H_