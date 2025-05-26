#include "CtDataStorage.h"
#include <vtkDICOMImageReader.h>
#include <vtkMatrix4x4.h>
#include <vtkMath.h>
#include <vtkImageData.h>
#include <vtkNIFTIImageWriter.h>
#include "onnxmodule/ONNXInference.h"
#include "utility/IOWorker.h"
#include "ProgressCommand.h"

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

bool CtDataStorage::convertCtDataToNifti(const QString& dicomDir)
{
	assert(m_progressReporter);
	m_progressReporter->setProgressText("");
	QString dstPath = dicomDir + "/" + kNiftiCacheName;
	bool isExist = QFile::exists(dstPath);
	if (!isExist)
	{
		QEventLoop waitLoop;
		QFutureWatcher<bool> watcher;
		QObject::connect(&watcher, &QFutureWatcher<void>::finished, &waitLoop, &QEventLoop::quit);
		auto future = IOWorker::run([dicomDir, dstPath, this] {
			QString srcPath = dicomDir + "/ctsrc.nii.gz";
			bool suc = convertDicomToNifti(dicomDir, srcPath);
			if (suc)
			{
				ONNXInference onnx;
				onnx.setProgressReporter(m_progressReporter);
				onnx.onnxModelInference(srcPath, dstPath);
			}
			return suc;
			});
		watcher.setFuture(future);
		waitLoop.exec();
		if (!watcher.result())
		{
			return false;
		}
	}
	if (m_progressReporter)
	{
		m_progressReporter->setProgressText("loading finished");
		m_progressReporter->complete();
	}
	return true;
}

void CtDataStorage::setProgressReporter(ProgressReporter* reporter)
{
	if (m_progressReporter == reporter)
	{
		return;
	}
	m_progressReporter = reporter;
}

ProgressReporter* CtDataStorage::progressReporter() const
{
	return m_progressReporter;
}

bool CtDataStorage::convertDicomToNifti(const QString& dicomDir, const QString& niftiPath)
{
	if (m_progressReporter == nullptr)
	{
		return false;
	}
	vtkNew<ProgressCommand> progressCommand;
	progressCommand->setProgressReporter(m_progressReporter);
	m_progressReporter->setProgressText("reading dicom data...");
	m_progressReporter->setStepWeight(0.05);
	auto dicomReader = vtkSmartPointer<vtkDICOMImageReader>::New();
	dicomReader->AddObserver(vtkCommand::ProgressEvent, progressCommand);
	dicomReader->SetDirectoryName(dicomDir.toStdString().c_str());
	dicomReader->Update();

	vtkMatrix4x4* patientMatrix = GetPatientMatrix(dicomReader);
	vtkImageData* imageData = dicomReader->GetOutput();
	if (imageData == nullptr || imageData->GetNumberOfPoints() == 0)
	{
		m_progressReporter->setProgressText("failed to read CT data");
		return false;
	}
	m_progressReporter->completeStep();
	m_progressReporter->setStepWeight(0.15);
	m_progressReporter->setProgressText("writing dicom nifti...");
	auto niftiWriter = vtkSmartPointer<vtkNIFTIImageWriter>::New();
	niftiWriter->AddObserver(vtkCommand::ProgressEvent, progressCommand);
	niftiWriter->SetInputConnection(dicomReader->GetOutputPort());
	niftiWriter->SetFileName(niftiPath.toStdString().c_str());
	niftiWriter->SetQFormMatrix(patientMatrix);
	niftiWriter->SetQFac(-1.0);
	niftiWriter->Write();
	m_progressReporter->completeStep();
	return true;
}
