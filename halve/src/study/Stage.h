#pragma once

#include <QSharedPointer>
#include <QQuickVTKItem.h>
#include "combined/CatheterTrack.h"
#include "CameraInfo.h"
#include "StageOptions.h"
#include "StageScalar.h"

class vtkCamera;
class ReseauDb;
class vtkRenderWindow;
class vtkActor;
class vtkPolyDataMapper;
class vtkQuaterniond;
class vtkRenderer;
class Symptom;
class vtkPolyData;
class vtkCubeAxesActor;
class vtkGlyph3D;
class vtkImageData;
class vtkScalarBarActor;
class vtkOrientationMarkerWidget;
class Profile;
class vtkVector3d;
class CameraModifiedCallback;
class ImageDataSqueezeFilter;
class Combined;
class Obscurity;
class StageData;
class vtkVector4d;
class Surface;
class SurfaceDb;
class vtkPoints;
class Mapping;
class vtkMatrix4x4;
class DissolveDb;
class MappingPointsDb;
class BlackboardDb;
class CatheterDb;
class Catheter;
class vtkUnsignedCharArray;
class vtkIdList;
class Obscurity;
class vtkProperty;
class vtkEventQtSlotConnect;
class ScalarDb;
class AzimuthDb;
class Azimuth;
class vtkDataSet;
class vtkSelection;
class StageOptions;
class DetectorPair;
class ReproduceOptions;
class Scalar;
class StageScalar;
class DissolveOptions;
class Channel;
class vtkFollower;
class PantPair;
class vtkVectorText;
class vtkUnstructuredGrid;
class Reseau;
struct MeshPair;
class MappingSetting;

class Stage : public QQuickVTKItem {
    Q_OBJECT;
    Q_PROPERTY(Combined *combined WRITE setCombined FINAL);
    Q_PROPERTY(Profile* profile WRITE setProfile READ profile NOTIFY profileChanged);
    Q_PROPERTY(bool gauge READ gauge WRITE setGauge NOTIFY gaugeChanged FINAL)
    Q_PROPERTY(bool peep READ peep WRITE setPeep NOTIFY peepChanged FINAL)
    Q_PROPERTY(Obscurity *obscurity WRITE setObscurity  FINAL)
    Q_PROPERTY(StageScalar *stageScalar WRITE setStageScalar  FINAL)
    Q_PROPERTY(bool isSelected READ isSelected WRITE setIsSelected NOTIFY isSelectedChanged FINAL)
    Q_PROPERTY(bool hightlight READ hightlight NOTIFY hightlightChanged FINAL)
    Q_PROPERTY(CameraInfo cameraInfo READ cameraInfo NOTIFY cameraInfoChanged FINAL)
    Q_PROPERTY(QString azimuthName READ azimuthName WRITE setAzimuthName NOTIFY azimuthNameChanged FINAL)
    Q_PROPERTY(StageOptions *options READ options WRITE setOptions NOTIFY optionsChanged FINAL)
    Q_PROPERTY(Approach approach READ approach WRITE setApproach NOTIFY approachChanged FINAL)
    Q_PROPERTY(Halve::WoundMode woundMode READ woundMode WRITE setWoundMode NOTIFY woundModeChanged FINAL)
    Q_PROPERTY(MappingSetting *mappingSetting READ mappingSetting WRITE setMappingSetting NOTIFY mappingSettingChanged FINAL)

public:
    enum Approach {
        Drag,
        Select,
        Dissolve,
        Wound
    };
    Q_ENUM(Approach);

    enum SelectState {
        Pressed,
        Released,
        Moving
    };
    Q_ENUM(SelectState);

public:
    explicit Stage(QQuickItem* parent = nullptr);
    ~Stage() override = default;
    Q_INVOKABLE void resetRender();

    Q_INVOKABLE void applySelectArea(const QList<QPoint> &points);
    Q_INVOKABLE void cleanSelectArea();
    Q_INVOKABLE void deleteSelectArea();
    Q_INVOKABLE void saveMesh(const QString &name, const QColor& color);


    Q_INVOKABLE void wound(int x, int y, int r, SelectState state);
    Q_INVOKABLE void applyPick(int x, int y);
    Q_INVOKABLE void applyPick(double x, double y, double z);
    Q_INVOKABLE void applyDissolve(int x, int y);

    Q_INVOKABLE void refreshHighlight();
    Q_INVOKABLE void setCameraAzimuth(const QString &name);
    Q_INVOKABLE void resetCameraAzimuth();
    void setCameraPosition(StageData* userData, const double pos[3], const double viewUp[3]);
    void setCameraPosition(StageData* userData, const vtkVector3d &pos, const vtkVector3d &viewUp);
    void setCombined(Combined *combined);
    void setStageScalar(StageScalar *stageScalar);
    void setProfile(Profile* profile);
    Profile *profile() const;

