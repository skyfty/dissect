# CMakeList.txt : CMake project for halve, include source and define
# project specific logic here.
#

set(SRCS
    "TreeModel.h"
    "TreeModel.cpp"
    "TreeElement.h"
    "TreeElement.cpp"
    "SerialPortListModel.h"
    "SerialPortListModel.cpp"
)
target_sources(halve PRIVATE ${SRCS})
