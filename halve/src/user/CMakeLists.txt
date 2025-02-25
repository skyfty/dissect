# CMakeList.txt : CMake project for halve, include source and define
# project specific logic here.
#

set(SRCS
    "User.cpp"
    "User.h"
    "UserListModel.h"
    "UserListModel.cpp"
    "UserExporter.cpp"
    "UserExporter.h"
    "UserListSortFilterProxyModel.h"
    "UserListSortFilterProxyModel.cpp"
)
target_sources(halve PRIVATE ${SRCS})
