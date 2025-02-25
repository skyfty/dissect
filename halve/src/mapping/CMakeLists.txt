# CMakeList.txt : CMake project for halve, include source and define
# project specific logic here.
#

set(SRCS
    "MappingPointStorage.h"
    "MappingPointsListModel.h"
    "MappingPointsListModel.cpp"
    "MappingPointsDb.h"
    "MappingPointsDb.cpp"
    "MappingPoint.h"
    "MappingPointGroup.h"
    "MappingPointGroup.cpp"
    "MappingPointGroupListModel.cpp"
    "MappingPointGroupListModel.h"
    "MappingPointGroupDb.cpp"
    "MappingPointGroupDb.h"
    "MappingPointSource.cpp"
    "MappingPointSource.h"
    "MappingLocationSource.cpp"
    "MappingLocationSource.h"
    "MappingPointVisbleFilter.cpp"
    "MappingPointVisbleFilter.h"
    "MappingPointSortFilterProxyModel.cpp"
    "MappingPointSortFilterProxyModel.h"
    "MappingPointGroupConsultFilterListModel.cpp"
    "MappingPointGroupConsultFilterListModel.h"
    "MappingPointRestrik.cpp"
    "MappingPointRestrik.h"
    "MappingPointCopyer.cpp"
    "MappingPointCopyer.h"
    "MappingReseauChecker.cpp"
    "MappingReseauChecker.h"
    "MappingSetting.h"
    "MappingSetting.cpp"
)
target_sources(halve PRIVATE ${SRCS})
