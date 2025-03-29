cmake_minimum_required(VERSION 3.16)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

project(dissect VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/halve)

enable_language(CUDA)

option(HALVE_BUILD_TESTS "Build Tests" OFF)
option(HALVE_USE_CUDA "Enable CUDA complite" OFF)

add_subdirectory ("third_party")
add_subdirectory ("ys")
add_subdirectory ("screengrab")
add_subdirectory ("halve")
