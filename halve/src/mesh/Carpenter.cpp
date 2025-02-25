#include "Carpenter.h"
#include "HalveType.h"
#include <QDateTime>
#include <vtkFlyingEdges3D.h>
#include <vtkImageData.h>
#include <vtkImageOpenClose3D.h>
#include <vtkPolyDataNormals.h>
#include <vtkWindowedSincPolyDataFilter.h>
#include "ImageDataRefineFilter.h"
#include "mesh/CarpenterSource.h"
#include <vtkPointData.h>

Carpenter::Carpenter(QObject *parent)
    : QObject{parent}
{
    initFilters();
}

void Carpenter::initFilters() {
    m_carpenterSource = vtkSmartPointer<CarpenterSource>::New();

    m_openCloseFilter = vtkSmartPointer<vtkImageOpenClose3D>::New();
    m_openCloseFilter->SetOpenValue(ScalarsReset);
    m_openCloseFilter->SetCloseValue(ScalarsSet);
    m_openCloseFilter->ReleaseDataFlagOff();
    m_openCloseFilter->SetInputConnection(m_carpenterSource->GetOutputPort());

    m_flyingEdgesFilter = vtkSmartPointer<vtkFlyingEdges3D>::New();
    m_flyingEdgesFilter->SetInputConnection(m_openCloseFilter->GetOutputPort());
    m_flyingEdgesFilter->ComputeNormalsOff();
    m_flyingEdgesFilter->ComputeScalarsOff();
    m_flyingEdgesFilter->ComputeGradientsOff();
    m_flyingEdgesFilter->InterpolateAttributesOff();
    m_flyingEdgesFilter->SetValue(0, ScalarsSet);

    m_refineFilter = vtkSmartPointer<ImageDataRefineFilter>::New();
    m_refineFilter->SetInputConnection(0, m_carpenterSource->GetOutputPort());
    m_refineFilter->SetInputConnection(1, m_flyingEdgesFilter->GetOutputPort());

    m_smoother = vtkSmartPointer<vtkWindowedSincPolyDataFilter>::New();
    m_smoother->SetInputConnection(m_refineFilter->GetOutputPort());
    m_smoother->BoundarySmoothingOn();
    m_smoother->FeatureEdgeSmoothingOff();
}

vtkSmartPointer<vtkImageData> Carpenter::getImageData() {
    m_openCloseFilter->Update();
    return m_openCloseFilter->GetOutput();
}

int Carpenter::iterations() const
{
    return m_iterations;
}

void Carpenter::setIterations(int newIterations)
{
    m_iterations = newIterations;
    m_smoother->SetNumberOfIterations(newIterations);
}

int Carpenter::kernelSize() const
{
    return m_kernelSize;
}

void Carpenter::setKernelSize(int newKernelSize)
{
    m_kernelSize = newKernelSize;
    m_openCloseFilter->SetKernelSize(newKernelSize, newKernelSize, newKernelSize);
}

void Carpenter::setInputData(vtkSmartPointer<vtkImageData> imageData)
{
    m_carpenterSource->SetInputData(imageData);
}

vtkSmartPointer<vtkPolyData> Carpenter::getPolyData() {
    m_smoother->Update();
    return m_smoother->GetOutput();
}

void Carpenter::modified() {
    Q_ASSERT(m_carpenterSource != nullptr);
    m_openCloseFilter->Modified();
}
