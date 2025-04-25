#ifndef CATHETERMESH_H
#define CATHETERMESH_H

#include <QObject>
#include <vtkSmartPointer.h>

class vtkUnstructuredGrid;
class CatheterMesh : public QObject
{
    Q_OBJECT
public:
    explicit CatheterMesh(QObject *parent = nullptr);
    vtkUnstructuredGrid* grid();
    vtkSmartPointer<vtkUnstructuredGrid> mesh();
    void setMeshAndGrid(const vtkSmartPointer<vtkUnstructuredGrid> &mesh, const vtkSmartPointer<vtkUnstructuredGrid>& grid);

signals:
    void changed();

private:
    vtkSmartPointer<vtkUnstructuredGrid> m_grid;
    vtkSmartPointer<vtkUnstructuredGrid> m_mesh;
};

#endif // CATHETERMESH_H
