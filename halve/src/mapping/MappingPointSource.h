#ifndef MAPPINGPOINTSOURCE_H
#define MAPPINGPOINTSOURCE_H

#include <vtkPolyDataAlgorithm.h>

class MappingPointsDb;

class MappingPointSource : public vtkPolyDataAlgorithm {
public:
    vtkTypeMacro(MappingPointSource, vtkPolyDataAlgorithm);

    static MappingPointSource* New();
    void SetMappingPointsDb(MappingPointsDb *mappingPointsDb);

protected:
    MappingPointSource();
    ~MappingPointSource() override = default;
    int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
    vtkMTimeType GetMTime() override;


private:
    MappingPointSource(const MappingPointSource&) = delete;
    void operator=(const MappingPointSource&) = delete;

private:
    MappingPointsDb *m_mappingPointsDb = nullptr;;
};

#endif // MAPPINGPOINTSOURCE_H
