
#include <vtkObjectFactory.h>
#include "album/AlbumExporter.h"
#include "catheter/CatheterElectrodeNodePointListModel.h"
#include "catheter/CatheterStage.h"
#include "catheter/CatheterTemplet.h"
#include "catheter/CatheterTempletFilterModel.h"
#include "catheter/CatheterTempletListModel.h"
#include "catheter/CatheterType.h"
#include "catheter/CatheterTypeModel.h"
#include "combined/TrackDataListModel.h"
#include "dissolve/DissolveDataSource.h"
#include "dissolve/DissolveElectrodeNodeListModel.h"
#include "dissolve/DissolveSortFilterProxyModel.h"
#include "halitus/BreathChartListModel.h"
#include "halitus/BreathSurvey.h"

#include "HalveType.h"
#include <profile/Profile.h>
#include <profile/ProfileData.h>
#include <profile/ProfileListModel.h>
#include "mapping/MappingPointCopyer.h"
#include "mapping/MappingPointGroupConsultFilterListModel.h"
#include "mapping/MappingPointRestrik.h"
#include "mapping/MappingReseauChecker.h"
#include "melt/MeltConsultElectrodeListModel.h"
#include "mesh/MeshImporter.h"
#include "mesh/Obscurity.h"
#include "combined/Combined.h"
#include "album/AlbumModel.h"
#include "casket/Casket.h"
#include "casket/CasketFilterProxyModel.h"
#include "casket/CasketListModel.h"
#include "catheter/BodySurfaceElectrodeNodeListModel.h"
#include "catheter/CatheterCasketFilterProxyModel.h"
#include "catheter/CatheterElectrodeNodeListModel.h"
#include "catheter/CatheterListModel.h"
#include "catheter/CatheterRepertory.h"
#include "catheter/CatheterRepertoryTableModel.h"
#include "catheter/ElectrodeNodeListModel.h"
#include "catheter/ElectrodeNodeSelectListModel.h"
#include "dissolve/DissolveListModel.h"
#include "mapping/MappingPointSortFilterProxyModel.h"
#include "melt/AppointElectrode.h"
#include "melt/MeltChannelChartListModel.h"
#include "melt/MeltOptionsDb.h"
#include "melt/MeltDataSource.h"
#include "melt/MeltSampleLine.h"
#include "melt/MeltSetting.h"
#include "notebook/NotebookListModel.h"
#include "notebook/NotebookSortFilterModel.h"
#include "notebook/NotebookTreeModel.h"
#include "oscillograp/OscillograpCatheterElectrodeNodeListModel.h"
#include "profile/StorageChecker.h"
#include "study/CatheterTrackWidget.h"
#include "study/Hotkey.h"
#include "study/alarm/AlarmListModel.h"
#include "user/UserExporter.h"
#include "reseau/ReseauListModel.h"
#include "oscillograp/OscillograpChartListModel.h"
#include "oscillograp/OscillograpElectrode.h"
#include "oscillograp/PantElectrodeListNode.h"
#include "study/CameraInfo.h"
#include "study/Orientation.h"
#include "study/StageData.h"
#include "study/StageOptions.h"
#include "study/StageScalar.h"
#include "study/StageSettings.h"
#include "study/Study.h"
#include "study/azimuth/AzimuthModel.h"
#include "study/azimuth/AzimuthProxyModel.h"
#include "study/scalar/ScalarColor.h"
#include "terminal/Alleyway.h"
#include "terminal/FpgaTest.h"
#include "user/UserListModel.h"
#include "user/User.h"
#include "user/UserListSortFilterProxyModel.h"

#include "study/Stage.h"
#include "oscillograp/OscillograpDataSource.h"
#include "profile/LayoutSaver.h"
#include "widget/SerialPortListModel.h"
#include "widget/TreeModel.h"
#include "widget/TreeElement.h"

#include "catheter/CatheterTableModel.h"
#include "catheter/Catheter.h"
#include "study/CatheterShow.h"


#include <profile/FilterOptionItem.h>
#include <profile/FilterOptions.h>
#include <profile/EcgOptions.h>
#include <profile/ProfileSnapshot.h>
#include <estate/Pulse.h>

