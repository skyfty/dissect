# CMakeList.txt : CMake project for halve, include source and define
# project specific logic here.
#

set(SRCS
    "Reseau.h"
    "Reseau.cpp"
    "ReseauDb.h"
    "ReseauDb.cpp"
    "ReseauSortFilterProxyModel.h"
    "ReseauSortFilterProxyModel.cpp"
    "ReseauListModel.h"
    "ReseauListModel.cpp"
)
target_sources(halve PRIVATE ${SRCS})
