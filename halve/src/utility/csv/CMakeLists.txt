set(SRCS
    "CSVFieldParser.cc"
    "CSVFieldParser.h"
    "CSVReader.h"
    "CSVReader.cc"
    "vtkCSVReader.h"
    "vtkCSVReader.cxx"
)
target_sources(halve PRIVATE ${SRCS})