#include <mapping/MappingPointGroup.h>
#include <mapping/MappingPointGroupListModel.h>
#include <mapping/MappingSetting.h>

#include <melt/Melt.h>
#include <melt/MeltElectrodeNodeFilterModel.h>
#include <melt/MeltOptions.h>
#include <melt/MeltTableModel.h>

#include <catheter/CatheterElectrodeNodePointDb.h>
#include <catheter/CatheterMagnetism.h>
#include <catheter/CatheterSortFilterProxyModel.h>
#include <catheter/CatheterTypeDb.h>
#include <catheter/ElectrodeNode.h>

#include <utility/ModelCache.h>

#include <album/Picture.h>

#include <mesh/BlackboardDb.h>
#include <mesh/ReproduceOptions.h>
#include <reseau/Reseau.h>
#include <reseau/ReseauDb.h>
#include <reseau/ReseauSortFilterProxyModel.h>
#include <mapping/MappingPointsListModel.h>
#include <QAbstractAxis>
#include <QAbstractSeries>
#include <vtkMatrix4x4.h>
#include <dissolve/DissolveDb.h>
#include <channel/Channel.h>
#include <channel/ChannelDataFilter.h>
#include <casket/CasketCatheterListModel.h>
#include <halitus/BreathOptions.h>
#include <study/scalar/Scalar.h>
#include <study/scalar/ScalarModel.h>
#include <study/azimuth/Azimuth.h>
#include <oscillograp/OscillograpOptions.h>
#include <oscillograp/SignalSourceOptions.h>
#include <notebook/NotebookDataSource.h>
#include <notebook/NotebookOptions.h>
#include <notebook/NotebookSnapshot.h>

#include "registration/RegistrationSourceStage.h"
#include "registration/RegistrationTargetStage.h"
#include "registration/CtDataStorage.h"
#include "registration/RegistrationProcessor.h"
#include "registration/ProgressReporter.h"


Q_DECLARE_METATYPE(QAbstractSeries *);
Q_DECLARE_METATYPE(QAbstractAxis *);
Q_DECLARE_METATYPE(vtkMatrix4x4*);

vtkStandardNewMacro(StageData);
vtkStandardNewMacro(DetectorPair);
vtkStandardNewMacro(PantPair);
vtkStandardNewMacro(MeshPair);

QString Pant0ID = "pant.id.0";
QString Pant1ID = "pant.id.1";
QString Pant2ID = "pant.id.2";
QString PantCatheterID = "pant.pant";
QString MagnetismElectrodeID = "magnetism_electrode_id";
QString BudySurfaceCatheterID = "bodysurface.bodysurface";
QString CSCatheterType = "CS";

double DefaultViewUp[3]{0,1,0};
double DefaultViewAngle = 14.0;

double ElectrodeNodeRadius = 0.5;
double CatheterTubeRadius = 0.2;
double PantCatheterTubeRadius = 0.5;
quint32 DefaultCoefficient = 100;
int DefaultTubeSide = 10;
int DefaultTubeResolution = 70;


std::string MainBodyMeshColorName = "LightGrey";