    Approach approach() const;
    void setApproach(const Approach &newApproach);

    bool gauge() const;
    void setGauge(bool newGauge);

    bool peep() const;
    void setPeep(bool newPeep);

    bool dissolve() const;
    void setDissolve(bool newDissolve);

    void setObscurity(Obscurity *newObscurity);

    quint32 meshPointNumber() const;
    void setMeshPointNumber(quint32 newMeshPointNumber);

    bool isSelected() const;
    void setIsSelected(bool newIsSelected);

    bool hightlight() const;
    void setHightlight(quint16 newHightlight);

    CameraInfo cameraInfo() const;

    AzimuthDb *azimuthDb() const;
    void setAzimuthDb(AzimuthDb *newAzimuthDb);

    QString azimuthName() const;
    void setAzimuthName(const QString &newAzimuthName);

    StageOptions *options() const;
    void setOptions(StageOptions *newStageOptions);

    Halve::WoundMode woundMode() const;
    void setWoundMode(const Halve::WoundMode &newWoundMode);

    MappingSetting *mappingSetting() const;
    void setMappingSetting(MappingSetting *newMeltSetting);
signals:
    void profileChanged();
    void initRender();
    void meshPointChanged(int num);
    void updated(qint64 expendTime, int numberOfPoints);
    void aspectChanged();
    void selectedChanged();
    void saveImageSaveFinished();
    void gaugeChanged();
    void peepChanged();
    void dissolveChanged();
    void meshPointNumberChanged();
    void isSelectedChanged();
    void hightlightChanged();
    void viewAngleChanged();
    void approachChanged();
    void patnEnableChanged();
    void cameraInfoChanged();
    void azimuthDbChanged();
    void azimuthNameChanged();
    void optionsChanged();
    void clicked(qint32 x, qint32 y);
    void pointPicked(double x, double y, double z);

    void woundModeChanged();
    void mappingSettingChanged();

public slots:
    void onCatheterDeleted(Catheter *catheter);
    void onCatheterApparentChangedChanged(Catheter *catheter);
    void onCatheterEmployChanged(Catheter *catheter);
    void onCurrentPointIdChanged();
    void onAzimuthChanged(const Azimuth *azimuth);
    void onAzimuthDeleted(const Azimuth *azimuth);
    void onLeftButtonPress(int x, int y);
    void onCatheterMeshChanged(Catheter *catheter);
    void onCatheterImported();
    void onReseauChanged(Reseau* reseau);
    void onReseauDeleted(Reseau* reseau);
    void onReseauAdded(Reseau* reseau);
    void onCurrentReseauChanged();
    void onReseauPropertyChanged(Reseau* reseau);
    void onMappingGapChanged();
    void onMappingTypeChanged();
    void onDissolveOptionsRadiusChanged();


private:
    vtkUserData initializeVTK(vtkRenderWindow* renderWindow) override;
    void destroyingVTK(vtkRenderWindow* renderWindow, vtkUserData userData) override;

    void refreshDetectorPosition(StageData* data, Catheter* catheter, const QList<CatheterTrack>& trackDatas);
    void resetOptions();
    void resetOptions(StageData* userData);

    void resetMesh();
    void setMeshActorProperty(Reseau* reseau, vtkActor *actor);

    void initCubeAxesActor(StageData* userData);
    void setAxesProperty(StageData* userData);
    void showAxesActor(StageData* userData);

    void updateDetector(StageData* userData,
                        vtkPoints* points,
                        vtkSmartPointer<vtkUnsignedCharArray> scalars, Catheter *catheter);
    void updateShamDetector(StageData* userData, Catheter *catheter, const QList<CatheterTrack> &trackDatas);
    void updateSincerityDetector(StageData* userData, Catheter *catheter, const QList<CatheterTrack> &trackDatas);

    void updatePointCloud(StageData* userData, vtkSmartPointer<vtkPolyData> polyData);
    void setPlaceholderCube(StageData* userData);
    vtkSmartPointer<vtkActor> createPantActor();

    void applyDissolve(StageData* userData,int x, int y);
    void getPickPosition(StageData* userData,int x, int y, double pos[3]);

