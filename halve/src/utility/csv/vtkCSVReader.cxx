/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkCSVReader.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkCSVReader.h"

#include "vtkCellData.h"
#include "vtkDataArray.h"
#include "vtkFloatArray.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkNew.h"
#include "vtkObjectFactory.h"
#include "vtkPLY.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolygon.h"
#include "vtkSmartPointer.h"
#include "vtkStringArray.h"
#include "vtkUnsignedCharArray.h"

#include <vtksys/SystemTools.hxx>

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <vector>
#include "CSVReader.h"

vtkStandardNewMacro(vtkCSVReader);

// Construct object with merging set to true.
vtkCSVReader::vtkCSVReader()
{
}

vtkCSVReader::~vtkCSVReader()
{
}

int vtkCSVReader::RequestData(vtkInformation* vtkNotUsed(request),
  vtkInformationVector** vtkNotUsed(inputVector), vtkInformationVector* outputVector)
{
  // get the info object
  vtkInformation* outInfo = outputVector->GetInformationObject(0);

  // get the output
  vtkPolyData* output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

  std::ifstream ifs(this->FileName);
  if (!ifs) {
    vtkWarningMacro(<< "Could not open PLY file");
    return 0;
  }
  ifs.seekg(0,std::ios::end);//文件指针指向结尾
  auto fileSize = ifs.tellg();
  if (fileSize <= 0) {
    vtkWarningMacro(<< "Could not open PLY file");
    return 0;
  }
  ifs.seekg(0, std::ios::beg);
  std::string buffer;
  buffer.resize(fileSize);
  ifs.read(&buffer[0], fileSize);

  CSVTable table;
  if (!table.ReadCSV(buffer)) {
    vtkWarningMacro(<< "Could not open PLY file");
    return 0;
  }

  vtkNew<vtkPoints> points;
  vtkNew<vtkFloatArray> pointNormals;
  pointNormals->SetNumberOfComponents(3); // 3d normals (ie x,y,z)
  for(const auto &record : table.records()) {
    double scalarData[4];
    scalarData[0] = std::stod(record.at("Q0"));
    scalarData[1] = std::stod(record.at("Qx"));
    scalarData[2] = std::stod(record.at("Qy"));
    scalarData[3] = std::stod(record.at("Qz"));
    pointNormals->InsertNextTuple(&scalarData[1]);

    double pointData[3];
    pointData[0] = std::stod(record.at("Tx"));
    pointData[1] = std::stod(record.at("Ty"));
    pointData[2] = std::stod(record.at("Tz"));
    points->InsertNextPoint(pointData);
  }
  vtkNew<vtkPolyData> polyData;
  polyData->SetPoints(points);
  polyData->GetPointData()->SetNormals(pointNormals);
  output->ShallowCopy(polyData);

  return 1;
}

int vtkCSVReader::CanReadFile(const char* filename)
{
  FILE* fd = vtksys::SystemTools::Fopen(filename, "rb");
  if (!fd)
    return 0;

  char line[4] = {};
  const char* result = fgets(line, sizeof(line), fd);
  fclose(fd);
  return (result && strncmp(result, "ply", 3) == 0);
}

void vtkCSVReader::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);

  os << indent << "Comments:\n";
  indent = indent.GetNextIndent();
}
