/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkPLYReader.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkPLYReader
 * @brief   read Stanford University PLY polygonal file format
 *
 * vtkPLYReader is a source object that reads polygonal data in
 * Stanford University PLY file format (see
 * http://graphics.stanford.edu/data/3Dscanrep). It requires that
 * the elements "vertex" and "face" are defined. The "vertex" element
 * must have the properties "x", "y", and "z". The "face" element must
 * have the property "vertex_indices" defined. Optionally, if the "face"
 * element has the properties "intensity" and/or the triplet "red",
 * "green", "blue", and optionally "alpha"; these are read and added as scalars
 * to the output data.
 * If the "face" element has the property "texcoord" a new TCoords
 * point array is created and points are duplicated if they have 2 or
 * more different texture coordinates. Points are duplicated only if
 * DuplicatePointsForFaceTexture is true (default).
 * This creates a polygonal data that can be textured without
 * artifacts. If unique points are required use a vtkCleanPolyData
 * filter after this reader or use this reader with DuplicatePointsForFaceTexture
 * set to false.
 *
 * @sa
 * vtkPLYWriter, vtkCleanPolyData
 */

#ifndef vtkPLYReader_h
#define vtkPLYReader_h

#include "vtkAbstractPolyDataReader.h"

class vtkStringArray;

class vtkCSVReader : public vtkAbstractPolyDataReader
{
public:
  vtkTypeMacro(vtkCSVReader, vtkAbstractPolyDataReader);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * Construct object with merging set to true.
   */
  static vtkCSVReader* New();

  /**
   * A simple, non-exhaustive check to see if a file is a valid ply file.
   */
  static int CanReadFile(VTK_FILEPATH const char* filename);

protected:
  vtkCSVReader();
  ~vtkCSVReader() override;


  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

private:
  vtkCSVReader(const vtkCSVReader&) = delete;
  void operator=(const vtkCSVReader&) = delete;
};

#endif
