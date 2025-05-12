# CMakeList.txt : CMake project for halve, include source and define
# project specific logic here.
#

set(SRCS
    "RegistrationStage.h"
    "RegistrationStage.cpp"
    "RegistrationTargetStage.cpp"
    "RegistrationTargetStage.h"
    "RegistrationSourceStage.h"
    "RegistrationSourceStage.cpp"
    "RegistrationProcessor.h"
    "RegistrationProcessor.cpp"
    "RegistrationInteractorStyle.h"
    "RegistrationInteractorStyle.cpp"
    "CtDataStorage.h"
    "CtDataStorage.cpp"
    "ProgressCommand.h"
    "ProgressCommand.cpp"
    "ProgressReporter.h"
    "ProgressReporter.cpp"
)
target_sources(halve PRIVATE ${SRCS})

