#include "MeshImporter.h"
#include "vtkPLYReader.h"
#include "vtkXMLPolyDataReader.h"

#include <QFileInfo>
#include <QtConcurrent>
#include <vtkCallbackCommand.h>
#include <vtkOBJReader.h>
#include <vtkPolyDataReader.h>
#include <vtkSTLReader.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include "utility/VtkUtil.h"

#include <profile/Profile.h>

MeshImporter::MeshImporter(QObject *parent)
    : QObject{parent}
{}
void MeshImporter::open(const QString& filePath) {
    setRunning(true);
    QFileInfo fileInfo(filePath);
    QString extension = fileInfo.suffix().toLower();
    vtkSmartPointer<vtkPolyData> polyData = vtkutil::importPolyData(filePath);
    if (polyData != nullptr) {
        double bounds[6];
        polyData->GetBounds(bounds);
        double centerX = (bounds[0] + bounds[1]) / 2.0;
        double centerY = (bounds[2] + bounds[3]) / 2.0;
        double centerZ = (bounds[4] + bounds[5]) / 2.0;

        vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
        transform->Scale(0.34375, 0.34375, 0.5);
        transform->Translate(-centerX, -centerY, -centerZ);

        vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
        transformFilter->SetInputData(polyData);
        transformFilter->SetTransform(transform);
        transformFilter->Update();
    }
    setRunning(false);
}

Profile* MeshImporter::profile() const
{
    return m_profile;
}

void MeshImporter::setProfile(Profile *newProfile)
{
    if (m_profile == newProfile)
        return;
    m_profile = newProfile;
    emit profileChanged();
}

bool MeshImporter::running() const
{
    return m_running;
}

void MeshImporter::setRunning(bool newRunning)
{
    if (m_running == newRunning)
        return;
    m_running = newRunning;
    emit runningChanged();
}
