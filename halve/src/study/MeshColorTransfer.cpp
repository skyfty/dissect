#include "MeshColorTransfer.h"
#include "dissolve/DissolveDb.h"
#include "mapping/MappingPointsDb.h"
#include "reseau/Reseau.h"
#include "vtkObjectFactory.h"
#include "StageScalar.h"
#include <vtkAbstractArray.h>
#include <vtkDataArray.h>
#include <vtkUnsignedCharArray.h>
#include "utility/VtkUtil.h"

vtkStandardNewMacro(MeshColorTransfer);

void MeshColorTransfer::Build() {
    Q_ASSERT(m_reseau != nullptr);
    Q_ASSERT(m_stageScalar != nullptr);
    auto mtime = GetMTime();
    if (m_reseau->GetMTime() > mtime || m_stageScalar->GetMTime() > mtime) {
        Modified();
    }
}

vtkUnsignedCharArray *MeshColorTransfer::MapScalars(vtkAbstractArray *scalars, int colorMode, int component, int outputFormat)
{
    Q_ASSERT(m_reseau != nullptr);
    Q_ASSERT(m_stageScalar != nullptr);
    vtkDataArray* dataArray = vtkArrayDownCast<vtkDataArray>(scalars);

    vtkUnsignedCharArray* colors = vtkUnsignedCharArray::New();
    colors->SetNumberOfComponents(3);
    colors->SetNumberOfTuples(scalars->GetNumberOfTuples());

    auto dyestuff = vtkutil::qColorToVtkColor3ub(m_reseau->dyestuff());
    for (vtkIdType i = 0; i < scalars->GetNumberOfTuples(); ++i) {
        double mappingPointId = dataArray->GetTuple1(i);
        if (mappingPointId < 0) {
            colors->SetTypedTuple(i, dyestuff.GetData());
        } else if (!m_stageScalar->getColor(mappingPointId, colors->GetPointer(3 * i))) {
            colors->SetTypedTuple(i, dyestuff.GetData());
        }
    }
    return colors;
}
