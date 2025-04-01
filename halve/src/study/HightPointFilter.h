#ifndef HIGHTPOINTFILTER_H
#define HIGHTPOINTFILTER_H
#include <qtypes.h>
#include <vtkPolyDataAlgorithm.h>

class DissolveDb;
class MappingPointsDb;

class HightPointFilter : public vtkPolyDataAlgorithm
{

public:
    vtkTypeMacro(HightPointFilter, vtkPolyDataAlgorithm);
    static HightPointFilter* New();

    void SetDissolveDb(DissolveDb *dissolve) {
        m_dissolveDb = dissolve;
    }
    void SetMappingPointsDb(MappingPointsDb *mappingPointsDb){
        m_mappingPointsDb = mappingPointsDb;
    }

    vtkSetMacro(ShowDissolve, bool);
    vtkGetMacro(ShowDissolve, bool);

    vtkSetMacro(CurrentDissolvePointId, qint64);
    vtkGetMacro(CurrentDissolvePointId, qint64);

    vtkSetMacro(ShowSurface, bool);
    vtkGetMacro(ShowSurface, bool);

    vtkSetMacro(CurrentMappingPointId, qint64);
    vtkGetMacro(CurrentMappingPointId, qint64);


    vtkSetMacro(DissolvePointRadius, double);
    vtkGetMacro(DissolvePointRadius, double);

    vtkSetMacro(MappingPointRadius, double);
    vtkGetMacro(MappingPointRadius, double);

    vtkSetMacro(Margen, double);
    vtkGetMacro(Margen, double);

protected:
    HightPointFilter();
    ~HightPointFilter();

    int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
    int FillInputPortInformation(int port, vtkInformation* info) override;


private:
    HightPointFilter(const HightPointFilter&) = delete;
    void operator=(const HightPointFilter&) = delete;

private:
    bool ShowDissolve = false;
    bool ShowSurface = false;
    double Margen = 3.0;
    double DissolvePointRadius = 1.0;
    double MappingPointRadius = 1.0;
    qint64  CurrentDissolvePointId = -1;
    qint64  CurrentMappingPointId = -1;
    DissolveDb *m_dissolveDb = nullptr;
    MappingPointsDb *m_mappingPointsDb = nullptr;
};

#endif // HIGHTPOINTFILTER_H