void qmlRegisterTypes() {
    qRegisterMetaType<QAbstractSeries*>();
    qRegisterMetaType<QAbstractAxis*>();

    qmlRegisterUncreatableType<Halve>("Halve",1,0,"Halve", "");
    qmlRegisterType<TreeModel>("Halve",1,0,"TreeModel");
    qmlRegisterType<TreeElement>("Halve",1,0,"TreeElement");
    qmlRegisterType<Stage>("Halve", 1, 0, "Stage");
    qmlRegisterType<UserListModel>("Halve", 1, 0, "UserListModel");
    qmlRegisterType<UserListSortFilterProxyModel>("Halve", 1, 0, "UserListSortFilterProxyModel");
    qmlRegisterType<OscillograpDataSource>("Halve", 1, 0, "OscillograpDataSource");
    qmlRegisterType<User>("Halve", 1, 0, "User");
    qmlRegisterType<Combined>("Halve",1,0,"Combined");
    qmlRegisterType<Obscurity>("Halve",1,0,"Obscurity");
    qmlRegisterType<CatheterTableModel>("Halve",1,0,"CatheterTableModel");
    qmlRegisterType<Catheter>("Halve",1,0,"Catheter");
    qmlRegisterType<CatheterShow>("Halve",1,0,"CatheterShow");
    qmlRegisterType<Study>("Halve", 1, 0, "Study");
    qmlRegisterUncreatableType<Profile>("Halve",1,0,"Profile", "");
    qmlRegisterUncreatableType<ProfileSnapshot>("Halve",1,0,"ProfileSnapshot", "");
    qmlRegisterType<ProfileListModel>("Halve",1,0,"ProfileListModel");
    qmlRegisterUncreatableType<ProfileData>("Halve",1,0,"ProfileData", "");
    qmlRegisterUncreatableType<FilterOptions>("Halve",1,0,"FilterOptions", "");
    qmlRegisterUncreatableType<FilterOptionItem>("Halve",1,0,"FilterOptionItem", "");
    qmlRegisterUncreatableType<SignalSourceOptions>("Halve",1,0,"SignalSourceOptions", "");
    qmlRegisterUncreatableType<EcgOptions>("Halve",1,0,"EcgOptions", "");
    qmlRegisterUncreatableType<Mapping>("Halve",1,0,"Mapping", "");
    qmlRegisterUncreatableType<ReproduceOptions>("Halve",1,0,"ReproduceOptions", "");
    qmlRegisterType<MeltTableModel>("Halve",1,0,"MeltTableModel");
    qmlRegisterUncreatableType<Melt>("Halve",1,0,"Melt", "");
    qmlRegisterType<CatheterSortFilterProxyModel>("Halve",1,0,"CatheterSortFilterProxyModel");
    qmlRegisterUncreatableType<ModelCache>("Halve",1,0,"ModelCache", "");
    qmlRegisterType<AlbumModel>("Halve",1,0,"AlbumModel");
    qmlRegisterUncreatableType<Picture>("Halve",1,0,"Picture", "");
    qmlRegisterUncreatableType<BlackboardDb>("Halve",1,0,"BlackboardDb", "");
    qmlRegisterType<SerialPortListModel>("Halve",1,0,"SerialPortListModel");
    qmlRegisterType<Pulse>("Halve",1,0,"Pulse");
    qmlRegisterType<MappingPointsListModel>("Halve",1,0,"MappingPointsListModel");
    qmlRegisterUncreatableType<ElectrodeNode>("Halve",1,0,"ElectrodeNode", "");
    qmlRegisterType<ElectrodeNodeListModel>("Halve",1,0,"ElectrodeNodeListModel");
    qmlRegisterType<OscillograpChartListModel>("Halve",1,0,"OscillograpChartListModel");
    qmlRegisterType<MeltChannelChartListModel>("Halve",1,0,"MeltChannelChartListModel");
    qmlRegisterType<MeltDataSource>("Halve",1,0,"MeltDataSource");
    qmlRegisterType<CatheterListModel>("Halve",1,0,"CatheterListModel");
    qmlRegisterUncreatableType<MeltSamplingElectrode>("Halve",1,0,"MeltSamplingElectrode", "");
    qmlRegisterType<DissolveListModel>("Halve",1,0,"DissolveListModel");
    qmlRegisterUncreatableType<DissolveDb>("Halve",1,0,"DissolveDb", "");
    qmlRegisterType<ChannelDataFilter>("Halve",1,0,"ChannelDataFilter");
    qmlRegisterUncreatableType<CatheterRepertory>("Halve",1,0,"CatheterRepertory", "");
    qmlRegisterType<CatheterRepertoryTableModel>("Halve",1,0,"CatheterRepertoryTableModel");
    qmlRegisterType<BodySurfaceElectrodeNodeListModel>("Halve",1,0,"BodySurfaceElectrodeNodeListModel");
    qmlRegisterType<CatheterElectrodeNodeListModel>("Halve",1,0,"CatheterElectrodeNodeListModel");
    qmlRegisterType<CatheterCasketFilterProxyModel>("Halve",1,0,"CatheterCasketFilterProxyModel");
    qmlRegisterType<CasketListModel>("Halve",1,0,"CasketListModel");
    qmlRegisterUncreatableType<Casket>("Halve",1,0,"Casket", "");
    qmlRegisterType<CasketFilterProxyModel>("Halve",1,0,"CasketFilterProxyModel");
    qmlRegisterType<CasketCatheterListModel>("Halve",1,0,"CasketCatheterListModel");
    qmlRegisterType<ElectrodeNodeSelectListModel>("Halve",1,0,"ElectrodeNodeSelectListModel");
    qmlRegisterType<MeltSampleLine>("Halve",1,0,"MeltSampleLine");
    qmlRegisterType<OscillograpElectrode>("Halve",1,0,"OscillograpElectrode");
    qmlRegisterType<AppointElectrode>("Halve",1,0,"AppointElectrode");
    qmlRegisterType<MeltElectrodeNodeFilterModel>("Halve",1,0,"MeltElectrodeNodeFilterModel");
    qmlRegisterType<MappingPointGroupListModel>("Halve",1,0,"MappingPointGroupListModel");
    qmlRegisterType<ReseauSortFilterProxyModel>("Halve",1,0,"ReseauSortFilterProxyModel");
    qmlRegisterUncreatableType<Reseau>("Halve",1,0,"Reseau", "");
    qmlRegisterUncreatableType<ReseauDb>("Halve",1,0,"ReseauDb", "");
    qmlRegisterType<LayoutSaver>("Halve",1,0,"LayoutSaver");
    qmlRegisterType<PantElectrodeListNode>("Halve",1,0,"PantElectrodeListNode");
    qmlRegisterType<Alleyway>("Halve",1,0,"Alleyway");
    qmlRegisterType<FpgaTest>("Halve",1,0,"FpgaTest");
    qmlRegisterType<TrackDataListModel>("Halve",1,0,"TrackDataListModel");
    qmlRegisterType<ReseauListModel>("Halve",1,0,"ReseauListModel");
    qmlRegisterType<DissolveSortFilterProxyModel>("Halve",1,0,"DissolveSortFilterProxyModel");
    qmlRegisterType<MappingPointSortFilterProxyModel>("Halve",1,0,"MappingPointSortFilterProxyModel");
    qmlRegisterType<BreathSurvey>("Halve",1,0,"BreathSurvey");
    qmlRegisterType<BreathOptions>("Halve",1,0,"BreathOptions");
    qmlRegisterType<ScalarModel>("Halve",1,0,"ScalarModel");
    qmlRegisterUncreatableType<Scalar>("Halve",1,0,"Scalar", "");
    qmlRegisterUncreatableType<ScalarColor>("Halve",1,0,"ScalarColor", "");
    qmlRegisterUncreatableType<MeltOptions>("Halve",1,0,"MeltOptions", "");
    qmlRegisterUncreatableType<MeltOptionsDb>("Halve", 1, 0, "MeltConfigDb", "");
    qmlRegisterType<AzimuthModel>("Halve",1,0,"AzimuthModel");
    qmlRegisterType<AzimuthProxyModel>("Halve",1,0,"AzimuthProxyModel");
    qmlRegisterType<CatheterTypeModel>("Halve",1,0,"CatheterTypeModel");
    qmlRegisterUncreatableType<CatheterType>("Halve",1,0,"CatheterType", "");
    qmlRegisterUncreatableType<CatheterTypeDb>("Halve",1,0,"CatheterTypeDb", "");
    qmlRegisterType<OscillograpCatheterElectrodeNodeListModel>("Halve",1,0,"OscillograpCatheterElectrodeNodeListModel");
    qmlRegisterType<Orientation>("Halve", 1, 0, "Orientation");
    qmlRegisterUncreatableType<CameraInfo>("Halve",1,0,"CameraInfo", "");
    qmlRegisterUncreatableType<Azimuth>("Halve",1,0,"Azimuth", "");
    qmlRegisterUncreatableType<MeltSetting>("Halve",1,0,"MeltSetting", "");
    qmlRegisterUncreatableType<StageSettings>("Halve",1,0,"StageSettings", "");
    qmlRegisterType<CatheterTemplet>("Halve",1,0,"CatheterTemplet");
    qmlRegisterType<CatheterTempletListModel>("Halve",1,0,"CatheterTempletListModel");
    qmlRegisterType<CatheterTempletFilterModel>("Halve",1,0,"CatheterTempletFilterModel");
    qmlRegisterType<MappingPointGroupConsultFilterListModel>("Halve",1,0,"MappingPointGroupConsultFilterListModel");
    qmlRegisterType<MeltConsultElectrodeListModel>("Halve",1,0,"MeltConsultElectrodeListModel");
    qmlRegisterType<Channel>("Halve",1,0,"Channel");
    qmlRegisterUncreatableType<ChannelReplica>("Halve",1,0,"ChannelReplica", "");
    qmlRegisterType<UserExporter>("Halve",1,0,"UserExporter");
    qmlRegisterType<NotebookTreeModel>("Halve",1,0,"NotebookTreeModel");
    qmlRegisterType<StageScalar>("Halve",1,0,"StageScalar");
    qmlRegisterType<NotebookDataSource>("Halve",1,0,"NotebookDataSource");
    qmlRegisterUncreatableType<NotebookOptions>("Halve",1,0,"NotebookOptions", "");
    qmlRegisterType<NotebookListModel>("Halve",1,0,"NotebookListModel");
    qmlRegisterType<NotebookSortFilterModel>("Halve",1,0,"NotebookSortFilterModel");
    qmlRegisterType<DissolveDataSource>("Halve",1,0,"DissolveDataSource");
    qmlRegisterType<AlbumExporter>("Halve",1,0,"AlbumExporter");
    qmlRegisterUncreatableType<OscillograpOptions>("Halve",1,0,"OscillograpOptions", "");
    qmlRegisterUncreatableType<NotebookSnapshot>("Halve",1,0,"NotebookSnapshot", "");
    qmlRegisterUncreatableType<MappingPointGroup>("Halve",1,0,"MappingPointGroup", "");
    qmlRegisterType<DissolveElectrodeNodeListModel>("Halve",1,0,"DissolveElectrodeNodeListModel");
    qmlRegisterType<BreathChartListModel>("Halve",1,0,"BreathChartListModel");
    qmlRegisterType<MeshImporter>("Halve",1,0,"MeshImporter");
    qmlRegisterType<MappingPointRestrik>("Halve",1,0,"MappingPointRestrik");
    qmlRegisterType<MappingPointCopyer>("Halve",1,0,"MappingPointCopyer");
    qmlRegisterUncreatableType<CatheterMagnetism>("Halve",1,0,"CatheterMagnetism", "");
    qmlRegisterType<CatheterStage>("Halve",1,0,"CatheterStage");
    qmlRegisterType<CatheterElectrodeNodePointListModel>("Halve",1,0,"CatheterElectrodeNodePointListModel");
    qmlRegisterType<CatheterElectrodeNodePointDb>("Halve",1,0,"CatheterElectrodeNodePointDb");
    qmlRegisterType<AlarmListModel>("Halve",1,0,"AlarmListModel");
    qmlRegisterType<CatheterTrackWidget>("Halve",1,0,"CatheterTrackWidget");
    qmlRegisterType<MappingReseauChecker>("Halve",1,0,"MappingReseauChecker");
    qmlRegisterType<StorageChecker>("Halve",1,0,"StorageChecker");
    qmlRegisterType<Hotkey>("Halve",1,0,"Hotkey");
    qmlRegisterType<MappingSetting>("Halve",1,0,"MappingSetting");

    qmlRegisterType<RegistrationSourceStage>("Halve", 1, 0, "RegistrationSourceStage");
    qmlRegisterType<RegistrationTargetStage>("Halve", 1, 0, "RegistrationTargetStage");
    qmlRegisterType<CtDataStorage>("Halve", 1, 0, "CtDataStorage");
    qmlRegisterType<RegistrationProcessor>("Halve", 1, 0, "RegistrationProcessor");
    qmlRegisterType<ProgressReporter>("Halve", 1, 0, "ProgressReporter");
}

