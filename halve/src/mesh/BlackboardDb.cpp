#include <vtkSMPTools.h>
#include "BlackboardDb.h"
#include <vtkMetaImageReader.h>
#include <vtkImageData.h>
#include <vtkMetaImageWriter.h>
#include <vtkOctreePointLocator.h>
#include <vtkNew.h>
#include <vtkImageContinuousErode3D.h>
#include <QFile>
#include <vtkImageDataToPointSet.h>
#include <vtkImageOpenClose3D.h>
#include <QDir>
#include "utility/ImageDataPointId.h"
#include "vtkPointData.h"
#include "BlackboxSource.h"
#include <vtkXMLImageDataReader.h>
#include <vtkXMLImageDataWriter.h>
#include <vtkExtractGeometry.h>
#include <vtkPlanes.h>
#include <vtkPoints.h>
#include <vtkMatrix4x4.h>
#include <QPolygon>
#include "HalveType.h"
#include <vtkImageContinuousDilate3D.h>
#include <utility/Thread.h>
#include "combined/Bound.h"

BlackboardDb::BlackboardDb(QObject* parent)
    : QObject{parent} {
    Q_ASSERT(Thread::currentlyOn(Thread::UI));
    m_imageData = createBlackBox();
}

vtkSmartPointer<vtkImageData> BlackboardDb::createBlackBox() {
    vtkNew<BlackboxSource> blackboxSource;
    blackboxSource->SetDataScalar(ScalarsReset);
    blackboxSource->SetBounds(combined::getHoldingBound());
    blackboxSource->SetSpacing(DefaultSpacing);
    blackboxSource->Update();
    return blackboxSource->GetOutput();
}

vtkSmartPointer<vtkImageData> BlackboardDb::clone(bool reset){
    vtkSmartPointer<vtkImageData> imageData = vtkSmartPointer<vtkImageData>::New();
    imageData->DeepCopy(m_imageData);
    if(reset) {
        vtkDataArray* scalars = imageData->GetPointData()->GetScalars();
        scalars->Fill(ScalarsReset);
    }
    return imageData;
}
vtkSmartPointer<vtkImageData> BlackboardDb::clone(const vtkSmartPointer<vtkIdList> &pointIds) {
    vtkDataArray* srcScalars = m_imageData->GetPointData()->GetScalars();
    vtkSmartPointer<vtkImageData> imageData = createBlackBox();
    vtkDataArray* scalars = imageData->GetPointData()->GetScalars();
    vtkSMPTools::For(0, pointIds->GetNumberOfIds(), [&](vtkIdType f, vtkIdType l){
        for (vtkIdType i = f; i < l; i++) {
            vtkIdType pointId = pointIds->GetId(i) & MaskPointId;
            scalars->SetTuple1(pointId, srcScalars->GetTuple1(pointId));
        }
    });
    return imageData;
}

vtkSmartPointer<vtkImageData> BlackboardDb::getImageData() {
    Q_ASSERT(m_imageData != nullptr);
    return m_imageData;
}

void BlackboardDb::findPointsWithRadius(double x, double y, double z, double radius, vtkIdList *ids) const {
    Q_ASSERT(m_imageData != nullptr);
    ImageDataPointId imageDataPointId(m_imageData, DefaultSpacing);
    imageDataPointId.getPointsInRadius(x, y, z, radius,ids);
}

void BlackboardDb::getSeriePointIds(const vtkVector3d &start, const vtkVector3d &end, vtkIdList *ids) {
    ImageDataPointId imageDataPointId(m_imageData, DefaultSpacing);
    double direction[3] = {
        end.GetX() - start.GetX(),
        end.GetY() - start.GetY(),
        end.GetZ() - start.GetZ()
    };
    double length = vtkMath::Norm(direction);
    vtkMath::Normalize(direction);
    int numPoints = static_cast<int>(length / DefaultSpacing[0]);
    for (int i = 0; i <= numPoints; ++i) {
        double position[3]{
            start.GetX() + i * DefaultSpacing[0] * direction[0],
            start.GetY() + i * DefaultSpacing[1] * direction[1],
            start.GetZ() + i * DefaultSpacing[2] * direction[2]
        };
        vtkIdType id = imageDataPointId.getPoint(position);
        if (id != -1) {
            ids->InsertNextId(id);
        }
    }
}

