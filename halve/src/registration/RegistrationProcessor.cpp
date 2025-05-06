#include "RegistrationProcessor.h"
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkLandmarkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkAppendPolyData.h>
#include <vtkImageData.h>
#include <vtkPolyDataPointSampler.h>
#include "profile/Profile.h"
#include "mesh/BlackboardDb.h"
#include "reseau/ReseauDb.h"
#include <utility/IOWorker.h>
#include "mesh/Obscurity.h"

RegistrationProcessor::RegistrationProcessor(QObject* parent) : QObject(parent)
{
}

RegistrationProcessor::~RegistrationProcessor()
{
}

bool RegistrationProcessor::startRegistration(Profile* profile, Obscurity* obscurity, QVariantList visibleStates)
{
    if (!m_sourcePoints || !m_targetPoints ||
        m_sourcePoints->GetNumberOfPoints() != m_targetPoints->GetNumberOfPoints()) {
        //特征点数量不匹配或未初始化
        return false;
    }
    auto landmarkTransform = vtkSmartPointer<vtkLandmarkTransform>::New();
    landmarkTransform->SetSourceLandmarks(m_sourcePoints);
    landmarkTransform->SetTargetLandmarks(m_targetPoints);
    landmarkTransform->SetModeToRigidBody();

    auto appendFilter = vtkSmartPointer<vtkAppendPolyData>::New();
    for (int i = 0; i < visibleStates.size(); ++i) {
        QVariant item = visibleStates.at(i);
        if (item.isValid() && item.canConvert<bool>()) {
            bool visibleState = item.toBool();
            if (visibleState && i < m_sourcePolyDatas.size())
            {
                appendFilter->AddInputData(m_sourcePolyDatas.at(i));
            }
        }
    }

    auto transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    transformFilter->SetInputConnection(appendFilter->GetOutputPort());
    transformFilter->SetTransform(landmarkTransform);
    transformFilter->Update();
    vtkSmartPointer<vtkPolyData> sourcePolyData = transformFilter->GetOutput();

    auto fusedAppendFilter = vtkSmartPointer<vtkAppendPolyData>::New();
    fusedAppendFilter->AddInputData(sourcePolyData);
    fusedAppendFilter->AddInputData(m_targetPolyData);

    vtkSmartPointer<vtkPolyDataPointSampler> sampler = vtkSmartPointer<vtkPolyDataPointSampler>::New();
    sampler->SetInputConnection(fusedAppendFilter->GetOutputPort());
    sampler->SetDistance(0.3);
    sampler->Update();
    vtkSmartPointer<vtkPolyData> fusedPolyData = sampler->GetOutput();

    saveFusedPolyData(profile, obscurity, fusedPolyData);

    return true;
}

int RegistrationProcessor::undoType() const
{
    if (m_undoStack.isEmpty())
    {
        return 0;
    }
    int undoType = m_undoStack.back();
    return undoType;
}

void RegistrationProcessor::setUndoType(int undoType)
{
    m_undoStack.push_back(undoType);
}

void RegistrationProcessor::undo()
{
    if (m_undoStack.isEmpty())
    {
        return;
    }
    int undoType = m_undoStack.back();
    if (undoType == 1)
    {
        vtkIdType pointNumber = m_sourcePoints->GetNumberOfPoints();
        if (pointNumber > 0)
        {
            m_sourcePoints->SetNumberOfPoints(pointNumber - 1);
            m_sourcePoints->Modified();
        }
    }
    else if (undoType == -1)
    {
        vtkIdType pointNumber = m_targetPoints->GetNumberOfPoints();
        if (pointNumber > 0)
        {
            m_targetPoints->SetNumberOfPoints(pointNumber - 1);
            m_targetPoints->Modified();
        }
    }
    m_undoStack.pop_back();
}

void RegistrationProcessor::clearUndoStack()
{
    m_undoStack.clear();
}

vtkIdType RegistrationProcessor::calcOutlinePointIndex() const
{
    if (m_sourcePoints == nullptr || m_targetPoints == nullptr)
    {
        return -1;
    }
    vtkIdType sourceNumber = m_sourcePoints->GetNumberOfPoints();
    vtkIdType targetNumber = m_targetPoints->GetNumberOfPoints();
    if (sourceNumber == targetNumber)
    {
        return -1;
    }
    return sourceNumber < targetNumber ? sourceNumber : targetNumber;
}

void RegistrationProcessor::saveFusedPolyData(Profile* profile, Obscurity* obscurity, vtkSmartPointer<vtkPolyData> fusedPolyData)
{
    vtkNew<vtkIdList> ids;
    auto i = fusedPolyData->GetNumberOfPoints();
    double point[3] = { 0, };
    auto backboardDb = profile->blackboardDb();
    for (vtkIdType id = 0; id < i; id++) {
        fusedPolyData->GetPoint(id, point);
        vtkIdType pointId = backboardDb->findClosestPoint(point);
        if (pointId != -1) {
            ids->InsertNextId(pointId);
        }
    }

    auto reseauDb = profile->reseauDb();

    vtkSmartPointer<vtkPolyData> newPolyData = obscurity->extract(ids);;
    Reseau* newReseau = reseauDb->add(QDateTime::currentMSecsSinceEpoch(), "fused", QColor(255, 255, 255), ids, fusedPolyData);
    IOWorker::run([newReseau, newPolyData] {
        newReseau->savePolyData(newPolyData);
        newReseau->savePointIds();
        });
    reseauDb->save();
}
