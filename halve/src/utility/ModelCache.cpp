#include "ModelCache.h"
#include <QResource>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <vtkCylinderSource.h>
#include <vtkPLYReader.h>
#include <QtConcurrent>
#include <vtkLookupTable.h>
#include <vtkPolyData.h>
#include <QColor>
#include <vtkColor.h>
#include <vtkNamedColors.h>
#include <vtkSphereSource.h>
#include <vtkCubeSource.h>
#include <vtkPlatonicSolidSource.h>
#include "HalveType.h"
#include <vtkArrowSource.h>

static ModelCache *modelCache = nullptr;

ModelCache::ModelCache(QObject *parent)
    :QObject(parent)
{
    m_nameColors = vtkNew<vtkNamedColors>();
    modelCache = this;
}

ModelCache* ModelCache::instance() {
    return modelCache;
}

void ModelCache::destroy() {
    if (modelCache != nullptr) {
        modelCache->m_mesh.clear();
        modelCache->m_lookupTable.clear();
    }
    delete modelCache;
    modelCache = nullptr;
}
MeshName PrestrainList[] = {MeshName::ORIENTATION};

void ModelCache::prestrain() {
    initColors();

    m_futureWatcher = new QFutureWatcher<void>(this);
    QObject::connect(m_futureWatcher, &QFutureWatcher<void>::finished, this, [this]() {
        emit finished();
    });
    m_futureWatcher->setFuture(QtConcurrent::map(std::begin(PrestrainList), std::end(PrestrainList),[this](MeshName name) {
        vtkSmartPointer<vtkPolyData> polyData = loadMesh(name);
        if (polyData != nullptr) {
            QMutexLocker locker(&m_mutex);
            m_mesh[name] = polyData;
        }
    }));
}


vtkSmartPointer<vtkPolyData> ModelCache::mesh(MeshName name) {
    auto iter = m_mesh.find(name);
    if (iter != std::end(m_mesh)) {
        return iter.value();
    }
    vtkSmartPointer<vtkPolyData> polyData = loadMesh(name);
    m_mesh[name] = polyData;
    return polyData;
}


vtkSmartPointer<vtkPolyData> ModelCache::loadMesh(MeshName name) {
    if (name == MeshName::ORIENTATION) {
        return loadMeshFile(":/assets/model/body.ply");
    }
    if (name == MeshName::ELECTRODE_NODE) {
        return loadMeshFile(":/assets/model/catheter_node.ply");
    }
    if (name == MeshName::PANT0_ELECTRODE_NODE) {
        vtkNew<vtkCylinderSource> source;
        source->SetRadius(ElectrodeNodeRadius);
        source->Update();
        return source->GetOutput();
    }
    if (name == MeshName::PANT1_ELECTRODE_NODE) {
        vtkNew<vtkCubeSource> source;
        source->SetXLength(0.6);
        source->SetYLength(0.6);
        source->SetZLength(0.6);
        source->Update();
        return source->GetOutput();
    }
    if (name == MeshName::MN_DISSOLVE_POINT) {
        vtkNew<vtkPlatonicSolidSource> source;
        source->SetSolidTypeToIcosahedron();
        source->Update();
        return source->GetOutput();
    }
    if (name == MeshName::MN_MAPPING_3D_POINT) {
        vtkNew<vtkPlatonicSolidSource> source;
        source->SetSolidTypeToOctahedron();
        source->Update();
        return source->GetOutput();
    }
    if (name == MeshName::MN_MAPPING_SURFACE_POINT) {
        vtkNew<vtkSphereSource> source;
        source->SetRadius(1);
        source->Update();
        return source->GetOutput();
    }
    Q_ASSERT(false);
    return nullptr;
}

vtkSmartPointer<vtkPolyData> ModelCache::loadMeshFile(const char *fileName) {
    QResource resource(fileName);
    auto data = resource.uncompressedData();
    vtkNew<vtkPLYReader> reader;
    reader->SetInputString(std::string(data.data(), data.size()));
    reader->SetReadFromInputString(true);
    reader->Update();
    return reader->GetOutput();
}


vtkColor3d ModelCache::color3d(const vtkStdString& name) {
    return m_nameColors->GetColor3d(name);
}

vtkColor3ub ModelCache::color3ub(const vtkStdString& name) {
    return m_nameColors->GetColor3ub(name);
}
vtkColor4d ModelCache::color4d(const vtkStdString& name) {
    return m_nameColors->GetColor4d(name);
}

void ModelCache::initColors() {
}

static std::pair<Halve::CatheterElectrodeType, vtkStdString> CatheterElectrodeColorsNameMap[] = {
    {Halve::CET_TUBE, "LightGrey"},
    {Halve::CET_MAG, "Red"},
    {Halve::CET_PANT, "Yellow"},
    {Halve::CET_ASSIST, "DarkSeaGreen"},
    {Halve::CET_ORIGIN, "LightCyan"},
    {Halve::CET_HANDLE, "Green"},
};

const vtkStdString& ModelCache::colorName(Halve::CatheterElectrodeType name) const {
    Q_ASSERT(name >= 0 && name <= Halve::CET_HANDLE);
    if (name > Halve::CET_HANDLE) {
        name =  Halve::CET_HANDLE;
    }
    return CatheterElectrodeColorsNameMap[name].second;
}

vtkSmartPointer<vtkLookupTable> ModelCache::lookupTable(LookupTableName name) {
    if (m_lookupTable.contains(name)) {
        return m_lookupTable.value(name);
    }
    if (name == LookupTableName::LTN_CATHETER) {
        vtkIdType range = std::size(CatheterElectrodeColorsNameMap);
        vtkSmartPointer<vtkLookupTable> lookupTable = vtkSmartPointer<vtkLookupTable>::New();
        lookupTable->SetNumberOfTableValues(range);
        lookupTable->SetRange(0, range);
        for(const auto &colorName:CatheterElectrodeColorsNameMap) {
            lookupTable->SetTableValue(colorName.first, color4d(colorName.second).GetData());
        }
        lookupTable->Build();
        m_lookupTable[name] = lookupTable;
        return lookupTable;
    }
    if (name == LookupTableName::LTN_POINTCLOUD) {
        vtkSmartPointer<vtkLookupTable> lookupTable = vtkSmartPointer<vtkLookupTable>::New();

        lookupTable->SetNumberOfTableValues(3);
        lookupTable->SetRange(0, 2);
        lookupTable->SetTableValue(0, color4d("Yellow").GetData());
        lookupTable->SetTableValue(1, color4d("Blue").GetData());
        lookupTable->SetTableValue(2, color4d("Green").GetData());

        lookupTable->Build();
        m_lookupTable[name] = lookupTable;
        return lookupTable;
    }
    Q_ASSERT(false);
    return nullptr;
}
