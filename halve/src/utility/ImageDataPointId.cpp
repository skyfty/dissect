#include "ImageDataPointId.h"
#include <vtkIdList.h>
#include <vtkImageData.h>
#include <vtkVector.h>

ImageDataPointId::ImageDataPointId(vtkImageData *imageData, const double *spacing) {
    m_spacing = spacing;
    m_imageData = imageData;
    m_imageData->GetOrigin(m_origin);
    m_imageData->GetDimensions(m_dimensions);
}

vtkIdType ImageDataPointId::getPoint(const vtkVector3d &pos) const {
    return getPoint(pos.GetX(), pos.GetY(), pos.GetZ());
}
vtkIdType ImageDataPointId::getPoint(const double *pos) const {
    return getPoint(pos[0], pos[1], pos[2]);
}

vtkIdType ImageDataPointId::getPoint(double x, double y, double z) const {
    int ijk[3]{};
    ijk[0] = static_cast<int>((x - m_origin[0]) / m_spacing[0]);
    ijk[1] = static_cast<int>((y - m_origin[1]) / m_spacing[1]);
    ijk[2] = static_cast<int>((z - m_origin[2]) / m_spacing[2]);

    ijk[0] = std::max(0, std::min(ijk[0], m_dimensions[0] - 1));
    ijk[1] = std::max(0, std::min(ijk[1], m_dimensions[1] - 1));
    ijk[2] = std::max(0, std::min(ijk[2], m_dimensions[2] - 1));

    vtkIdType pointIndex = ijk[2] * m_dimensions[0] * m_dimensions[1] + ijk[1] * m_dimensions[0] + ijk[0];
    return pointIndex;
}
void ImageDataPointId::getPointsInRadius(const vtkVector3d& pos, double radius, vtkIdList* idTotalList) const {
    return getPointsInRadius(pos.GetX(), pos.GetY(), pos.GetZ(), radius, idTotalList);

}
void ImageDataPointId::getPointsInRadius(const double* pos, double radius, vtkIdList* idTotalList) const {
    return getPointsInRadius(pos[0], pos[1], pos[2], radius, idTotalList);

}

void ImageDataPointId::getPointsInRadius(double x, double y, double z, double radius, vtkIdList* idTotalList) const {
    std::vector<vtkIdType> pointIds;
    int ijk[3]{};
    int radiusInVoxels[3] = {
        static_cast<int>(radius / m_spacing[0]),
        static_cast<int>(radius / m_spacing[1]),
        static_cast<int>(radius / m_spacing[2])
    };

    int centerIJK[3] = {
        static_cast<int>((x - m_origin[0]) / m_spacing[0]),
        static_cast<int>((y - m_origin[1]) / m_spacing[1]),
        static_cast<int>((z - m_origin[2]) / m_spacing[2])
    };

    for (int i = -radiusInVoxels[0]; i <= radiusInVoxels[0]; ++i) {
        for (int j = -radiusInVoxels[1]; j <= radiusInVoxels[1]; ++j) {
            for (int k = -radiusInVoxels[2]; k <= radiusInVoxels[2]; ++k) {
                int currentIJK[3] = { centerIJK[0] + i, centerIJK[1] + j, centerIJK[2] + k };
                if (currentIJK[0] >= 0 && currentIJK[0] < m_dimensions[0] && currentIJK[1] >= 0 && currentIJK[1] < m_dimensions[1] && currentIJK[2] >= 0 && currentIJK[2] < m_dimensions[2]) {
                    double currentX = m_origin[0] + currentIJK[0] * m_spacing[0];
                    double currentY = m_origin[1] + currentIJK[1] * m_spacing[1];
                    double currentZ = m_origin[2] + currentIJK[2] * m_spacing[2];
                    double distance = std::sqrt(std::pow(currentX - x, 2) + std::pow(currentY - y, 2) + std::pow(currentZ - z, 2));
                    if (distance <= radius) {
                        vtkIdType pointId = currentIJK[2] * m_dimensions[0] * m_dimensions[1] + currentIJK[1] * m_dimensions[0] + currentIJK[0];
                        idTotalList->InsertNextId(pointId);
                    }
                }
            }
        }
    }

}
