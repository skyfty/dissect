#ifndef STAGEDATA_H
#define STAGEDATA_H

#include <QHash>
#include <QMap>
#include <vtkObject.h>
#include <vtkSmartPointer.h>

class vtkRenderer;
class vtkLookupTable;
class vtkCubeAxesActor;
class vtkActor;
class vtkScalarBarActor;
class vtkOrientationMarkerWidget;
class vtkOpenGLHardwareSelector;
class vtkDistanceWidget;
class vtkIPWCallback;
class vtkImplicitPlaneWidget2;
class MouseInteractorStyle;
class vtkRenderStepsPass;
class vtkOutlineGlowPass;
class OrientationWidget;
class vtkInteractorStyleDrawPolygon;
class QVTKInteractor;
class vtkRenderWindowInteractor;
class vtkActorCollection;
class HardwareSelector;
class vtkContourWidget;
class vtkOrientedGlyphContourRepresentation;
class vtkDataSetMapper;
class vtkCellPicker;
class DistanceWidget;
class vtkTextActor;
class vtkFollower;
class Catheter;
class vtkVectorText;
class vtkBiDimensionalWidget;
class CatheterShowFilter;
class vtkDistanceWidget;
class ImageDataSqueezeFilter;
class vtkCoordinate;
class vtkAreaPicker;
class vtkOpenGLGlyph3DMapper;
class vtkPolyDataMapper;
class Reseau;
class vtkGaussianKernel;
class vtkPointInterpolator;
class vtkColorTransferFunction;
class vtkProbabilisticVoronoiKernel;
class MeshColorTransfer;
class vtkCleanPolyData;
class HightPointFilter;
class MappingLocationSource;
class MappingPointVisbleFilter;
class DissolvePointSource;
class EliminateSource;
class MappingPointSource;
class ReseauShowFilter;
class ElectricalPantFilter;

struct DetectorPair : public vtkObject {
    static DetectorPair *New();
    vtkTypeMacro(DetectorPair, vtkObject);
    vtkSmartPointer<vtkActor> detectorTubeActor;
    vtkSmartPointer<vtkActor> detectorShadowActor;
    vtkSmartPointer<vtkActor> detectorNodeActor;
    vtkSmartPointer<CatheterShowFilter> detectorShowFilter;
    QList<vtkSmartPointer<vtkFollower>> textFollower;
};

struct PantPair : public vtkObject {
    static PantPair *New();
    vtkTypeMacro(PantPair, vtkObject);
    vtkSmartPointer<vtkActor> pantActor;
    vtkSmartPointer<CatheterShowFilter> showFilter;
    vtkSmartPointer<vtkVectorText> text;
    vtkSmartPointer<vtkFollower> textFollower;
};

struct MeshPair : public vtkObject {
    static MeshPair *New();
    vtkTypeMacro(MeshPair, vtkObject);
    vtkSmartPointer<vtkActor> actor;
    vtkSmartPointer<EliminateSource> source;
    vtkSmartPointer<vtkPointInterpolator> interpolator;
    vtkSmartPointer<MeshColorTransfer> colorTransfer;
    vtkSmartPointer<ReseauShowFilter> reseauShowFilter;
};

struct StageData : vtkObject
{
public:
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkRenderer> rendererMeshShadow;
    vtkSmartPointer<vtkRenderer> rendererSurface;
    vtkSmartPointer<vtkRenderer> rendererDetectorShadow;
    vtkSmartPointer<vtkRenderer> rendererCatheterLabel;
    vtkSmartPointer<vtkRenderWindowInteractor> interactor;
    vtkSmartPointer<vtkRenderStepsPass> basicPasses;
    vtkSmartPointer<vtkCubeAxesActor> axesActor;
    QHash<Catheter*, vtkSmartPointer<DetectorPair>> detectors;
    vtkSmartPointer<PantPair> pantActor;

    vtkSmartPointer<vtkActor> blackboradActor;
    QHash<Reseau*, vtkSmartPointer<MeshPair>> meshs;
    vtkSmartPointer<vtkActor> currentMeshActor;

    vtkSmartPointer<vtkActor> selectedActor;
    vtkSmartPointer<HardwareSelector> hardwareSelector;
    vtkSmartPointer<DistanceWidget> distanceWidget;
    vtkSmartPointer<vtkIPWCallback> ipw;
    vtkSmartPointer<vtkImplicitPlaneWidget2> planeWidget;
    vtkSmartPointer<ImageDataSqueezeFilter> squeezeFilter;
    vtkSmartPointer<MouseInteractorStyle> mouseInteractorStyle;
    vtkSmartPointer<vtkCellPicker> picker;

    vtkSmartPointer<MappingLocationSource> mappinglocationSource;
    vtkSmartPointer<MappingPointVisbleFilter> mappingPointVisbleFilter;
    vtkSmartPointer<DissolvePointSource> dissolvePointSource;

    vtkSmartPointer<vtkActor> mapping3dPointActor;
    vtkSmartPointer<vtkActor> mappingSurfacePointActor;
    vtkSmartPointer<vtkActor> dissolvePointActor;
    vtkSmartPointer<vtkActor> highlightPointActor;
    vtkSmartPointer<HightPointFilter> highlightPointFilter;
    vtkSmartPointer<vtkProbabilisticVoronoiKernel> voronoiKernel;

public:
    static StageData* New();
    ~StageData() override {}
    vtkTypeMacro(StageData, vtkObject);
};
#endif // STAGEDATA_H
