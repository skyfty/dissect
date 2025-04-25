#include "CatheterTubeFilter.h"
#include "HalveType.h"
#include "vtkObjectFactory.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"
#include "vtkDataObject.h"
#include "utility/VtkUtil.h"
#include <vtkAppendFilter.h>
#include <vtkParametricFunctionSource.h>
#include <vtkParametricSpline.h>
#include <vtkPolyLine.h>
#include <vtkTubeFilter.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPointData.h>
#include <utility/ModelCache.h>
#include <vtkGlyph3D.h>
#include <vtkCleanPolyData.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkLandmarkTransform.h>
#include <vtkDoubleArray.h>
#include <vtkCylinderSource.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkCellData.h>
#include <vtkPolyDataNormals.h>
#include <vtkQuad.h>
#include <QDateTime.h>
#include <QtConcurrent>

vtkStandardNewMacro(CatheterTubeFilter);

//------------------------------------------------------------------------------
int CatheterTubeFilter::FillInputPortInformation(int port, vtkInformation * info)
{
    if (!this->Superclass::FillInputPortInformation(port, info))
    {
        return 0;
    }
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkUnstructuredGrid");
    return 1;
}

vtkSmartPointer<vtkPolyData> CatheterTubeFilter::makeTube(
    vtkUnstructuredGrid* input0,
    const QList<QPair<vtkIdType, vtkVector3d>> &pointDirectionsList, double radius, int sides, int resolution) {
    vtkDoubleArray* nodeFlexibilityArray = dynamic_cast<vtkDoubleArray*>(input0->GetPointData()->GetArray(FlexibilityName));
    vtkNew<vtkPoints> points;
    for (const auto& pd : pointDirectionsList) {
        vtkIdType pointId = pd.first;
        double nodeFlexibility = nodeFlexibilityArray->GetValue(pointId);
        vtkVector3d direction = pd.second;
        double dirNorm = vtkMath::Norm(direction.GetData());
        double normalizedDir[3] = {
            direction[0] / dirNorm,
            direction[1] / dirNorm,
            direction[2] / dirNorm};
        vtkMath::MultiplyScalar(normalizedDir, nodeFlexibility / 2); // Scale the direction vector

        double origin[3]{}, frontPoint[3]{}, backPoint[3]{};
        input0->GetPoint(pointId, origin);
        vtkMath::Add(origin, normalizedDir, frontPoint);
        vtkMath::Subtract(origin, normalizedDir, backPoint);
        points->InsertNextPoint(backPoint);
        points->InsertNextPoint(origin);
        points->InsertNextPoint(frontPoint);
    }
    vtkNew<vtkParametricSpline> spline;
    spline->SetPoints(points);
    spline->ParameterizeByLengthOn();

    vtkNew<vtkParametricFunctionSource> functionSource;
    functionSource->SetParametricFunction(spline);
    functionSource->SetUResolution(resolution);
    functionSource->Update();

    vtkNew<vtkTubeFilter> tubeFilter;
    tubeFilter->SetInputConnection(functionSource->GetOutputPort());
    tubeFilter->SetRadius(radius);   
    tubeFilter->CappingOn();
    tubeFilter->SetNumberOfSides(sides);
    tubeFilter->Update();

    return tubeFilter->GetOutput();
}

typedef QList< QPair<vtkIdType, vtkVector3d> > PointDirectionsListType;
typedef QList<QPair<vtkIdType, PointDirectionsListType>> PointDirectionsListListType;

