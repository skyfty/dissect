#ifndef MAPPINGLOCATIONSOURCE_H
#define MAPPINGLOCATIONSOURCE_H

#include <vtkPolyDataAlgorithm.h>

class MappingPointsDb;
class MappingLocationSource: public vtkPolyDataAlgorithm {
public:
    vtkTypeMacro(MappingLocationSource, vtkPolyDataAlgorithm);

    static MappingLocationSource* New();
    void SetMappingPointsDb(MappingPointsDb *mappingPointsDb);

protected:
    MappingLocationSource();
    ~MappingLocationSource() override = default;
    int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
    vtkMTimeType GetMTime() override;

private:
    MappingLocationSource(const MappingLocationSource&) = delete;
    void operator=(const MappingLocationSource&) = delete;

private:
    MappingPointsDb *m_mappingPointsDb = nullptr;;
    unsigned char m_defaultColor[3]{};
};
#endif // MAPPINGLOCATIONSOURCE_H
