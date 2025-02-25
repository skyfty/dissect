#include "HardwareSelector.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"

#include "vtkObjectFactory.h"
#include <QList>
#include <vtkIntArray.h>
#include <vtkOpenGLRenderWindow.h>
#include <vtkOpenGLState.h>

//----------------------------------------------------------------------------
vtkStandardNewMacro(HardwareSelector);

bool HardwareSelector::prepare() {
    int* size = this->Renderer->GetSize();
    SetArea(0,0, size[0] - 1, size[1] - 1);
    return CaptureBuffers();
}

class RendererDrawFlagsHelper {
public:
    RendererDrawFlagsHelper(vtkRenderer* renderer):m_renderer(renderer){
   
        this->m_renderer->SetDraw(false);
    }
    ~RendererDrawFlagsHelper(){
        this->m_renderer->SetDraw(true);
    }

private:
    vtkRenderer* m_renderer;
};

vtkSelection* HardwareSelector::select(const QList<QPoint> &points) {
    RendererDrawFlagsHelper drawFlagsHelper(this->Renderer);
    const int* windowSize = this->Renderer->GetRenderWindow()->GetSize();
    vtkSmartPointer<vtkIntArray> polygonPointsArray = vtkSmartPointer<vtkIntArray>::New();
    polygonPointsArray->SetNumberOfComponents(2);
    polygonPointsArray->SetNumberOfTuples(points.size());
    for (unsigned int j = 0; j < points.size(); ++j) {
        int pos[2] = { points[j].x(),points[j].y() };
        polygonPointsArray->SetTypedTuple(j, pos);
    }
    return GeneratePolygonSelection(polygonPointsArray->GetPointer(0), polygonPointsArray->GetNumberOfValues());
}

vtkSelection* HardwareSelector::select(int x, int y) {
    RendererDrawFlagsHelper drawFlagsHelper(this->Renderer);
    int region[4] = { x,y, x, y };
    return select(region);
}

vtkSelection* HardwareSelector::select(int region[4]) {
    RendererDrawFlagsHelper drawFlagsHelper(this->Renderer);
    return GenerateSelection(region[0], region[1], region[2], region[3]);
}