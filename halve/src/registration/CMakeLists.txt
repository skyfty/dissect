# CMakeList.txt : CMake project for halve, include source and define
# project specific logic here.
#

set(SRCS
    "RegistrationTargetStage.cpp"
    "RegistrationTargetStage.h"
    "RegistrationSourceStage.h"
    "RegistrationSourceStage.cpp"
)
target_sources(halve PRIVATE ${SRCS})

