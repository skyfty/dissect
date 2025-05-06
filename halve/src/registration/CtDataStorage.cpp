#include "CtDataStorage.h"
#include <vtkDICOMImageReader.h>
#include <vtkMatrix4x4.h>
#include <vtkMath.h>
#include <vtkNIFTIImageWriter.h>
#include "onnxmodule/ONNXInference.h"

namespace {
	vtkMatrix4x4* GetPatientMatrix(vtkDICOMImageReader* reader) {
		auto matrix = vtkMatrix4x4::New();

		float* vec = reader->GetImageOrientationPatient();
		double rowVec[3] = { vec[0],vec[1],vec[2] };
		double colVec[3] = { vec[3],vec[4],vec[5] };
		double normal[3];
		vtkMath::Cross(colVec, rowVec, normal);

		float* position = reader->GetImagePositionPatient();

		matrix->Element[0][0] = rowVec[0];
		matrix->Element[1][0] = rowVec[1];
		matrix->Element[2][0] = rowVec[2];

		matrix->Element[0][1] = colVec[0];
		matrix->Element[1][1] = colVec[1];
		matrix->Element[2][1] = colVec[2];

		matrix->Element[0][2] = normal[0];
		matrix->Element[1][2] = normal[1];
		matrix->Element[2][2] = normal[2];

		matrix->Element[0][3] = -position[0];
		matrix->Element[1][3] = -position[1];
		matrix->Element[2][3] = position[2];
		matrix->Element[3][3] = 1.0;

		return matrix;
	}
}

void CtDataStorage::convertCtDataToNifti(const QString& dicomDir)
{
	QString dstPath = dicomDir + "/" + kNiftiCacheName;
	bool isExist = QFile::exists(dstPath);
	if (isExist)
	{
		return;
	}
	QString srcPath = dicomDir + "/ctsrc.nii.gz";
	convertDicomToNifti(dicomDir, srcPath);
	ONNXInference onnx;
	onnx.onnxModelInference(srcPath, dstPath);
}

void CtDataStorage::convertDicomToNifti(const QString& dicomDir, const QString& niftiPath)
{
	auto dicomReader = vtkSmartPointer<vtkDICOMImageReader>::New();
	dicomReader->SetDirectoryName(dicomDir.toStdString().c_str());
	dicomReader->Update();

	vtkMatrix4x4* patientMatrix = GetPatientMatrix(dicomReader);

	auto niftiWriter = vtkSmartPointer<vtkNIFTIImageWriter>::New();
	niftiWriter->SetInputConnection(dicomReader->GetOutputPort());
	niftiWriter->SetFileName(niftiPath.toStdString().c_str());
	niftiWriter->SetQFormMatrix(patientMatrix);
	niftiWriter->SetQFac(-1.0);
	niftiWriter->Write();
}
