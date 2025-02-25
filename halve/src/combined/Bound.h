#ifndef CONFIG_H
#define CONFIG_H

class vtkPoints;

namespace combined {
    double* getEffectiveBound();
    double* getHoldingBound();
    bool inHoldingBound(vtkPoints* points);
    bool inHoldingBound(double pos[3]);

};

#endif // CONFIG_H
