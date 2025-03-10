
#include "study/Stage.h"

#include "StageData.h"
#include "vtkRenderer.h"

#include <vtkProperty.h>
#include <QThread>
#include "combined/Bound.h"
#include <vtkCubeAxesActor.h>
#include <vtkTextProperty.h>
#include <vtkStringArray.h>

void Stage::showAxesActor(StageData* userData) {
    bool b = m_stageOptions->showAxes();

    if (b && userData->axesActor == nullptr) {
        initCubeAxesActor(userData);
        userData->axesActor->SetCamera(userData->renderer->GetActiveCamera());
        userData->renderer->AddActor(userData->axesActor);
    } else if (!b && userData->axesActor != nullptr) {
        userData->renderer->RemoveActor(userData->axesActor);
        userData->axesActor = nullptr;
    }
    if (userData->axesActor != nullptr) {
        setAxesProperty(userData);
    }
}

void Stage::initCubeAxesActor(StageData* userData) {
    Q_ASSERT(m_vtkRenderThreadId == QThread::currentThreadId());
    userData->axesActor = vtkSmartPointer<vtkCubeAxesActor>::New();
    userData->axesActor->SetGridLineLocation(vtkCubeAxesActor::VTK_GRID_LINES_FURTHEST);
    userData->axesActor->SetFlyModeToOuterEdges();
    userData->axesActor->DrawXGridlinesOn();
    userData->axesActor->DrawYGridlinesOn();
    userData->axesActor->DrawZGridlinesOn();
    userData->axesActor->XAxisMinorTickVisibilityOn();
    userData->axesActor->YAxisMinorTickVisibilityOn();
    userData->axesActor->ZAxisMinorTickVisibilityOn();
    userData->axesActor->SetBounds(combined::getHoldingBound());
}

constexpr double lineWidth = 1;
static double lineColor[] = { 0.8, 0.8, 0.8 };
static double labelColor[] = { 1, 1, 1 };
static int fontSize = 15;

void Stage::setAxesProperty(StageData* userData) {
    userData->axesActor->SetUseAxisOrigin(m_stageOptions->useAxisOrigin());
    userData->axesActor->GetXAxesLinesProperty()->SetLineWidth(lineWidth);
    userData->axesActor->GetXAxesLinesProperty()->SetColor(lineColor);
    userData->axesActor->GetYAxesLinesProperty()->SetLineWidth(lineWidth);
    userData->axesActor->GetYAxesLinesProperty()->SetColor(lineColor);
    userData->axesActor->GetZAxesLinesProperty()->SetLineWidth(lineWidth);
    userData->axesActor->GetZAxesLinesProperty()->SetColor(lineColor);

    userData->axesActor->GetXAxesGridlinesProperty()->SetColor(lineColor);
    userData->axesActor->GetYAxesGridlinesProperty()->SetColor(lineColor);
    userData->axesActor->GetZAxesGridlinesProperty()->SetColor(lineColor);

    for (int i = 0; i < 3; ++i) {
        vtkTextProperty* titleProperty = userData->axesActor->GetTitleTextProperty(i);
        titleProperty->SetColor(labelColor);
        titleProperty->SetFontFamilyToArial();;
        titleProperty->SetFontSize(fontSize);
        userData->axesActor->GetLabelTextProperty(i)->SetColor(labelColor);
        userData->axesActor->GetLabelTextProperty(i)->SetFontSize(fontSize);
    }

    bool showInnerGrid = m_stageOptions->showInnerGridlines();
    userData->axesActor->SetDrawXInnerGridlines(showInnerGrid);
    userData->axesActor->SetDrawYInnerGridlines(showInnerGrid);
    userData->axesActor->SetDrawZInnerGridlines(showInnerGrid);
}
