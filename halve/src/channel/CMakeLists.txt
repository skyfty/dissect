# CMakeList.txt : CMake project for halve, include source and define
# project specific logic here.
#

set(SRCS
    "ChannelData.h"
    "ChannelDataDb.h"
    "ChannelDataDb.cpp"
    "ChannelDataCalculate.cpp"
    "ChannelDataCalculate.h"
    # "CalculateWorklet.cu"
    "CalculateWorklet.h"
    "ChannelDataFilter.cpp"
    "ChannelDataFilter.h"
    "ChannelStorageWorket.cpp"
    "ChannelStorageWorket.h"
    "ChannelHost.cpp"
    "ChannelHost.h"
    "ChannelWorker.cpp"
    "ChannelWorker.h"
    "Channel.cpp"
    "Channel.h"
    "ChannelDataStorage.h"
    "ChannelTrackStorage.h"
    "ChannelTrackStorageAdapter.h"
    "ChannelTrack.h"
    "ChannelTrackData.h"
    "ChannelTrackDb.cpp"
    "ChannelTrackDb.h"
    "MainWindow.ui"
    "ChannelWatchWindow.cpp"
    "ChannelWatchWindow.h"
    "ChannelTrackDataMagneticTableModel.cpp"
    "ChannelTrackDataMagneticTableModel.h"
    "ChannelTrackDataElectricalTableModel.cpp"
    "ChannelTrackDataElectricalTableModel.h"
)
target_sources(halve PRIVATE ${SRCS})
