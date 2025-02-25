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
    void setMesh(const vtkSmartPointer<vtkUnstructuredGrid> &mesh);

signals:
    void changed();

private:
    vtkSmartPointer<vtkUnstructuredGrid> m_grid;
    vtkSmartPointer<vtkUnstructuredGrid> m_mesh;
};

#endif // CATHETERMESH_H
