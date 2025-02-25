#ifndef STAGERENDERER_H
#define STAGERENDERER_H

#include <vtkRenderer.h>


class StageRenderer : public vtkRenderer
{
public:
    vtkTypeMacro(StageRenderer, vtkRenderer);
    static StageRenderer* New();

protected:
    StageRenderer();
    ~StageRenderer() = default;
private:
    StageRenderer(const StageRenderer&) = delete;
    void operator=(const StageRenderer&) = delete;
};

#endif // STAGERENDERER_H
