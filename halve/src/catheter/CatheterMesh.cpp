#include "CatheterMesh.h"

#include <vtkUnstructuredGrid.h>
#include <utility/Thread.h>

CatheterMesh::CatheterMesh(QObject *parent)
    : QObject{parent}
{
    Q_ASSERT(Thread::currentlyOn(Thread::UI));
}

void CatheterMesh::setMesh(const vtkSmartPointer<vtkUnstructuredGrid> & mesh) {
    if (mesh == nullptr || mesh == m_mesh) {
        return;
    }
    m_mesh = mesh;
    emit changed();
}

vtkSmartPointer<vtkUnstructuredGrid> CatheterMesh::mesh() {
    Q_ASSERT(Thread::currentlyOn(Thread::UI));
    if (m_mesh == nullptr) {
        m_mesh = vtkSmartPointer<vtkUnstructuredGrid>::New();
    }
    return m_mesh;
}
vtkUnstructuredGrid* CatheterMesh::grid() {
    Q_ASSERT(Thread::currentlyOn(Thread::UI));
    if (m_grid == nullptr) {
        m_grid = vtkSmartPointer<vtkUnstructuredGrid>::New();
    }
    return m_grid;
}
