#ifndef VALIDPOINTFILTER_H
#define VALIDPOINTFILTER_H
#include <vtkPolyDataAlgorithm.h>

class DissolveDb;

class DissolvePointSource : public vtkPolyDataAlgorithm {
public:
    vtkTypeMacro(DissolvePointSource, vtkPolyDataAlgorithm);

    static DissolvePointSource* New();
    void SetDissolveDb(DissolveDb *dissolveDb);

protected:
    DissolvePointSource();
    ~DissolvePointSource() override = default;
    int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
    vtkMTimeType GetMTime() override;
    int FillInputPortInformation(int port, vtkInformation* info) override;

private:
    DissolvePointSource(const DissolvePointSource&) = delete;
    void operator=(const DissolvePointSource&) = delete;

private:
    DissolveDb *m_dissolveDb = nullptr;;
};

#endif // VALIDPOINTFILTER_H
