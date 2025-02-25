# CMakeList.txt : CMake project for halve, include source and define
# project specific logic here.
#

set(SRCS
    "ScalarModel.h"
    "ScalarModel.cpp"
    "ScalarDb.h"
    "ScalarDb.cpp"
    "Scalar.h"
    "Scalar.cpp"
    "ScalarOptions.h"
    "ScalarOptions.cpp"
    "ScalarColor.h"
    "ScalarColor.cpp"
)
target_sources(halve PRIVATE ${SRCS})
