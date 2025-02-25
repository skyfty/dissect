# CMakeList.txt : CMake project for halve, include source and define
# project specific logic here.
#

set(SRCS
    "Notebook.h"
    "Notebook.cpp"
    "NotebookDb.h"
    "NotebookDb.cpp"
    "NotebookListModel.h"
    "NotebookListModel.cpp"
    "NotebookSnapshot.h"
    "NotebookSnapshot.cpp"
    "NotebookGenre.h"
    "NotebookGenre.cpp"
    "NotebookTreeModel.h"
    "NotebookTreeModel.cpp"
    "NotebookOptions.h"
    "NotebookOptions.cpp"
    "NotebookAutoRecord.h"
    "NotebookAutoRecord.cpp"
    "NotebookMapping.h"
    "NotebookMapping.cpp"
    "NotebookScreenRecord.h"
    "NotebookScreenRecord.cpp"
    "NotebookScreenshot.h"
    "NotebookScreenshot.cpp"
    "NotebookFile.h"
    "NotebookFile.cpp"
    "NotebookDataSource.h"
    "NotebookDataSource.cpp"
    "NotebookSortFilterModel.h"
    "NotebookSortFilterModel.cpp"
)
target_sources(halve PRIVATE ${SRCS})
