#include "DistanceWidget.h"

#include "vtkNew.h"
#include "vtkObjectFactory.h"
#include "vtkDistanceRepresentation2D.h"
#include "vtkAxisActor2D.h"
#include "vtkHandleRepresentation.h"
#include "vtkTextProperty.h"

#include <qDebug>
#include <vtkMath.h>
#include <vtkRenderer.h>
#include <vtkWindow.h>
#include <vtkPointPlacer.h>
#include <vtkCommand.h>
#include <vtkWidgetEvent.h>
#include <vtkWidgetCallbackMapper.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCallbackCommand.h>
#include <vtkHandleWidget.h>
#include <vtkWorldPointPicker.h>

#include "vtkWidgetEvent.h"
#include "vtkWidgetEventTranslator.h"

vtkStandardNewMacro(DistanceWidget);

vtkStandardNewMacro(DistanceRepresentation2D);
vtkStandardNewMacro(PointHandleRepresentation2D);

class DistanceWidgetCallback : public vtkCommand {
public:
    static DistanceWidgetCallback* New() { return new DistanceWidgetCallback; }
    void Execute(vtkObject*, unsigned long eventId, void*) override
    {
        switch (eventId) {
        case vtkCommand::StartInteractionEvent:
            this->DistanceWidget->StartDistanceInteraction(this->HandleNumber);
            break;
        case vtkCommand::InteractionEvent:
            this->DistanceWidget->DistanceInteraction(this->HandleNumber);
            break;
        case vtkCommand::EndInteractionEvent:
            this->DistanceWidget->EndDistanceInteraction(this->HandleNumber);
            break;
        }
    }
    int HandleNumber;
    DistanceWidget* DistanceWidget;
};

//------------------------------------------------------------------------------
void DistanceWidget::CreateDefaultRepresentation()
{
    if (!this->WidgetRep) {
        this->WidgetRep = DistanceRepresentation2D::New();
    }
    reinterpret_cast<vtkDistanceRepresentation*>(this->WidgetRep)->InstantiateHandleRepresentation();
}
DistanceWidget::DistanceWidget() {

    this->ManagesCursor = 0;

    this->WidgetState = DistanceWidget::Start;

    // The widgets for moving the end points. They observe this widget (i.e.,
    // this widget is the parent to the handles).
    this->Point1Widget = vtkHandleWidget::New();
    this->Point1Widget->SetParent(this);
    this->Point2Widget = vtkHandleWidget::New();
    this->Point2Widget->SetParent(this);

    // Set up the callbacks on the two handles
    this->DistanceWidgetCallback1 = DistanceWidgetCallback::New();
    this->DistanceWidgetCallback1->HandleNumber = 0;
    this->DistanceWidgetCallback1->DistanceWidget = this;
    this->Point1Widget->AddObserver(
        vtkCommand::StartInteractionEvent, this->DistanceWidgetCallback1, this->Priority);
    this->Point1Widget->AddObserver(
        vtkCommand::InteractionEvent, this->DistanceWidgetCallback1, this->Priority);
    this->Point1Widget->AddObserver(
        vtkCommand::EndInteractionEvent, this->DistanceWidgetCallback1, this->Priority);

    this->DistanceWidgetCallback2 = DistanceWidgetCallback::New();
    this->DistanceWidgetCallback2->HandleNumber = 1;
    this->DistanceWidgetCallback2->DistanceWidget = this;
    this->Point2Widget->AddObserver(
        vtkCommand::StartInteractionEvent, this->DistanceWidgetCallback2, this->Priority);
    this->Point2Widget->AddObserver(
        vtkCommand::InteractionEvent, this->DistanceWidgetCallback2, this->Priority);
    this->Point2Widget->AddObserver(
        vtkCommand::EndInteractionEvent, this->DistanceWidgetCallback2, this->Priority);

    // These are the event callbacks supported by this widget
    this->CallbackMapper->SetCallbackMethod(vtkCommand::LeftButtonPressEvent,
        vtkWidgetEvent::AddPoint, this, DistanceWidget::AddPointAction);
    this->CallbackMapper->SetCallbackMethod(
        vtkCommand::MouseMoveEvent, vtkWidgetEvent::Move, this, DistanceWidget::MoveAction);
    this->CallbackMapper->SetCallbackMethod(vtkCommand::LeftButtonReleaseEvent,
        vtkWidgetEvent::EndSelect, this, DistanceWidget::EndSelectAction);
}

