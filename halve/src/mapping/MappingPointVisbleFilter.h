#ifndef MAPPINGPOINTVISBLEFILTER_H
#define MAPPINGPOINTVISBLEFILTER_H

#include <vtkPolyDataAlgorithm.h>

class MappingPointsDb;

class MappingPointVisbleFilter : public vtkPolyDataAlgorithm
{
public:
    vtkTypeMacro(MappingPointVisbleFilter, vtkPolyDataAlgorithm);
    static MappingPointVisbleFilter* New();
    void SetMappingPointsDb(MappingPointsDb *mappingPointsDb);

protected:
    MappingPointVisbleFilter();
    ~MappingPointVisbleFilter() override;
    int FillInputPortInformation(int port, vtkInformation* info) override;

    int RequestData(vtkInformation* request, vtkInformationVector** inputVector,
                    vtkInformationVector* outputVector) override;

    vtkMTimeType GetMTime() override;

private:
    MappingPointVisbleFilter(const MappingPointVisbleFilter&) = delete;
    void operator=(const MappingPointVisbleFilter&) = delete;

private:
    MappingPointsDb *m_mappingPointsDb = nullptr;;
};

#endif // MAPPINGPOINTVISBLEFILTER_H
