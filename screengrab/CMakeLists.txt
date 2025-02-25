set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
add_library(screengrab
    ScreenGrab.h ScreenGrab.cpp)
target_include_directories(screengrab PUBLIC ".")