void BlackboardDb::polygonIsectLine(const QPointF &p1, const QPointF &p2, const QPointF &pos, int *winding)
{
    auto x1 = p1.x();
    auto y1 = p1.y();
    auto x2 = p2.x();
    auto y2 = p2.y();
    auto y = pos.y();

    int dir = 1;

    if (qFuzzyCompare(y1, y2)) {
        return;
    } else if (y2 < y1) {
        auto x_tmp = x2; x2 = x1; x1 = x_tmp;
        auto y_tmp = y2; y2 = y1; y1 = y_tmp;
        dir = -1;
    }

    if (y >= y1 && y < y2) {
        auto x = x1 + ((x2 - x1) / (y2 - y1)) * (y - y1);
        if (x<=pos.x()) {
            (*winding) += dir;
        }
    }
}
vtkSmartPointer<vtkIdList> BlackboardDb::obtain(vtkIdType type, QPolygon polygon, const vtkSmartPointer<vtkMatrix4x4>& mat, const vtkVector2i& windowSize, const vtkVector4d& viewport) {
    return obtain(m_imageData, type, polygon, mat, windowSize, viewport);
}

vtkSmartPointer<vtkIdList> BlackboardDb::obtain(vtkImageData *imageData, vtkIdType type, QPolygon polygon, const vtkSmartPointer<vtkMatrix4x4>& matrix, const vtkVector2i &windowSize,const vtkVector4d &viewport){
    Q_ASSERT(matrix != nullptr);

    vtkSmartPointer<vtkIdList> ids = vtkSmartPointer<vtkIdList>::New();
    ids->Allocate(20000);

    QPoint lastStart(polygon[0]);
    double *mat = matrix->GetData();
    Q_ASSERT(mat != nullptr);
    int dimensions[3];
    imageData->GetDimensions(dimensions);
    double origin[3];
    imageData->GetOrigin(origin);
    vtkDataArray* scalars = imageData->GetPointData()->GetScalars();

    // 遍历图像数据的每个点
    for (int z = 0; z < dimensions[2]; ++z) {
        for (int y = 0; y < dimensions[1]; ++y) {
            for (int x = 0; x < dimensions[0]; ++x) {
                vtkIdType pointId = z * (dimensions[0] * dimensions[1]) + y * dimensions[0] + x;
                vtkIdType scalarData = scalars->GetTuple1(pointId);
                // 跳过不需要的点
                if (scalarData == ScalarsReset || (scalarData == ScalarsWounded && !(type & MaskWoundPointId)) || (scalarData == ScalarsFilling && !(type & MaskFillingPointId))) {
                    continue;
                }
                double val[3] = { origin[0] + x * DefaultSpacing[0], origin[1] + y * DefaultSpacing[1], origin[2] + z * DefaultSpacing[2] };
                double view[4];
                // 将点的坐标转换为视图坐标
                view[0] = val[0] * mat[0] + val[1] * mat[1] + val[2] * mat[2] + mat[3];
                view[1] = val[0] * mat[4] + val[1] * mat[5] + val[2] * mat[6] + mat[7];
                view[2] = val[0] * mat[8] + val[1] * mat[9] + val[2] * mat[10] + mat[11];
                view[3] = val[0] * mat[12] + val[1] * mat[13] + val[2] * mat[14] + mat[15];

                if (view[3] != 0.0) {
                    val[0] = view[0] / view[3];
                    val[1] = view[1] / view[3];
                    val[2] = view[2] / view[3];
                }
                // 将视图坐标转换为窗口坐标
                val[0] = (val[0] + 1.0) * (windowSize[0] * (viewport[2] - viewport[0])) / 2.0 + windowSize[0] * viewport[0];
                val[1] = (val[1] + 1.0) * (windowSize[1] * (viewport[3] - viewport[1])) / 2.0 + windowSize[1] * viewport[1];

                // 检查点是否在窗口内
                if (val[0] >= 0 && val[0] < windowSize[0] && val[1] >= 0 && val[1] < windowSize[1] ) {
                    QPoint pt(val[0], val[1]);
                    QPoint lastPt(polygon[0]);

                    int windingNumber = 0;
                    // 计算点是否在多边形内
                    for (qsizetype i2 = 1; i2 < polygon.size(); ++i2) {
                        QPoint e(polygon[i2]);
                        polygonIsectLine(lastPt, e, pt, &windingNumber);
                        lastPt = e;
                    }
                    if (lastPt != lastStart) {
                        polygonIsectLine(lastPt, lastStart, pt, &windingNumber);
                    }
                    // 如果点在多边形内，添加到结果列表中
                    if ((windingNumber % 2) != 0) {
                        if (scalarData == ScalarsWounded) {
                            pointId = pointId | MaskWoundPointId;
                        } else if (scalarData == ScalarsFilling) {
                            pointId = pointId | MaskFillingPointId;
                        }
                        ids->InsertNextId(pointId);
                    }
                }
            }
        }
    }
    return ids;
}

