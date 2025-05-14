#ifndef HALVETYPE_H
#define HALVETYPE_H
// mycppclass.h
#include <QObject>
#include <vtkType.h>

class Halve : public QObject {
    Q_OBJECT
public:
    enum Representation {
        POINT_CLOUD,
        WIREFRAME,
        SURFACE,
        SURFACE_WITH_EDGE
    };
    Q_ENUM(Representation);


    enum ElectrodeType{
        PAIR,
        BOTH,
        SINGLE,
        ECG,
        PANT
    };
    Q_ENUM(ElectrodeType)

    enum MappingType{
        Lat = Qt::UserRole + 1,
        Voltage,
    };
    Q_ENUM(MappingType)

    enum MeltState {
        INIT,
        TADK,
        STAND,
        REVIEW
    };
    Q_ENUM(MeltState);

    enum UseDuplicatePoints{
        Last = Qt::UserRole + 1,
        Best,
    };
    Q_ENUM(UseDuplicatePoints)

    enum Perception{
        Max = Qt::UserRole + 1,
        Min,
        PlusdVdt,
        MinusdVdt,
        AbsdVdt,
        AbsPeak,
        FirstDeflection,
        LastDeflection,
        MaxVoltage,
        MinVoltage
    };
    Q_ENUM(Perception)

    enum NotebookType{
        GENRE = 0,
        NOTEBOOK,
        SNAPSHOT,
        AUTORECORD,
        SCREENSHOT,
        SCREENRECORD,
        MAPPING,

    };
    Q_ENUM(NotebookType)

    enum ProfileType{
        PROFILE_TEMP,
        PROFILE_FILE,
        PROFILE_SNAPSHOT
    };
    Q_ENUM(ProfileType)

    enum ProfileManagerState {
        PMS_PERFORM,
        PMS_STRAINING,
    };
    Q_ENUM(ProfileManagerState)

    enum NotebookSortType{
        NOTEBOOK_SORT_CRETETIME_ASC,
        NOTEBOOK_SORT_CRETETIME_DESC,
        NOTEBOOK_SORT_CATEGORY,
    };
    Q_ENUM(NotebookSortType)


    enum ChannelMode{
        CHANNELMODE_ELECTRICAL,
        CHANNELMODE_MAGNETIC,
        CHANNELMODE_BLEND
    };
    Q_ENUM(ChannelMode);


    enum TrackStatus {
        TrackStatus_Invalid,
        TrackStatus_Valid,
        TrackStatus_Missing,
        TrackStatus_Disabled,
    };
    Q_ENUM(TrackStatus);


    enum CatheterElectrodeType {
        CET_TUBE = 0,
        CET_MAG = 1,
        CET_PANT = 2,
        CET_ASSIST = 3,
        CET_ORIGIN = 4,
        CET_HANDLE
    };
    Q_ENUM(CatheterElectrodeType);


    enum AlarmName {
        AN_ELECTRIC_REFERENCE_POSITION_IS_TOO_OFFSET = 0x1 << 0,
        AN_THE_CS_CATHETER_CARTRIDGE_IS_NOT_CONFIGURED = 0x1 << 1,
        AN_PRETHORACIC_REFERENCE_CATHETER_CONFIGURATION_ERROR = 0x1 << 2,
        AN_BACK_REFERENCE_CATHETER_CONFIGURATION_ERROR = 0x1 << 3,
        AN_REFERENCE_CATHETER_CONFIGURATION_ERROR = AN_PRETHORACIC_REFERENCE_CATHETER_CONFIGURATION_ERROR | AN_BACK_REFERENCE_CATHETER_CONFIGURATION_ERROR,
        AN_REPRODUCE_CATHETER_CONFIGURATION_ERROR = 0x1 << 4,
        AN_REPRODUCE_CATHETER_NOT_RECEICE_DATA_ERROR = 0x1 << 5,
        AN_REPRODUCE_CATHETER_CONSULT_TARGET_CONFIG_ERROR = 0x1 << 6,
        AN_REPRODUCE_CATHETER_MISSED_ERROR = 0x1 << 7,
        AN_REPRODUCE_CATHETER_NOT_IN_CASKET_ERROR = 0x1 << 8,
        AN_CATHETER_PORT_REUSED_ERROR = 0x1 << 9,
        AN_REPRODUCE_CATHETER_TRACKDATA_ERROR = AN_REPRODUCE_CATHETER_MISSED_ERROR|AN_REPRODUCE_CATHETER_NOT_RECEICE_DATA_ERROR,
        AN_REPRODUCE_CATHETER_ERROR = AN_REPRODUCE_CATHETER_TRACKDATA_ERROR|AN_REPRODUCE_CATHETER_CONFIGURATION_ERROR|AN_REPRODUCE_CATHETER_CONSULT_TARGET_CONFIG_ERROR|AN_REPRODUCE_CATHETER_NOT_IN_CASKET_ERROR,
        AN_STORAGE_FULL_ERROR = 0x1 << 10,
    };
    Q_ENUM(AlarmName);


