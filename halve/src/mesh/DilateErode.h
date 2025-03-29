#ifndef DILATEWORKLET_H
#define DILATEWORKLET_H

#include <vector>

class DilateErode {
public:
    DilateErode();
    void filter(const int* buffer, int* outBuffer, int bufferSize);

    void setDilateValue(int v);
    void setErodeValue(int v);
    void setKernelSize(int v);
    void setDimension(int v);

private:

    int m_dilateValue;
    int m_erodeValue;
    int m_kernelSize = 1;
    int m_dim = 0;
    std::vector<int> m_workBuffer;
};

#endif // DILATEWORKLET_H