DistanceWidget::~DistanceWidget() {
    this->Point1Widget->RemoveObserver(this->DistanceWidgetCallback1);
    this->Point1Widget->Delete();
    this->DistanceWidgetCallback1->Delete();

    this->Point2Widget->RemoveObserver(this->DistanceWidgetCallback2);
    this->Point2Widget->Delete();
    this->DistanceWidgetCallback2->Delete();
}

//------------------------------------------------------------------------------
void DistanceWidget::SetEnabled(int enabling)
{
    // The handle widgets are not actually enabled until they are placed.
    // The handle widgets take their representation from the
    // vtkDistanceRepresentation.
    if (enabling) {
        if (this->WidgetState == DistanceWidget::Start) {
            reinterpret_cast<vtkDistanceRepresentation*>(this->WidgetRep)->VisibilityOff();
        } else {
            // The interactor must be set prior to enabling the widget.
            if (this->Interactor) {
                this->Point1Widget->SetInteractor(this->Interactor);
                this->Point2Widget->SetInteractor(this->Interactor);
            }

            this->Point1Widget->SetEnabled(1);
            this->Point2Widget->SetEnabled(1);
        }
    }

    if (enabling) //----------------
    {
        if (this->Enabled) // already enabled, just return
        {
            return;
        }

        if (!this->Interactor) {
            vtkErrorMacro(<< "The interactor must be set prior to enabling the widget");
            return;
        }

        int X = this->Interactor->GetEventPosition()[0];
        int Y = this->Interactor->GetEventPosition()[1];

        if (!this->CurrentRenderer) {
            this->SetCurrentRenderer(this->Interactor->FindPokedRenderer(X, Y));
            if (this->CurrentRenderer == nullptr) {
                return;
            }
        }

        // We're ready to enable
        this->Enabled = 1;
        this->CreateDefaultRepresentation();
        this->WidgetRep->SetRenderer(this->CurrentRenderer);

        // Set the renderer, interactor and representation on the two handle
        // widgets.
        this->Point1Widget->SetRepresentation(
            reinterpret_cast<vtkDistanceRepresentation*>(this->WidgetRep)->GetPoint1Representation());
        this->Point1Widget->SetInteractor(this->Interactor);
        this->Point1Widget->GetRepresentation()->SetRenderer(this->CurrentRenderer);

        this->Point2Widget->SetRepresentation(
            reinterpret_cast<vtkDistanceRepresentation*>(this->WidgetRep)->GetPoint2Representation());
        this->Point2Widget->SetInteractor(this->Interactor);
        this->Point2Widget->GetRepresentation()->SetRenderer(this->CurrentRenderer);

        // listen for the events found in the EventTranslator
        if (!this->Parent) {
            this->EventTranslator->AddEventsToInteractor(
                this->Interactor, this->EventCallbackCommand, this->Priority);
        } else {
            this->EventTranslator->AddEventsToParent(
                this->Parent, this->EventCallbackCommand, this->Priority);
        }

        if (this->ManagesCursor) {
            this->WidgetRep->ComputeInteractionState(X, Y);
            this->SetCursor(this->WidgetRep->GetInteractionState());
        }

        this->WidgetRep->BuildRepresentation();
        this->CurrentRenderer->AddViewProp(this->WidgetRep);

        if (this->WidgetState == DistanceWidget::Start) {
            reinterpret_cast<vtkDistanceRepresentation*>(this->WidgetRep)->VisibilityOff();
        } else {
            this->Point1Widget->SetEnabled(1);
            this->Point2Widget->SetEnabled(1);
        }

        this->InvokeEvent(vtkCommand::EnableEvent, nullptr);
    }

    else // disabling------------------
    {
        vtkDebugMacro(<< "Disabling widget");

        if (!this->Enabled) // already disabled, just return
        {
            return;
        }

        this->Enabled = 0;

        // don't listen for events any more
        if (!this->Parent) {
            this->Interactor->RemoveObserver(this->EventCallbackCommand);
        } else {
            this->Parent->RemoveObserver(this->EventCallbackCommand);
        }

        this->CurrentRenderer->RemoveViewProp(this->WidgetRep);

        this->Point1Widget->SetEnabled(0);
        this->Point2Widget->SetEnabled(0);

        this->InvokeEvent(vtkCommand::DisableEvent, nullptr);
        this->SetCurrentRenderer(nullptr);
    }

    // Should only render if there is no parent
    if (this->Interactor && !this->Parent) {
        this->Interactor->Render();
    }
}

