# CMakeList.txt : CMake project for halve, include source and define
# project specific logic here.
#

set(SRCS
    "main.cpp"
    "HalveType.h"
    "Translator.cpp"
    "Translator.h"
    "QmlRegisterTypes.cpp"
)
target_sources(halve PRIVATE ${SRCS})

add_subdirectory(utility)
add_subdirectory(widget)
add_subdirectory(combined)
add_subdirectory(user)
add_subdirectory(profile)
add_subdirectory(oscillograp)
add_subdirectory(catheter)
add_subdirectory(melt)
add_subdirectory(album)
add_subdirectory(mesh)
add_subdirectory(study)
add_subdirectory(mapping)
add_subdirectory(channel)
add_subdirectory(dissolve)
add_subdirectory(casket)
add_subdirectory(estate)
add_subdirectory(terminal)
add_subdirectory(reseau)
add_subdirectory(halitus)
add_subdirectory(notebook)
add_subdirectory(registration)
add_subdirectory(onnxmodule)
