#ifndef DILATEWORKLET_H
#define DILATEWORKLET_H

class DilateErode {
public:
    DilateErode(int kernelSize, int dim[3]);
    void operator()(const int* buffer, int* outBuffer, int bufferSize);

private:
    int m_kernelSize = 3;
    int m_dim[3] {0,0,0};
};

#endif // DILATEWORKLET_H