// The following methods are the callbacks that the measure widget responds to
//------------------------------------------------------------------------------
void DistanceWidget::AddPointAction(vtkAbstractWidget* w)
{
    DistanceWidget* self = reinterpret_cast<DistanceWidget*>(w);
    int X = self->Interactor->GetEventPosition()[0];
    int Y = self->Interactor->GetEventPosition()[1];

    // Freshly enabled and placing the first point
    if (self->WidgetState == DistanceWidget::Start) {
        self->GrabFocus(self->EventCallbackCommand);
        self->WidgetState = DistanceWidget::Define;
        self->InvokeEvent(vtkCommand::StartInteractionEvent, nullptr);
        reinterpret_cast<vtkDistanceRepresentation*>(self->WidgetRep)->VisibilityOn();
        double e[2];
        e[0] = static_cast<double>(X);
        e[1] = static_cast<double>(Y);
        reinterpret_cast<vtkDistanceRepresentation*>(self->WidgetRep)->StartWidgetInteraction(e);
        self->CurrentHandle = 0;
        self->InvokeEvent(vtkCommand::PlacePointEvent, &(self->CurrentHandle));
    } 
    // Clean up
    self->EventCallbackCommand->SetAbortFlag(1);
    self->Render();
}

//------------------------------------------------------------------------------
void DistanceWidget::EndSelectAction(vtkAbstractWidget* w)
{
    DistanceWidget* self = reinterpret_cast<DistanceWidget*>(w);
    self->ReleaseFocus();
    self->InvokeEvent(vtkCommand::LeftButtonReleaseEvent, nullptr);
    self->CurrentHandle = -1;
    self->WidgetState = DistanceWidget::Start;
    self->WidgetRep->BuildRepresentation();
    self->EventCallbackCommand->SetAbortFlag(1);
    self->Render();
}

//------------------------------------------------------------------------------
void DistanceWidget::MoveAction(vtkAbstractWidget* w)
{
    DistanceWidget* self = reinterpret_cast<DistanceWidget*>(w);

    if (self->WidgetState == DistanceWidget::Start) {
        return;
    }

    if (self->WidgetState == DistanceWidget::Define) {
        int X = self->Interactor->GetEventPosition()[0];
        int Y = self->Interactor->GetEventPosition()[1];
        double e[2];
        e[0] = static_cast<double>(X);
        e[1] = static_cast<double>(Y);
        reinterpret_cast<vtkDistanceRepresentation*>(self->WidgetRep)->WidgetInteraction(e);
        self->InvokeEvent(vtkCommand::InteractionEvent, nullptr);
        self->EventCallbackCommand->SetAbortFlag(1);
    } else 
    {
        self->InvokeEvent(vtkCommand::MouseMoveEvent, nullptr);
    }

    self->WidgetRep->BuildRepresentation();
    self->Render();
}

