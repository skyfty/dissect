# CMakeList.txt : CMake project for halve, include source and define
# project specific logic here.
#

set(SRCS
    "Picture.cpp"
    "Picture.h"
    "AlbumModel.h"
    "AlbumModel.cpp"
    "AlbumDb.h"
    "AlbumDb.cpp"
    "ScreenRecord.h"
    "ScreenRecord.cpp"
    "AlbumExporter.h"
    "AlbumExporter.cpp"
)
target_sources(halve PRIVATE ${SRCS})

