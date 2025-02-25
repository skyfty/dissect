# CMakeList.txt : CMake project for halve, include source and define
# project specific logic here.
#

set(SRCS
    "SignalSourceOptions.h"
    "SignalSourceOptions.cpp"
    "OscillograpChartListModel.cpp"
    "OscillograpChartListModel.h"
    "OscillograpDataSource.h"
    "OscillograpDataSource.cpp"
    "OscillograpElectrode.h"
    "OscillograpElectrode.cpp"
    "PantCalculate.h"
    "PantCalculate.cpp"
    "PantElectrodeListNode.h"
    "PantElectrodeListNode.cpp"
    "OscillograpOptions.h"
    "OscillograpOptions.cpp"
    "OscillograpCatheterElectrodeNodeListModel.h"
    "OscillograpCatheterElectrodeNodeListModel.cpp"
)
target_sources(halve PRIVATE ${SRCS})
