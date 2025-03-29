# CMakeList.txt : CMake project for halve, include source and define
# project specific logic here.
#

set(SRCS
    "BlackboxSource.cpp"
    "BlackboxSource.h"
    "ImageDataRefineFilter.cpp"
    "ImageDataRefineFilter.h"
    "ImageDataSqueezeFilter.cpp"
    "ImageDataSqueezeFilter.h"
    "Obscurity.h"
    "Obscurity.cpp"
    "BlackboardDb.h"
    "BlackboardDb.cpp"
    "ObscurityWorker.h"
    "ObscurityWorker.cpp"
    "ReproduceOptions.h"
    "ReproduceOptions.cpp"
    "MeshImporter.h"
    "MeshImporter.cpp"
    "DilateErode.h"
    "DilateErode.cu"
    "Carpenter.cpp"
    "Carpenter.h"
    "CarpenterSource.cpp"
    "CarpenterSource.h"
    "ImageOpenClose.cpp"
    "ImageOpenClose.h"
    "ReactWorker.cpp"
    "ReactWorker.h"
)
target_sources(halve PRIVATE ${SRCS})
