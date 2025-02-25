# CMakeList.txt : CMake project for halve, include source and define
# project specific logic here.
#

set(SRCS
    "Alleyway.h"
    "Alleyway.cpp"
    "FpgaTest.h"
    "FpgaTest.cpp"
)
target_sources(halve PRIVATE ${SRCS})
