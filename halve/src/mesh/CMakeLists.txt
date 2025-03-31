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
    "Carpenter.cpp"
    "Carpenter.h"
    "ImageOpenClose.cpp"
    "ImageOpenClose.h"
    "CarpenterSource.cpp"
    "CarpenterSource.h"
)
if(HALVE_USE_CUDA)
    list(APPEND SRCS
       "DilateErode.h"
       "DilateErode.cu"
    )
endif()

target_sources(halve PRIVATE ${SRCS})
