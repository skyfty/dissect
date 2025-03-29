set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIRS ON)
set(CUDA_SEPARABLE_COMPILATION ON)
set(CMAKE_CUDA_STANDARD 14)
set(CMAKE_CUDA_ARCHITECTURES OFF)

find_package(Qt6 REQUIRED COMPONENTS Quick Concurrent Charts Core Qml Gui RemoteObjects SerialPort Test LinguistTools)
qt_standard_project_setup(REQUIRES 6.6)

find_package(VTK REQUIRED)

if (VTK_FOUND)
    set(VTK_INCLUDE_DIRS "${VTK_PREFIX_PATH}/include/vtk-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}")
endif()

find_package(TBB REQUIRED)
if (HALVE_USE_CUDA)
    find_package(CUDA REQUIRED)
endif()
find_package(VTKm REQUIRED QUIET)

list(APPEND DEPENDS_LIBRARYS
    Qt::Charts
    Qt::Core
    Qt::Gui
    Qt::Qml
    Qt::Quick
    Qt::Concurrent
    Qt::RemoteObjects
    Qt::RemoteObjectsPrivate
    Qt::SerialPort
    ${VTK_LIBRARIES}
    vtkm::cont
    vtkm::worklet
    kddockwidgets
    breakpad
    sqlite_orm
    screengrab
    ys
)

if (HALVE_USE_CUDA)
    list(APPEND DEPENDS_LIBRARYS
        ${CUDA_LIBRARIES}
        ${CUDA_CUDART_LIBRARY}
        ${CUDA_cudadevrt_LIBRARY}
    )
endif()

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

if (HALVE_BUILD_TESTS)
    add_subdirectory(tests)
endif()

target_link_libraries(halve PRIVATE ${DEPENDS_LIBRARYS})

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
