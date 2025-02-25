#ifndef MESHCOLORTRANSFER_H
#define MESHCOLORTRANSFER_H
#include "vtkColorTransferFunction.h"
#include "vtkScalarsToColors.h"
#include "HalveType.h"
#include <vtkSmartPointer.h>

class StageScalar;
class Reseau;
class vtkAlgorithmOutput;
class Mapping;

class MeshColorTransfer : public vtkScalarsToColors
{
public:
    vtkTypeMacro(MeshColorTransfer, vtkScalarsToColors);
    static MeshColorTransfer* New();

    MeshColorTransfer() = default;
    ~MeshColorTransfer() = default;

    void Build() override;

    void setStageScalar(StageScalar *stageScalar) {
        m_stageScalar = stageScalar;
    }
    void setReseau(Reseau* reseau) {
        m_reseau = reseau;
    }
    vtkUnsignedCharArray* MapScalars(vtkAbstractArray* scalars, int colorMode, int component, int outputFormat = VTK_RGBA) override;
private:
    StageScalar *m_stageScalar = nullptr;
    Reseau* m_reseau = nullptr;
};

#endif // MESHCOLORTRANSFER_H
