#ifndef CARPENTER_H
#define CARPENTER_H

#include <QObject>
#include <vtkSmartPointer.h>

class vtkImageOpenClose3D;
class vtkFlyingEdges3D;
class vtkWindowedSincPolyDataFilter;
class vtkPolyDataNormals;
class vtkImageData;
class vtkPolyData;
class ImageDataRefineFilter;
class CarpenterSource;


class Carpenter : public QObject
{
    Q_OBJECT
public:
    explicit Carpenter(QObject *parent = nullptr);

    void setInputData(vtkSmartPointer<vtkImageData> imageData);
    vtkSmartPointer<vtkPolyData> getPolyData();
    vtkSmartPointer<vtkImageData> getImageData();

    void modified();

    int kernelSize() const;
    void setKernelSize(int newKernelSize);

    int iterations() const;
    void setIterations(int newIterations);

private:
    void initFilters();

private:
    int m_kernelSize;
    int m_iterations;
    vtkSmartPointer<CarpenterSource> m_carpenterSource;
    vtkSmartPointer<vtkImageOpenClose3D> m_openCloseFilter;
    vtkSmartPointer<ImageDataRefineFilter> m_refineFilter;
    vtkSmartPointer<vtkFlyingEdges3D> m_flyingEdgesFilter;
    vtkSmartPointer<vtkWindowedSincPolyDataFilter> m_smoother;
    vtkSmartPointer<vtkPolyDataNormals> m_normalGenerator;
};

#endif // CARPENTER_H
