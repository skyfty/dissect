# CMakeList.txt : CMake project for halve, include source and define
# project specific logic here.
#

set(SRCS
    "CasketListModel.h"
    "CasketListModel.cpp"
    "CasketDb.h"
    "CasketDb.cpp"
    "Casket.h"
    "Casket.cpp"
    "CasketFilterProxyModel.h"
    "CasketFilterProxyModel.cpp"
    "CasketCatheterListModel.h"
    "CasketCatheterListModel.cpp"
)
target_sources(halve PRIVATE ${SRCS})

