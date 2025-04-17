set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIRS ON)

set(CUDA_SEPARABLE_COMPILATION ON)
set(CMAKE_CUDA_STANDARD 14)
set(CMAKE_CUDA_ARCHITECTURES OFF)

# 读取 version.txt
set(VERSION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/version.txt")
set(RC_FILE "${CMAKE_CURRENT_BINARY_DIR}/halve.rc")  # 现在输出到 build 目录
set(RC_IN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/halve.rc.in")
set(HALVE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/halve.ico")

file(READ "${VERSION_FILE}" VERSION_STRING)
string(STRIP "${VERSION_STRING}" VERSION_STRING)
string(REPLACE "." "," VERSION_COMMA "${VERSION_STRING}")
configure_file("${RC_IN_FILE}" "${RC_FILE}")

find_package(Qt6 REQUIRED COMPONENTS Quick Concurrent Charts Core Qml Gui RemoteObjects SerialPort Test LinguistTools)
qt_standard_project_setup(REQUIRES 6.6)

find_package(TBB REQUIRED)
find_package(VTKm REQUIRED)
find_package(VTK REQUIRED)
set(VTK_INCLUDE_DIRS "${VTK_PREFIX_PATH}/include/vtk-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}")

list(INSERT CMAKE_MODULE_PATH 0
  "${VTK_INCLUDE_DIRS}/ThirdParty/vtkm/vtkvtkm/vtk-m/CMake")
  
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/config.h"
)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

qt_add_executable(halve "resource.qrc"  "${RC_FILE}")

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
target_link_libraries(halve PRIVATE ${DEPENDS_LIBRARYS})

target_include_directories(halve
    PRIVATE
        "${VTK_INCLUDE_DIRS}/vtksqlite"
        "${VTK_INCLUDE_DIRS}/vtkeigen"
        "${VTK_INCLUDE_DIRS}/vtkvtkm/vtk-m"
        "${VTK_INCLUDE_DIRS}/vtkvtkm/vtk-m/vtkm/thirdparty/diy/vtkmdiy/include"
        "${VTK_INCLUDE_DIRS}/vtkvtkm/vtk-m/vtkm/thirdparty/lcl/vtkmlcl"
)
set_target_properties(halve PROPERTIES WIN32_EXECUTABLE TRUE)

qt_import_qml_plugins(halve)
qt_finalize_executable(halve)

include(GNUInstallDirs)
install(TARGETS halve
    LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if (HALVE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

