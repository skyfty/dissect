# CMakeList.txt : CMake project for halve, include source and define
# project specific logic here.
#
set(SRCS
    "PolyDataTestWarp.h"
    "VtkDataWarp.h"
    "VtkDataWarp.cpp"
    "sqliteorm.h"
    "ModelCache.h"
    "ModelCache.cpp"
    "StartoverState.cpp"
    "StartoverState.h"
    "SimpleChecker.cpp"
    "SimpleChecker.h"
    "PanTompkins.cpp"
    "PanTompkins.h"
    "Thread.cpp"
    "Thread.h"
    "IOWorker.cpp"
    "IOWorker.h"
    "VtkUtil.cpp"
    "VtkUtil.h"
    "FileUtils.cpp"
    "FileUtils.h"
    "PointLocator.cpp"
    "PointLocator.h"
    "FrameRate.cpp"
    "FrameRate.h"
    "vtkMemberFunctionCommand.h"
    "exporter/ExporterHost.h"
    "exporter/ExporterHost.cpp"
    "exporter/Exporter.h"
    "exporter/Exporter.cpp"
    "ImageDataPointId.h"
    "ImageDataPointId.cpp"
)
target_sources(halve PRIVATE ${SRCS})

add_subdirectory(csv)
