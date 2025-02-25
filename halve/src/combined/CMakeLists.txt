# CMakeList.txt : CMake project for halve, include source and define
# project specific logic here.
#

set(SRCS
    "Combined.cpp"
    "Combined.h"
    "TrackData.h"
    "TrackData.cpp"
    "Detector.h"
    "Detector.cpp"
    "Bound.h"
    "Bound.cpp"
    "CatheterTrack.h"
    "CatheterTrack.cpp"
    "TrackDataListModel.h"
    "TrackDataListModel.cpp"
    "NdiStorageAdapter.h"
    "CombinedDb.h"
    "CombinedDb.cpp"
    "Blend.cpp"
    "Blend.h"
    "BlendDint.cpp"
    "BlendDint.h"
    "BlendMagnetism.cpp"
    "BlendMagnetism.h"
    "CatheterTrackPackage.cpp"
    "CatheterTrackPackage.h"
    "InspectReproduceCatheter.cpp"
    "ConvertTrackData.cpp"
)
target_sources(halve PRIVATE ${SRCS})
