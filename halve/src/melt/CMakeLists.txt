# CMakeList.txt : CMake project for halve, include source and define
# project specific logic here.
#

set(SRCS
    "Melt.cpp"
    "Melt.h"
    "MeltTableModel.h"
    "MeltTableModel.cpp"
    "MeltDb.h"
    "MeltDb.cpp"
    "MeltChannelChartListModel.h"
    "MeltChannelChartListModel.cpp"
    "MeltSamplingElectrode.h"
    "MeltSamplingElectrode.cpp"
    "MeltDataSource.h"
    "MeltDataSource.cpp"
    "MeltSurvey.h"
    "MeltSurvey.cpp"
    "MeltSampleLine.h"
    "MeltSampleLine.cpp"
    "AppointElectrode.h"
    "AppointElectrode.cpp"
    "MeltElectrodeNodeFilterModel.h"
    "MeltElectrodeNodeFilterModel.cpp"
    "MeltWorker.h"
    "MeltWorker.cpp"
    "MeltOptions.h"
    "MeltOptions.cpp"
    "MeltOptionsDb.h"
    "MeltOptionsDb.cpp"
    "MeltSetting.h"
    "MeltSetting.cpp"
    "MeltChannelStorage.h"
    "MeltChannelDb.h"
    "MeltChannelDb.cpp"
    "MeltConsultElectrodeListModel.h"
    "MeltConsultElectrodeListModel.cpp"
    "MeltType.h"
)
target_sources(halve PRIVATE ${SRCS})

