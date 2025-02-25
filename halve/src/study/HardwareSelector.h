#ifndef HARDWARESELECTOR_H
#define HARDWARESELECTOR_H

#include <QPoint>
#include <vtkOpenGLHardwareSelector.h>

class vtkSelection;

class HardwareSelector : public vtkOpenGLHardwareSelector {
 public:
    static HardwareSelector* New();
  vtkTypeMacro(HardwareSelector, vtkOpenGLHardwareSelector);

    /**
   * Perform the selection over the specified region.
   * Avoid clearing of captured buffers.
   */
    vtkSelection* select(int region[4]);
    vtkSelection* select(const QList<QPoint> &points);
    vtkSelection* select(int x, int y);
    bool prepare();

    // Fixes a -Woverloaded-virtual warning.
    using vtkOpenGLHardwareSelector::BeginRenderProp;

protected:
    HardwareSelector() = default;
    ~HardwareSelector() override = default;


private:
    HardwareSelector(const HardwareSelector&) = delete;
    void operator=(const HardwareSelector&) = delete;

};

#endif // HARDWARESELECTOR_H
