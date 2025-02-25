# CMakeList.txt : CMake project for halve, include source and define
# project specific logic here.
#

set(SRCS
    "AlarmListModel.h"
    "AlarmListModel.cpp"
    "AlarmDb.h"
    "AlarmDb.cpp"
    "Alarm.h"
    "Alarm.cpp"
)
target_sources(halve PRIVATE ${SRCS})