//------------------------------------------------------------------------------
int CatheterTubeFilter::RequestData(vtkInformation * vtkNotUsed(request),
                                        vtkInformationVector * *inputVector, vtkInformationVector * outputVector)
{
    // Retrieve input and output
    vtkInformation* inInfo0 = inputVector[0]->GetInformationObject(0);
    vtkInformation* outInfo = outputVector->GetInformationObject(0);

    vtkUnstructuredGrid* input0 = vtkUnstructuredGrid::SafeDownCast(inInfo0->Get(vtkDataObject::DATA_OBJECT()));
    vtkUnstructuredGrid* output = vtkUnstructuredGrid::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));
    if (input0 == nullptr || output == nullptr) {
        vtkErrorMacro(<< "Input Output data is nullptr.");
        return 0;
    }

    vtkIntArray* nodeMeshIndex = dynamic_cast<vtkIntArray*>(input0->GetPointData()->GetArray(NodeMeshIndexName));
    vtkUnsignedCharArray* nodeMeshColors = dynamic_cast<vtkUnsignedCharArray*>(input0->GetPointData()->GetArray(ColorsPointDataName));
    vtkDoubleArray* connectivityRadius = dynamic_cast<vtkDoubleArray*>(input0->GetCellData()->GetArray(ConnectivityRadiusName));
    vtkIntArray* connectivityResolution = dynamic_cast<vtkIntArray*>(input0->GetCellData()->GetArray(ResolutionName));
    vtkIntArray* connectivitySides = dynamic_cast<vtkIntArray*>(input0->GetCellData()->GetArray(TubeSidesName));

	QMap<vtkIdType, QList<QPair<vtkIdType,vtkVector3d>>> pointDirections;

    PointDirectionsListListType pointDirectionsListList;
    for(int i = 0; i < input0->GetNumberOfCells(); ++i) {
        vtkPolyLine *line = dynamic_cast<vtkPolyLine*>(input0->GetCell(i));
        vtkNew<vtkPoints> points;
		vtkNew<vtkIdList> meshPointIds;
        for(int j = 0; j < line->GetNumberOfPoints(); ++j) {
			vtkIdType pointId = line->GetPointIds()->GetId(j);
            double pp[3]{};
            input0->GetPoint(pointId, pp);
            points->InsertNextPoint(pp);
			meshPointIds->InsertNextId(pointId);
        }
        vtkNew<vtkParametricSpline> spline;
        spline->SetPoints(points);
        spline->ParameterizeByLengthOn();

        vtkNew<vtkParametricFunctionSource> functionSource;
        functionSource->SetParametricFunction(spline);
        functionSource->SetUResolution(connectivityResolution->GetValue(i));
        functionSource->Update();

        vtkIdType numElectrodes = points->GetNumberOfPoints(); // Number of electrodes
        double totalLength = 0.0;
        vtkNew<vtkDoubleArray> cumulativeLengths;
        cumulativeLengths->SetNumberOfValues(numElectrodes);
        cumulativeLengths->SetValue(0, 0.0);

        for (int i1 = 1; i1 < numElectrodes; ++i1) {
            double prev[3], curr[3];
            points->GetPoint(i1 - 1, prev);
            points->GetPoint(i1, curr);
            double dx = curr[0] - prev[0];
            double dy = curr[1] - prev[1];
            double dz = curr[2] - prev[2];
            totalLength += std::sqrt(dx * dx + dy * dy + dz * dz);
            cumulativeLengths->SetValue(i1, totalLength);
        }

        PointDirectionsListType pointDirectionsList;
        for (int i1 = 0; i1 < numElectrodes; ++i1) {
            double c1 = cumulativeLengths->GetValue(i1);

            double t1 = 0.0;
            double t2 = 0.0;
            if (i1 == 0) {
                t1 = 0.0;
                t2 = Length;
            }
            else if (i1 == numElectrodes - 1) {
                t1 = c1 - Length;
                t2 = c1;
            }
            else {
                t1 = c1 - Length;
                t2 = c1 + Length;
            }
            double t11 = t1 / totalLength;
            double t22 = t2 / totalLength;
            double pCurrent[3]{}, pNext[3]{};

            double Du[3]{};
            spline->Evaluate(&t11, pCurrent, Du);
            spline->Evaluate(&t22, pNext, Du);

            double tangent[3] = { 0.0, 0.0, 0.0 };
            tangent[0] = pNext[0] - pCurrent[0];
            tangent[1] = pNext[1] - pCurrent[1];
            tangent[2] = pNext[2] - pCurrent[2];

            double norm = vtkMath::Norm(tangent);
            if (norm > 0.0) {
                tangent[0] /= norm;
                tangent[1] /= norm;
                tangent[2] /= norm;
            }
			vtkIdType meshPointId = meshPointIds->GetId(i1);
            auto idx = nodeMeshIndex->GetValue(meshPointId);
            auto pointTangent = std::make_pair(meshPointId, vtkVector3d(tangent[0], tangent[1], tangent[2]));
            pointDirections[idx].append(pointTangent);
            pointDirectionsList.append(pointTangent);
        }
        pointDirectionsListList.append(std::make_pair(i,pointDirectionsList));
    }

    auto future = QtConcurrent::mapped(pointDirectionsListList, [&](const QPair<vtkIdType, PointDirectionsListType>& pointDirectionsListPair) {
        auto idx = pointDirectionsListPair.first;
        vtkSmartPointer<vtkPolyData> tubePolyData = makeTube(input0, 
            pointDirectionsListPair.second,
            connectivityRadius->GetValue(idx), connectivitySides->GetValue(idx),connectivityResolution->GetValue(idx));
        vtkSmartPointer<vtkUnsignedCharArray> colors = vtkSmartPointer<vtkUnsignedCharArray>::New();
        colors->SetName(ColorsPointDataName);
        colors->SetNumberOfComponents(3);
        for (int j = 0; j < tubePolyData->GetNumberOfPoints(); ++j) {
            colors->InsertNextTypedTuple(Color.GetData());
        }
        tubePolyData->GetPointData()->SetScalars(colors);
        return tubePolyData;
    });

    vtkUnsignedCharArray* pointColorArray = dynamic_cast<vtkUnsignedCharArray*>(input0->GetPointData()->GetArray(ColorsPointDataName));
    auto pointDirectionKeys = pointDirections.keys();
    auto pointDirectionsFuture = QtConcurrent::mapped(pointDirectionKeys, [&](vtkIdType meshIndex) {
        vtkSmartPointer<vtkPolyData> polyData = nullptr;
        if (meshIndex == -1) {
            return polyData;
        }
        auto idx = nodeMeshIndex->GetValue(meshIndex);
        if (idx >= m_meshPolyDatas.size()) {
            return polyData;
        }
        const QList<QPair<vtkIdType, vtkVector3d>>& pointDirectionsList = pointDirections[meshIndex];
        vtkSmartPointer<vtkPolyData> nodePolyData = m_meshPolyDatas[idx];

        vtkNew<vtkPoints> points;
        vtkNew<vtkDoubleArray> directions;
        directions->SetName(PointDirectionName);
        directions->SetNumberOfComponents(3);
        vtkNew<vtkUnsignedCharArray> colors;
        colors->SetNumberOfComponents(3);
        colors->SetName(ColorsPointDataName);

        for (const auto& pointDirection : pointDirectionsList) {
            vtkIdType pointId = pointDirection.first;
            double pp[3]{};
            input0->GetPoint(pointId, pp);
            points->InsertNextPoint(pp);
            vtkVector3d direction = pointDirection.second;
            directions->InsertNextTuple3(direction[0], direction[1], direction[2]);
            unsigned char color[3];
            pointColorArray->GetTypedTuple(pointId, color);
            colors->InsertNextTypedTuple(color);
        }

        vtkNew<vtkPolyData> electrodeData;
        electrodeData->SetPoints(points);
        electrodeData->GetPointData()->SetVectors(directions);
        electrodeData->GetPointData()->SetActiveVectors(PointDirectionName);
        electrodeData->GetPointData()->SetScalars(colors);

        vtkNew<vtkGlyph3D> glyph3D;
        glyph3D->SetInputData(electrodeData);
        glyph3D->SetSourceData(nodePolyData);
        glyph3D->OrientOn();
        glyph3D->ScalingOff();
        glyph3D->SetGeneratePointIds(true);
        glyph3D->SetVectorModeToUseVector();
        glyph3D->SetColorModeToColorByScalar();
        glyph3D->Update();
        polyData = glyph3D->GetOutput();
        return polyData;
    });

    vtkNew<vtkAppendFilter> appendFilter;
    for (int i = 0; i < pointDirectionKeys.size(); ++i) {
        vtkSmartPointer<vtkPolyData> tubePolyData = pointDirectionsFuture.resultAt(i);
        if (tubePolyData != nullptr) {
            appendFilter->AddInputData(tubePolyData);
        }
    }
    for (int i = 0; i < pointDirectionsListList.size(); ++i) {
        vtkSmartPointer<vtkPolyData> tubePolyData = future.resultAt(i);
        if (tubePolyData != nullptr) {
            appendFilter->AddInputData(tubePolyData);
        }
    }
    appendFilter->Update();

    output->ShallowCopy(appendFilter->GetOutput());
    return 1;
}