void BlackboardDb::findPointsWithRadius(const double position[3], double radius, vtkIdList *ids)  const{
    findPointsWithRadius(position[0], position[1],position[2],radius,ids);
}

void BlackboardDb::findPointsWithRadius(const vtkVector3d &position, double radius, vtkIdList *ids) const
{
    findPointsWithRadius(position.GetX(), position.GetY(),position.GetZ(),radius,ids);
}

vtkIdType BlackboardDb::findClosestPoint(double x, double y, double z) const {
    ImageDataPointId imageDataPointId(m_imageData, DefaultSpacing);
    return imageDataPointId.getPoint(x, y, z);
}

vtkIdType BlackboardDb::findClosestPoint(const double position[3]) const {
    return findClosestPoint(position[0], position[1],position[2]);

}
vtkIdType BlackboardDb::findClosestPoint(const vtkVector3d &position) const {
    return findClosestPoint(position.GetX(), position.GetY(),position.GetZ());
}

qint32 BlackboardDb::depictPoint(const vtkSmartPointer<vtkIdList> &pointIds, int radius) {
    Q_ASSERT(m_imageData != nullptr);
    Q_ASSERT(pointIds != nullptr);
    QMutexLocker locker(&m_mutex);
    qint32 depictCount = 0;
    vtkDataArray* scalars = m_imageData->GetPointData()->GetScalars();
    for (vtkIdType i = 0; i < pointIds->GetNumberOfIds(); i++) {
        vtkIdType pointId = pointIds->GetId(i) & MaskPointId;
        vtkNew<vtkIdList> ids;
        findPointsWithRadius(m_imageData->GetPoint(pointId), radius, ids);
        for (vtkIdType j = 0; j < ids->GetNumberOfIds(); j++) {
            vtkIdType id = ids->GetId(j);
            if (scalars->GetTuple1(id) == ScalarsWounded) {
                depictCount++;
                scalars->SetTuple1(id, ScalarsReset);
            }
        }

        int scalarsData = scalars->GetTuple1(pointId);
        if (scalarsData != ScalarsSet) {
            depictCount++;
            scalars->SetTuple1(pointId, ScalarsSet);
        }
    }
    if (depictCount > 0) {
        m_imageData->Modified();
    }
    return depictCount;
}

void BlackboardDb::clean() {
    Q_ASSERT(m_imageData != nullptr);
    QMutexLocker locker(&m_mutex);
    vtkDataArray* scalars = m_imageData->GetPointData()->GetScalars();
    scalars->Fill(ScalarsReset);
    m_imageData->Modified();
    emit changed();
}

void BlackboardDb::resetPoint(const vtkSmartPointer<vtkIdList> &pointIds) {
    if (pointIds->GetNumberOfIds() <= 0) {
        return;
    }
    QMutexLocker locker(&m_mutex);
    resetPoint(m_imageData, pointIds);
    emit changed();
}
 
