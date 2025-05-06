# CMakeList.txt : CMake project for halve, include source and define
# project specific logic here.
#

set(SRCS
    "ONNXInference.h"
    "ONNXInference.cpp"
)
target_sources(halve PRIVATE ${SRCS})

