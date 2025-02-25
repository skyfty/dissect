#ifndef BLACKBOARDDB_H
#define BLACKBOARDDB_H

#include <QMutex>
#include <QObject>
#include <vtkSmartPointer.h>
#include <vtkVector.h>

class vtkImageData;
class vtkOctreePointLocator;
class vtkPlanes;
class vtkIdList;
class vtkPoints;
class vtkMatrix4x4;

class BlackboardDb : public QObject
{
    Q_OBJECT
public:
    explicit BlackboardDb(QObject *parent = nullptr);
    ~BlackboardDb() = default;
    static vtkSmartPointer<vtkImageData> createBlackBox();
    vtkSmartPointer<vtkImageData> clone(bool reset = true);
    vtkSmartPointer<vtkImageData> clone(const vtkSmartPointer<vtkIdList> &pointIds);
    vtkSmartPointer<vtkImageData> getImageData();
    qint32 depictPoint(const vtkSmartPointer<vtkIdList> &pointIds, int radius);
    void clean();
    void resetPoint(const vtkSmartPointer<vtkIdList> &pointIds);
    static void resetPoint(vtkSmartPointer<vtkImageData>,const vtkSmartPointer<vtkIdList>& pointIds);
    void setPoint(const vtkSmartPointer<vtkIdList> &pointIds, int value);
    void setPoint(vtkIdType id, int value);
    static void setPoint(vtkSmartPointer<vtkImageData>, const vtkSmartPointer<vtkIdList> &pointIds, int value);

    void findPointsWithRadius(double x, double y, double z, double radius , vtkIdList *ids) const;
    void findPointsWithRadius(const double position[3], double radius, vtkIdList *ids) const;
    void findPointsWithRadius(const vtkVector3d &position, double radius, vtkIdList *ids) const;
    vtkIdType findClosestPoint(double x, double y, double z) const;
    vtkIdType findClosestPoint(const double position[3]) const;
    vtkIdType findClosestPoint(const vtkVector3d &position) const;

    void getSeriePointIds(const vtkVector3d &b, const vtkVector3d &e, vtkIdList *ids);
    static vtkSmartPointer<vtkIdList> obtain(vtkImageData* imageData, vtkIdType type, QPolygon polygon,const vtkSmartPointer<vtkMatrix4x4>& mat, const vtkVector2i &windowSize,const vtkVector4d &viewport);
    vtkSmartPointer<vtkIdList> obtain(vtkIdType type, QPolygon polygon, const vtkSmartPointer<vtkMatrix4x4>& mat, const vtkVector2i& windowSize, const vtkVector4d& viewport);
    int getScalar(vtkIdType pointId) const;
    void getPoint(vtkIdType pointId, vtkVector3d &pos) const;
    vtkSmartPointer<vtkIdList> amassPoint() const;

signals:
    void changed();

private:
    static inline void polygonIsectLine(const QPointF &p1, const QPointF &p2, const QPointF &pos, int *winding);
    static void resetPoint(vtkSmartPointer<vtkImageData> &imageData, const vtkSmartPointer<vtkIdList>& pointIds, vtkIdType f, vtkIdType l);
    static void setPoint(vtkSmartPointer<vtkImageData>& imageData, const vtkSmartPointer<vtkIdList>& pointIds, int value, vtkIdType f, vtkIdType l);

private:
    vtkSmartPointer<vtkImageData> m_imageData;
    QMutex m_mutex;
};

#endif // BLACKBOARDDB_H