    void applyGauge(StageData *userData);
    void setCameraAzimuth(StageData* userData, const Azimuth *azimuth);
    void setMeshActorProperty(Reseau* reseau, vtkProperty* property);
    void saveCameraInfo();
    void setCameraInfo(StageData* userData);
    void initRenderCamera(StageData* userData);
    void onInteractionEvent(StageData* userData);
    void createDetector();
    vtkSmartPointer<DetectorPair> createDetector(Catheter *catheter);
    void createDetectorTube(Catheter *catheter,vtkSmartPointer<DetectorPair> &detpair, bool visibility);
    void createDetectorShadow(Catheter *catheter,vtkSmartPointer<DetectorPair> &detpair, bool visibility);
    void setCatheterLabelRenderer(vtkRenderWindow* renderWindow, StageData* userData);
    void setDetectorTubeMesh(StageData* userData, Catheter *catheter,vtkSmartPointer<vtkPoints> points,vtkSmartPointer<vtkUnstructuredGrid> mesh);
    void setDetectorVisibility(StageData* userData, Catheter *catheter);
    std::pair<bool, bool> getDetectorVisibility(Catheter *catheter);
    vtkSmartPointer<MeshPair> createMeshPair(StageData* userData, Reseau* reseau,vtkSmartPointer<vtkPolyData> polyData);

    void createPantDetector();
    vtkSmartPointer<PantPair> createPantDetector(Catheter *catheter, bool visibility);
    void setDetectorConnectSlot(Catheter *catheter, bool b);
    void addPantDetector(StageData* userData,const vtkSmartPointer<PantPair> &detpair);
    void removePantDetector(StageData* userData,const vtkSmartPointer<PantPair> &detpair);
    void setPantDetectorVisibility(const vtkSmartPointer<PantPair> &detpair, bool catheterVisibility);

    void removeDetector(StageData* userData, Catheter *catheter);
    void removeDetector(StageData* userData,const vtkSmartPointer<DetectorPair> &detpair);
    void addDetector(StageData* userData,const vtkSmartPointer<DetectorPair> &detpair);
    void setDetectorVisibility(const vtkSmartPointer<DetectorPair> &detpair, bool catheterVisibility, bool catheterLabelVisibility);
    void createDetectorTextFollower(Catheter* catheter, vtkSmartPointer<DetectorPair>& detectorPair, bool visibility);

    vtkSmartPointer<vtkMatrix4x4> getRenderMatrixAndViewport(StageData* userData, vtkVector4d &viewport);
    void showSelectActor(vtkSmartPointer<vtkPolyData> polyData);
    void hideSelectActor(StageData* userData);


    void setLight(StageData* userData);
    void setPicker(StageData* userData);
    void setCameraModifiedCallback(StageData* userData);
    void setMouseInteractorStyle(StageData* userData);
    void setDetectorShadow(vtkRenderWindow* renderWindow, StageData* userData);
    void setMeshShadow(vtkRenderWindow* renderWindow, StageData* userData);

    void setInteractor(vtkRenderWindow* renderWindow, StageData* userData);
    void resetHightlightActor(StageData* userData);

    void showMappingPointActor(StageData* userData);
    void showMappingLocationActor(StageData* userData);

    vtkSmartPointer<vtkFollower> createTextFollower(const char *label);
    vtkSmartPointer<vtkFollower> createTextFollower(vtkVectorText *label);
    vtkSmartPointer<vtkIdList> extractSelectionGrid(StageData* userData, const QPolygon& polygon);

    void resetPoint();
    void resetPoint(StageData* userData);
    void resetDissolvePointActor(StageData* userData);
    void resetMappintPointActor(StageData* userData);
    void resetMappint3dPointActor(StageData* userData);
    void resetMappintSurfacePointActor(StageData* userData);

    void applyPickPolyDataPointId(vtkPolyData* polyData, double x, double y, double z, const char *mem);

private:
    Qt::HANDLE m_vtkRenderThreadId;
    QPointer<Profile> m_profile;
    QPointer<MappingPointsDb> m_mappingPointsDb;
    QPointer<DissolveDb> m_dissolveDb;
    QPointer<DissolveOptions> m_dissolveOptions;
    QPointer<CatheterDb> m_catheterDb;
    QPointer<Obscurity> m_obscurity;
    QPointer<ReseauDb> m_reseauDb;
    QPointer<Combined> m_combined;
    QPointer<StageOptions> m_stageOptions;
    QPointer<AzimuthDb> m_azimuthDb;
    QPointer<StageScalar> m_stageScalar;
    QPointer<ReproduceOptions> m_reproduceOptions;
    MappingSetting *m_mappingSetting = nullptr;

    QString m_azimuthName;
    int m_previousSwapBuffers = 0;
    QPointer<BlackboardDb> m_blackboardDb;
    bool m_isSelected = false;
    bool m_gauge = false;
    bool m_peep = false;
    Approach m_approach = Drag;
    Halve::WoundMode m_woundMode = Halve::WoundMode::WM_ABRADE;

    vtkSmartPointer<vtkCamera> m_camera;
    vtkSmartPointer<vtkIdList> m_selectedIds;

    qint32 m_highlightTimerTick = 0;
    qint32 m_hightlight = 0;
    friend class CameraModifiedCallback;
};
