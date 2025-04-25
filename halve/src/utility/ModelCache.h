#ifndef MODEPOOL_H
#define MODEPOOL_H

#include "HalveType.h"
#include <QString>
#include <QMap>
#include <vtkSmartPointer.h>
#include <QFutureWatcher>
#include <QHash>
#include <vtkColor.h>

class vtkPolyData;
class vtkLookupTable;
class vtkNamedColors;

class ModelCache : public QObject
{
    Q_OBJECT
public:
    explicit ModelCache(QObject *parent);

public:
    static ModelCache* instance();
    void destroy();
    Q_INVOKABLE void prestrain();

    vtkSmartPointer<vtkPolyData> mesh(MeshName name);
    vtkSmartPointer<vtkLookupTable> lookupTable(LookupTableName name);
    const vtkStdString& colorName(Halve::CatheterElectrodeType name) const;
    static vtkSmartPointer<vtkPolyData> loadMeshFile(const char* fileName);

    vtkColor3d color3d(const vtkStdString& name);
    vtkColor3ub color3ub(const vtkStdString& name);
    vtkColor4d color4d(const vtkStdString& name);

Q_SIGNALS:
    void finished();

private:
    vtkSmartPointer<vtkPolyData> loadMesh(MeshName name);

    void initColors();

private:
    QMutex m_mutex;
    vtkSmartPointer<vtkNamedColors> m_nameColors;
    QFutureWatcher<void> *m_futureWatcher = nullptr;
    QHash<MeshName, vtkSmartPointer<vtkPolyData>> m_mesh;
    QHash<LookupTableName, vtkSmartPointer<vtkLookupTable>> m_lookupTable;
};

#endif // MODEPOOL_H