void BlackboardDb::resetPoint(vtkSmartPointer<vtkImageData>& imageData, const vtkSmartPointer<vtkIdList>& pointIds, vtkIdType f, vtkIdType l) {
    vtkDataArray* scalars = imageData->GetPointData()->GetScalars();
    for (vtkIdType i = f; i < l; i++) {
        vtkIdType pointId = pointIds->GetId(i);
        if (pointId & MaskWoundPointId) {
            scalars->SetTuple1(pointId & MaskPointId, ScalarsWounded);
        } else if (pointId & MaskFillingPointId) {
            scalars->SetTuple1(pointId & MaskPointId, ScalarsReset);
        } else {
            scalars->SetTuple1(pointId & MaskPointId, ScalarsSet);
        }
    }
}

void BlackboardDb::resetPoint(vtkSmartPointer<vtkImageData> imageData, const vtkSmartPointer<vtkIdList>& pointIds) {
    vtkIdType numIds = pointIds->GetNumberOfIds();
    if (numIds > 200) {
        vtkSMPTools::For(0, numIds, [&](vtkIdType f, vtkIdType l) {
            resetPoint(imageData, pointIds, f, l);
        });
    } else {
        resetPoint(imageData, pointIds, 0, numIds);
    }
    imageData->Modified();
}

void BlackboardDb::setPoint(const vtkSmartPointer<vtkIdList> &ids, int value) {
    QMutexLocker locker(&m_mutex);
    setPoint(m_imageData, ids, value);
    emit changed();
}
void BlackboardDb::setPoint(vtkIdType pointId, int value) {
    QMutexLocker locker(&m_mutex);
    vtkDataArray* scalars = m_imageData->GetPointData()->GetScalars();
    scalars->SetTuple1(pointId & MaskPointId, value);
    m_imageData->Modified();
    emit changed();
}
void BlackboardDb::setPoint(vtkSmartPointer<vtkImageData> &imageData, const vtkSmartPointer<vtkIdList>& pointIds, int value, vtkIdType f, vtkIdType l) {
    vtkDataArray* scalars = imageData->GetPointData()->GetScalars();
    for (vtkIdType i = f; i < l; i++) {
        vtkIdType pointId = pointIds->GetId(i) & MaskPointId;
        scalars->SetTuple1(pointId, value);
    }
}

void BlackboardDb::setPoint(vtkSmartPointer<vtkImageData> imageData, const vtkSmartPointer<vtkIdList> &pointIds, int value) {
    vtkDataArray* scalars = imageData->GetPointData()->GetScalars();
    vtkIdType numIds = pointIds->GetNumberOfIds();
    if (numIds > 200) {
        vtkSMPTools::For(0, numIds, [&](vtkIdType f, vtkIdType l) {
            setPoint(imageData, pointIds, value, f, l);
        });
    } else {
        setPoint(imageData, pointIds, value, 0, numIds);
    }
    imageData->Modified();
}

int BlackboardDb::getScalar(vtkIdType pointId) const {
    Q_ASSERT(m_imageData != nullptr);
    vtkDataArray* scalars = m_imageData->GetPointData()->GetScalars();
    return scalars->GetTuple1(pointId & MaskPointId);
}

void BlackboardDb::getPoint(vtkIdType pointId, vtkVector3d& pos) const
{
    Q_ASSERT(m_imageData != nullptr);
    m_imageData->GetPoint(pointId & MaskPointId, pos.GetData());
}

vtkSmartPointer<vtkIdList> BlackboardDb::amassPoint() const {
    vtkSmartPointer<vtkIdList> ids = vtkSmartPointer<vtkIdList>::New();
    ids->Allocate(20000);
    vtkDataArray* scalars = m_imageData->GetPointData()->GetScalars();
    for (vtkIdType i = 0; i < scalars->GetNumberOfTuples(); i++) {
        vtkIdType scalarData = scalars->GetTuple1(i);
        if (scalarData == ScalarsSet) {
            ids->InsertNextId(i);
        } else if (scalarData == ScalarsWounded) {
            ids->InsertNextId(i | MaskWoundPointId);
        }
    }
    return ids;
}
