#ifndef MOUSEINTERACTORSTYLE_H
#define MOUSEINTERACTORSTYLE_H

#include <vtkInteractorStyleTrackballCamera.h>

class Stage;
class StageData;

// Handle mouse events
class MouseInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
    static MouseInteractorStyle* New();
    vtkTypeMacro(MouseInteractorStyle, vtkInteractorStyleTrackballCamera);
    void Zoom(double factor) ;

    void OnLeftButtonDown() override;
    void OnLeftButtonUp() override;
    void OnMouseMove() override;


    void OnMiddleButtonDown() override;
    void OnMiddleButtonUp() override;

    void OnRightButtonDown() override;
    void OnRightButtonUp() override;
    void OnMouseWheelForward() override;
    void OnMouseWheelBackward() override;

    /// @brief TODO Œ¥”√µΩ??? 
    Stage* stage = nullptr;
    StageData* userData = nullptr;
    bool m_middleButton = false;
    bool m_leftButton = false;
    bool m_enableZoom = true;
};


#endif // MOUSEINTERACTORSTYLE_H