    enum AlarmType {
        AT_INFO,
        AT_WARNING,
        AT_ERROR
    };
    Q_ENUM(AlarmType);


    enum WoundMode {
        WM_ABRADE,
        WM_PIERCE,
    };
    Q_ENUM(WoundMode);


    enum StorageState {
        SS_STEADY,
        SS_DANGER,
        SS_WARNING
    };
    Q_ENUM(StorageState);


};
constexpr double DefaultSpacing[3] = { 1.0f,1.0f,1.0f };
constexpr qint32 ScalarsWounded = -1;
constexpr qint32 ScalarsReset = 0;
constexpr qint32 ScalarsSet = 1;
constexpr qint32 ScalarsFilling = 2;

constexpr vtkIdType MaskSetPointId = 0x0000000000000000;
constexpr vtkIdType MaskFillingPointId = 0x0100000000000000;
constexpr vtkIdType MaskWoundPointId = 0x0200000000000000;
constexpr vtkIdType MaskPointId = ~(0xFF00000000000000);

constexpr qint32 MagnetismPortAmount = 8;
constexpr qint32 EcgPortAmount = 8;
constexpr qint32 ElectricalPortAmount = 48;
constexpr quint16 MagnetismPant0Port = 0;
constexpr quint16 MagnetismPant1Port = 1;
constexpr quint16 CatheterNoSeta = -1;

extern QString Pant0ID;
extern QString Pant1ID;
extern QString Pant2ID;
extern QString MagnetismElectrodeID;
extern QString PantCatheterID;
extern QString CSCatheterType;
extern QString BudySurfaceCatheterID;
extern double DefaultViewUp[3];
extern double DefaultViewAngle;
extern quint32 DefaultCoefficient;
extern double ElectrodeNodeRadius;
extern double CatheterTubeRadius;
extern double PantCatheterTubeRadius;
extern int DefaultTubeSide;
extern int DefaultTubeResolution;
extern std::string MainBodyMeshColorName;

extern const char *TypesPointDataName;
extern const char *PointsPointDataName;
extern const char *ConnectivityPointDataName;
extern const char *ColorsPointDataName;
extern const char* PointIdScalarName;
extern const char* PerceptionsPointDataName;
extern const char* NodeMeshName;
extern const char* NodeMeshIndexName;
extern const char* ConnectivityRadiusName;
extern const char* FlexibilityName;
extern const char* TubeSidesName;
extern const char* FlexibilityName;
extern const char* ResolutionName;
extern const char* PointDirectionName;
extern const char* LandmarkName;

enum LookupTableName {
    LTN_CATHETER,
    LTN_POINTCLOUD
};

enum MeshName {
    ORIENTATION,
    ELECTRODE_NODE,
    PANT0_ELECTRODE_NODE,
    PANT1_ELECTRODE_NODE,
    MN_DISSOLVE_POINT,
    MN_MAPPING_3D_POINT,
    MN_MAPPING_SURFACE_POINT,
};
#endif // HALVETYPE_H
