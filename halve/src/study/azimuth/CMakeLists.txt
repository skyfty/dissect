# CMakeList.txt : CMake project for halve, include source and define
# project specific logic here.
#

set(SRCS
    "AzimuthModel.h"
    "AzimuthModel.cpp"
    "AzimuthDb.h"
    "AzimuthDb.cpp"
    "AzimuthBuildinDb.h"
    "AzimuthBuildinDb.cpp"
    "AzimuthProxyModel.h"
    "AzimuthProxyModel.cpp"
    "Azimuth.h"
    "Azimuth.cpp"
)
target_sources(halve PRIVATE ${SRCS})
