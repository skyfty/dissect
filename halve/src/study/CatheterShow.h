#ifndef CATHETERSHOW_H
#define CATHETERSHOW_H

#include <QQuickVTKItem.h>
#include <vtkObject.h>

class CatheterShowData;
class Catheter;
class CatheterMesh;

class CatheterShow : public QQuickVTKItem
{
    Q_OBJECT
    Q_PROPERTY(Catheter *catheter READ catheter WRITE setCatheter NOTIFY catheterChanged FINAL)

public:
    explicit CatheterShow(QQuickItem* parent = nullptr);
    ~CatheterShow() override = default;
    Q_INVOKABLE void resetRender();
    Q_INVOKABLE void resetCamera();

    void setInteractor(vtkRenderWindow *renderWindow, CatheterShowData *userData);
    void setMouseInteractorStyle(CatheterShowData *userData);
    void setLight(CatheterShowData *userData);

    Catheter *catheter() const;
    void setCatheter(Catheter *newPointDb);


public slots:
    void onCatheterDyestuffChanged();


signals:
    void initRender();
    void catheterChanged();

private:
    void createTextFollower(CatheterShowData* userData, vtkIdType id, double pos[3]);
    vtkUserData initializeVTK(vtkRenderWindow* renderWindow) override;
    void destroyingVTK(vtkRenderWindow* renderWindow, vtkUserData userData) override;

private:
    QPointer<Catheter> m_catheter;

};

#endif // CATHETERSHOW_H
