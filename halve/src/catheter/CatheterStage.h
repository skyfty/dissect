#ifndef CATHETERSTAGE_H
#define CATHETERSTAGE_H

#include <QObject>
#include <QQuickVTKItem.h>
#include <QPointer>

class vtkCamera;
class vtkRenderWindow;
class CatheterStageData;
class vtkPolyData;
class CatheterElectrodeNodePointDb;
class MouseInteractorHighLightActor;
class vtkActor;
class vtkUnstructuredGrid;
class vtkPoints;
class Catheter;
class vtkLookupTable;
class vtkFollower;
class CatheterMould;

class CatheterStage : public QQuickVTKItem
{
    Q_OBJECT
    Q_PROPERTY(Catheter *catheter READ catheter WRITE setCatheter NOTIFY catheterChanged FINAL)

public:
    explicit CatheterStage(QQuickItem *parent = nullptr);
    ~CatheterStage() override = default;
    Q_INVOKABLE void resetRender();
    Q_INVOKABLE void resetCamera();
    Q_INVOKABLE void refresh();

    

    void setInteractor(vtkRenderWindow *renderWindow, CatheterStageData *userData);
    void setMouseInteractorStyle(CatheterStageData *userData);

    Catheter *catheter() const;
    void setCatheter(Catheter *newPointDb);
    void setPlaceholderCube(CatheterStageData* userData);

public slots:
    void onCatheterDyestuffChanged();

signals:
    void initRender();
    void catheterChanged();

private:
    vtkUserData initializeVTK(vtkRenderWindow* renderWindow) override;
    void destroyingVTK(vtkRenderWindow* renderWindow, vtkUserData userData) override;
    void refreshCatheterTube(vtkSmartPointer<vtkUnstructuredGrid> tubeMesh, const QList<vtkSmartPointer<vtkFollower>> &followers);
    void refreshCatheterTube();
    void resetTextFollower(CatheterStageData* userData, const QList<vtkSmartPointer<vtkFollower>> &followers);
    void cleanTextFollower(CatheterStageData* userData);
    vtkSmartPointer<vtkFollower> createCatheterLabelFollower(const QString &text, double pos[3]);
    QList<vtkSmartPointer<vtkFollower>> createCatheterLabelFollower(CatheterMould* catheterMould,vtkUnstructuredGrid* grid);

private:
    QPointer<Catheter> m_catheter;
    vtkSmartPointer<vtkLookupTable> m_lookupTable;


};

#endif // CATHETERSTAGE_H