// These are callbacks that are active when the user is manipulating the
// handles of the measure widget.
//------------------------------------------------------------------------------
void DistanceWidget::StartDistanceInteraction(int)
{
    this->Superclass::StartInteraction();
    this->InvokeEvent(vtkCommand::StartInteractionEvent, nullptr);
}

//------------------------------------------------------------------------------
void DistanceWidget::DistanceInteraction(int)
{
    this->InvokeEvent(vtkCommand::InteractionEvent, nullptr);
}

//------------------------------------------------------------------------------
void DistanceWidget::EndDistanceInteraction(int)
{
    this->Superclass::EndInteraction();
    this->InvokeEvent(vtkCommand::EndInteractionEvent, nullptr);
}

//------------------------------------------------------------------------------
void DistanceWidget::SetProcessEvents(vtkTypeBool pe)
{
    this->Superclass::SetProcessEvents(pe);

    this->Point1Widget->SetProcessEvents(pe);
    this->Point2Widget->SetProcessEvents(pe);
}

void PointHandleRepresentation2D::GetWorldPosition(double pos[3]) {
    this->WorldPosition->GetValue(pos);
}
void PointHandleRepresentation2D::SetWorldPosition(double pos[3])
{
    if (this->Renderer && this->PointPlacer) {
        if (this->PointPlacer->ValidateWorldPosition(pos)) {
            this->WorldPosition->SetValue(pos);
            this->WorldPositionTime.Modified();
        }
    } else {
        this->WorldPosition->SetValue(pos);
        this->WorldPositionTime.Modified();
    }
}

DistanceRepresentation2D::~DistanceRepresentation2D() {

}

DistanceRepresentation2D::DistanceRepresentation2D() {
    if (this->HandleRepresentation) {
        this->HandleRepresentation->Delete();
    }
    this->HandleRepresentation = PointHandleRepresentation2D::New();
    this->worldPointPicker = vtkSmartPointer<vtkWorldPointPicker>::New();
}
    //------------------------------------------------------------------------------
void DistanceRepresentation2D::BuildRepresentation()
{
    if (this->GetMTime() > this->BuildTime || this->AxisActor->GetMTime() > this->BuildTime 
        || this->AxisActor->GetTitleTextProperty()->GetMTime() > this->BuildTime
        || this->Point1Representation->GetMTime() > this->BuildTime 
        || this->Point2Representation->GetMTime() > this->BuildTime 
        || (this->Renderer && this->Renderer->GetVTKWindow() && this->Renderer->GetVTKWindow()->GetMTime() > this->BuildTime)) {
        this->Superclass::BuildRepresentation();

        // Okay, compute the distance and set the label
        double dp1[3], p1[3];
        this->Point1Representation->GetDisplayPosition(dp1);
        this->worldPointPicker->Pick(dp1[0], dp1[1], 0, GetRenderer());
        this->worldPointPicker->GetPickPosition(p1);

        double dp2[3], p2[3];
        this->Point2Representation->GetDisplayPosition(dp2);
        this->worldPointPicker->Pick(dp2[0], dp2[1], 0, GetRenderer());
        this->worldPointPicker->GetPickPosition(p2);
        this->Distance = sqrt(vtkMath::Distance2BetweenPoints(p1, p2));

        this->AxisActor->GetPoint1Coordinate()->SetValue(p1);
        this->AxisActor->GetPoint2Coordinate()->SetValue(p2);
        this->AxisActor->SetRulerMode(this->RulerMode);
        if (this->Scale != 0.0) {
            this->AxisActor->SetRulerDistance(this->RulerDistance / this->Scale);
        }
        this->AxisActor->SetNumberOfLabels(this->NumberOfRulerTicks);

        char string[512];
        snprintf(string, sizeof(string), this->LabelFormat, this->Distance * this->Scale);
        this->AxisActor->SetTitle(string);

        this->BuildTime.Modified();
    }
}
