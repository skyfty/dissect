#ifndef DISTANCEWIDGET_H
#define DISTANCEWIDGET_H


#include "vtkAbstractWidget.h"
#include "vtkInteractionWidgetsModule.h" // For export macro

#include <vtkDistanceRepresentation2D.h>
#include <vtkPointHandleRepresentation2D.h>
#include <vtkSmartPointer.h>

class vtkDistanceRepresentation;
class vtkHandleWidget;
class DistanceWidgetCallback;
class vtkWorldPointPicker;

class PointHandleRepresentation2D : public vtkPointHandleRepresentation2D {
public:
    /**
     * Instantiate this class.
     */
    static PointHandleRepresentation2D* New();
    ///@{
    /**
     * Standard methods for a VTK class.
     */
    vtkTypeMacro(PointHandleRepresentation2D, vtkPointHandleRepresentation2D);

    public:
    void GetWorldPosition(double pos[3]) override;
    void SetWorldPosition(double pos[3]) override;
};

class DistanceRepresentation2D : public vtkDistanceRepresentation2D {

public:
    /**
     * Instantiate this class.
     */
    static DistanceRepresentation2D* New();
    ///@{
    /**
     * Standard methods for a VTK class.
     */
    vtkTypeMacro(DistanceRepresentation2D, vtkDistanceRepresentation2D);


    
  /**
     * Method to satisfy superclasses' API.
     */
    void BuildRepresentation() override;

    
protected:
    DistanceRepresentation2D();
    ~DistanceRepresentation2D() override;

    vtkSmartPointer<vtkWorldPointPicker> worldPointPicker;
};

class DistanceWidget : public vtkAbstractWidget {
public:
    static DistanceWidget* New();

    vtkTypeMacro(DistanceWidget, vtkAbstractWidget);

    void SetEnabled(int) override;
    void SetRepresentation(vtkDistanceRepresentation* r)
    {
        this->Superclass::SetWidgetRepresentation(reinterpret_cast<vtkWidgetRepresentation*>(r));
    }
 
    void CreateDefaultRepresentation() override;
    void SetProcessEvents(vtkTypeBool) override;

    enum {
        Start = 0,
        Define
    };
    virtual int GetWidgetState() { return this->WidgetState; }
    
protected:
    DistanceWidget();
    ~DistanceWidget() override;
    int WidgetState;
    int CurrentHandle;

    static void AddPointAction(vtkAbstractWidget*);
    static void MoveAction(vtkAbstractWidget*);
    static void EndSelectAction(vtkAbstractWidget*);
    static void AddPointAction3D(vtkAbstractWidget*);
    static void MoveAction3D(vtkAbstractWidget*);
    static void EndSelectAction3D(vtkAbstractWidget*);


    vtkHandleWidget* Point1Widget;
    vtkHandleWidget* Point2Widget;
    DistanceWidgetCallback* DistanceWidgetCallback1;
    DistanceWidgetCallback* DistanceWidgetCallback2;


    void StartDistanceInteraction(int handleNum);
    void DistanceInteraction(int handleNum);
    void EndDistanceInteraction(int handleNum);

    friend class DistanceWidgetCallback;
};

#endif // DISTANCEWIDGET_H
