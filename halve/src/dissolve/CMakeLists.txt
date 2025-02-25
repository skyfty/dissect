# CMakeList.txt : CMake project for halve, include source and define
# project specific logic here.
#

set(SRCS
    "DissolvePoint.h"
    "DissolveStorage.h"
    "DissolveDb.h"
    "DissolveDb.cpp"
    "DissolveListModel.h"
    "DissolveListModel.cpp"
    "DissolveOptions.h"
    "DissolveOptions.cpp"
    "DissolvePointSource.cpp"
    "DissolvePointSource.h"
    "DissolveDataSource.cpp"
    "DissolveDataSource.h"
    "DissolveSortFilterProxyModel.h"
    "DissolveSortFilterProxyModel.cpp"
    "DissolveElectrodeNodeListModel.h"
    "DissolveElectrodeNodeListModel.cpp"
)
target_sources(halve PRIVATE ${SRCS})

