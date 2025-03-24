# CMakeList.txt : CMake project for halve, include source and define
# project specific logic here.
#
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
find_package(VTK REQUIRED COMPONENTS eigen)
if (VTK_FOUND)
    set(VTK_INCLUDE_DIRS "${VTK_PREFIX_PATH}/include/vtk-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}")
endif()
find_package(Qt6 REQUIRED COMPONENTS  Core)

set(SRCS
    "test.cpp"
    "ElecIdentify/ElecIdentify.cpp"
    "ElecIdentify/Elec2WorldUpdater.cpp"
    "ElecIdentify/DynamicNearestNeighbor.cpp"
    "ElecIdentify/ScaleFactorGetter.cpp"
    "ElecIdentify/Matrix3x4d.cpp"
)
add_library(ys ${SRCS})
target_link_libraries(ys
    PRIVATE
        VTK::eigen
        Qt::Core
)
target_include_directories(ys
    PRIVATE
        "${VTK_INCLUDE_DIRS}/vtkeigen"
)


target_include_directories(ys PUBLIC ".")
target_include_directories(ys PUBLIC "EcgMethod")
target_include_directories(ys PUBLIC "Algorithm")
target_include_directories(ys PUBLIC "Filter")
target_include_directories(ys PUBLIC "HeartRate")
target_include_directories(ys PUBLIC "EcgMethod/Neurokit")
target_include_directories(ys PUBLIC "3rd-party/findpeaks")
target_include_directories(ys PUBLIC "ElecIdentify")
target_include_directories(ys PUBLIC "3rd-party/hnswlib")
