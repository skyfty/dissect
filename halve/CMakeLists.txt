set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIRS ON)
set(CUDA_SEPARABLE_COMPILATION ON)
set(CMAKE_CUDA_STANDARD 14)

find_package(Qt6 REQUIRED COMPONENTS Quick Concurrent Charts Core Qml Gui RemoteObjects SerialPort Test LinguistTools)
qt_standard_project_setup(REQUIRES 6.6)

find_package(TBB REQUIRED)

find_package(VTK REQUIRED)
if (VTK_FOUND)
    set(VTK_INCLUDE_DIRS "${VTK_PREFIX_PATH}/include/vtk-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}")
endif()

find_package(VTKm REQUIRED QUIET)

if (POLICY CMP0146)
    cmake_policy(SET CMP0146 OLD)
endif()

# find_package(CUDA REQUIRED)
# LIST(APPEND CUDA_NVCC_FLAGS -gencode arch=compute_52,code=sm_52 -rdc=true)
# enable_language(CUDA)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

qt_add_executable(halve "resource.qrc" "halve.rc")

qt_add_resources(halve "configuration"
    PREFIX "/"
    FILES
        qtquickcontrols2.conf
)
qt6_add_repc_merged(halve
    src/combined/combined.rep
    src/channel/channel.rep
    src/profile/storagechecker.rep
    src/utility/exporter/exporter.rep
)

qt_add_translations(halve
    RESOURCE_PREFIX "/i18n"
    TS_FILES
        i18n/en.ts
        i18n/zh.ts
)

include_directories("src")
add_subdirectory(src)
add_subdirectory(tests)

set_target_properties(halve PROPERTIES
  WIN32_EXECUTABLE TRUE
  cuda_architecture_flags "--generate-code=arch=compute_89,code=sm_89"
  requires_static_builds TRUE)
set_target_properties(halve PROPERTIES CUDA_ARCHITECTURES "OFF")


target_link_libraries(halve
    PRIVATE
        Qt::Charts
        Qt::Core
        Qt::Gui
        Qt::Qml
        Qt::Quick
        Qt::Concurrent
        Qt::RemoteObjects
        Qt::RemoteObjectsPrivate
        Qt::SerialPort
        VTK::GUISupportQt
        VTK::GUISupportQtQuick
        VTK::IOPLY
        VTK::IOGeometry
        VTK::IOXML
        VTK::IOImage
        VTK::IOLegacy
        VTK::RenderingAnnotation
        VTK::RenderingContextOpenGL2
        VTK::RenderingCore
        VTK::RenderingFreeType
        VTK::RenderingGL2PSOpenGL2
        VTK::RenderingOpenGL2
        # VTK::AcceleratorsVTKmCore
        # VTK::AcceleratorsVTKmDataModel
        # VTK::AcceleratorsVTKmFilters

        VTK::png
        VTK::sqlite
        VTK::eigen
        VTK::FiltersCore
        VTK::FiltersSources
        VTK::CommonDataModel
        VTK::CommonCore
        VTK::InteractionStyle
        VTK::CommonComputationalGeometry
        VTK::FiltersGeometry
        VTK::FiltersHybrid
        VTK::FiltersExtraction
        VTK::FiltersPoints
        VTK::CommonColor
        VTK::ImagingHybrid
        VTK::ImagingSources
        VTK::ImagingMorphological
        VTK::ImagingStencil
        VTK::RenderingContext2D
        VTK::RenderingContextOpenGL2
        VTK::FiltersStatistics
        VTK::InfovisCore
        vtkm::cont
        vtkm::io
        vtkm::worklet
        vtkm::filter
        ${CUDA_LIBRARIES}
        ${CUDA_CUDART_LIBRARY}
        ${CUDA_cudadevrt_LIBRARY}
        ys
        kddockwidgets
        breakpad
        sqlite_orm
        screengrab)

target_include_directories(halve
    PRIVATE
        "${VTK_INCLUDE_DIRS}/vtksqlite"
        "${VTK_INCLUDE_DIRS}/vtkeigen"
        "${VTK_INCLUDE_DIRS}/vtkvtkm/vtk-m"
        "${VTK_INCLUDE_DIRS}/vtkvtkm/vtk-m/vtkm/thirdparty/diy/vtkmdiy/include"
        "${VTK_INCLUDE_DIRS}/vtkvtkm/vtk-m/vtkm/thirdparty/lcl/vtkmlcl"
)


qt_import_qml_plugins(halve)
qt_finalize_executable(halve)

include(GNUInstallDirs)
install(TARGETS halve
    LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
