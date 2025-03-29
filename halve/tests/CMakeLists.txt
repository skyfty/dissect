include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

include_directories(..)
include_directories(../src)
set(TESTING_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/test_resources.qrc)
set(TESTING_SRCS utils.cpp)
set(CMAKE_CUDA_STANDARD 14)

list(APPEND DEPENDS_TEST_LIBRARYS
    Qt::Core
    Qt::Gui
    Qt::Test
    vtkm::cont
    vtkm::worklet
)

if (HALVE_USE_CUDA)
    list(APPEND DEPENDS_TEST_LIBRARYS
        ${CUDA_LIBRARIES}
        ${CUDA_CUDART_LIBRARY}
        ${CUDA_cudadevrt_LIBRARY}
    )
endif()

function(add_halve_test test srcs)
    qt_add_executable(${test} ${srcs} ${TESTING_RESOURCES} ${TESTING_SRCS} )
    target_link_libraries(${test} PRIVATE ${DEPENDS_TEST_LIBRARYS})
    target_include_directories(${test} PRIVATE ${CMAKE_BINARY_DIR})
endfunction()

add_halve_test(tst_receiver tst_receiver.cpp)
add_halve_test(tst_pantompkins tst_pantompkins.cpp)


add_executable(tst_vtkm tst_vtkm.cpp vtkm_worklet.cu)
target_link_libraries(tst_vtkm PRIVATE ${DEPENDS_TEST_LIBRARYS})

add_executable(tst_dilate_erode  tst_dilate_erode.cpp ../src/mesh/DilateErode.h  ../src/mesh/DilateErode.cu)
target_link_libraries(tst_dilate_erode PRIVATE ${DEPENDS_TEST_LIBRARYS})