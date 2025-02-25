#ifndef RESEAUSHOWFILTER_H
#define RESEAUSHOWFILTER_H

#include <vtkPolyDataAlgorithm.h>
#include <vtkAlgorithmOutput.h>

class ReseauShowFilter : public vtkPolyDataAlgorithm
{
public:
    vtkTypeMacro(ReseauShowFilter, vtkPolyDataAlgorithm);
    static ReseauShowFilter* New();

    vtkSetMacro(ShowSurface, bool);
    vtkGetMacro(ShowSurface, bool);
protected:
    ReseauShowFilter();
    ~ReseauShowFilter() override;

    int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
    int FillInputPortInformation(int port, vtkInformation* info) override;


private:
    ReseauShowFilter(const ReseauShowFilter&) = delete;
    void operator=(const ReseauShowFilter&) = delete;

private:

    bool ShowSurface = false;

};

#endif // RESEAUSHOWFILTER_H
