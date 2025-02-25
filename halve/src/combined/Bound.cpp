#include "Bound.h"

#include <vtkBoundingBox.h>
#include <vtkPoints.h>

static double effectiveBound[] = { -200,200, -200, 200, 0, 400 };
static double holdingBound[] = { -100,100, -100, 100, -100, 100 };
namespace combined {
double* getEffectiveBound() {
    return effectiveBound;
}
double* getHoldingBound() {
    return holdingBound;
}
bool inHoldingBound(vtkPoints* points) {
    for(int i = 0; i < points->GetNumberOfPoints(); ++i) {
        if (!inHoldingBound(points->GetPoint(i))) {
            return false;
        }
    }
    return true;
}
bool inHoldingBound(double pos[3]) {
    static vtkBoundingBox boundingBox(combined::getHoldingBound());
    return boundingBox.ContainsPoint(pos);
}

}
