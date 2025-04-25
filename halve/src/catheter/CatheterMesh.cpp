#include "CatheterMesh.h"

#include <vtkUnstructuredGrid.h>
#include <utility/Thread.h>

CatheterMesh::CatheterMesh(QObject *parent)
    : QObject{parent}
{
    Q_ASSERT(Thread::currentlyOn(Thread::UI));
}

void CatheterMesh::setMeshAndGrid(const vtkSmartPointer<vtkUnstructuredGrid>& mesh, const vtkSmartPointer<vtkUnstructuredGrid>& grid) {
    bool c = false;
    if (grid != nullptr && grid != m_grid) {
        m_grid = grid;
        c = true;

    }
    if (mesh != nullptr && mesh != m_mesh) {
        m_mesh = mesh;
        c = true;
    }
    if (c) {
        emit changed();
    }
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
