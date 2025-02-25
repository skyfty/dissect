
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

include_directories(..)
include_directories(../src)
set(TESTING_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/test_resources.qrc)
set(TESTING_SRCS utils.cpp)
set(CMAKE_CUDA_STANDARD 14)

function(add_halve_test test srcs)
    qt_add_executable(${test} ${srcs} ${TESTING_RESOURCES} ${TESTING_SRCS} )
    target_link_libraries(${test} PRIVATE Qt::Core Qt::Gui Qt::Test)
    target_include_directories(${test} PRIVATE ${CMAKE_BINARY_DIR})
endfunction()

add_halve_test(tst_receiver tst_receiver.cpp)
add_halve_test(tst_pantompkins tst_pantompkins.cpp)
