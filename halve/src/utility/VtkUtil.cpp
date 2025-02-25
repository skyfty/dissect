#include "VtkUtil.h"

#include <QColor>
#include <QJsonObject>
#include <QTextStream>
#include <QVector3D>
#include <vtkIdTypeArray.h>
#include <vtkInformation.h>
#include <vtkMath.h>
#include <vtkPoints.h>
#include <vtkSelection.h>
#include <vtkSelectionNode.h>
#include <vtkUnsignedCharArray.h>
#include <vtkVector.h>
namespace vtkutil {
QColor fromVtkColor() {
    return QColor();
}
void qColorToVtkColor(const QColor &color, unsigned char color2[3]) {
    double dcolor[3] = {color.redF(), color.greenF(), color.blueF()};
    for (unsigned int j = 0; j < 3; j++) {
        color2[j] = static_cast<unsigned char>(255.0 * dcolor[j]);
    }
}


void getMinMaxArea(int x1, int y1, int x2, int y2,const int* size, int min[2], int max[2]) {
    min[0] = x1 <= x2 ? x1 : x2;
    if (min[0] < 0) {
        min[0] = 0;
    }
    if (min[0] >= size[0]) {
        min[0] = size[0] - 2;
    }

    min[1] = y1 <= y2 ? y1 : y2;
    if (min[1] < 0) {
        min[1] = 0;
    }
    if (min[1] >= size[1]) {
        min[1] = size[1] - 2;
    }

    max[0] = x2 > x1 ? x2 : x1;
    if (max[0] < 0) {
        max[0] = 0;
    }
    if (max[0] >= size[0]) {
        max[0] = size[0] - 2;
    }

    max[1] = y2 > y1 ? y2 : y1;
    if (max[1] < 0) {
        max[1] = 0;
    }
    if (max[1] >= size[1]) {
        max[1] = size[1] - 2;
    }
}
vtkColor3d qColorToVtkColor3d(const QColor &color) {
    vtkColor3d dcolor;
    dcolor.Set(color.redF(), color.greenF(), color.blueF());
    return dcolor;
}
vtkColor3ub qColorToVtkColor3ub(const QColor &color) {
    vtkColor3ub dcolor;
    dcolor.Set(color.red(), color.green(), color.blue());
    return dcolor;
}

//-----------------------------------------------------------------------------
void vtkShrinkSelection(vtkSelection* sel)
{
    std::map<void*, int> pixelCounts;
    unsigned int numNodes = sel->GetNumberOfNodes();
    void* chosen = nullptr;
    int maxPixels = -1;
    for (unsigned int cc = 0; cc < numNodes; cc++)
    {
        vtkSelectionNode* node = sel->GetNode(cc);
        vtkInformation* properties = node->GetProperties();
        if (properties->Has(vtkSelectionNode::PIXEL_COUNT()) && properties->Has(vtkSelectionNode::SOURCE()))
        {
            int numPixels = properties->Get(vtkSelectionNode::PIXEL_COUNT());
            void* source = properties->Get(vtkSelectionNode::SOURCE());
            pixelCounts[source] += numPixels;
            if (pixelCounts[source] > maxPixels)
            {
                maxPixels = numPixels;
                chosen = source;
            }
        }
    }

    std::vector<vtkSmartPointer<vtkSelectionNode>> chosenNodes;
    if (chosen != nullptr)
    {
        for (unsigned int cc = 0; cc < numNodes; cc++)
        {
            vtkSelectionNode* node = sel->GetNode(cc);
            vtkInformation* properties = node->GetProperties();
            if (properties->Has(vtkSelectionNode::SOURCE()) &&
                properties->Get(vtkSelectionNode::SOURCE()) == chosen)
            {
                chosenNodes.push_back(node);
            }
        }
    }
    sel->RemoveAllNodes();
    for (unsigned int cc = 0; cc < chosenNodes.size(); cc++)
    {
        sel->AddNode(chosenNodes[cc]);
    }
}


vtkSmartPointer<vtkPoints> dividePoints(double p1[3], double p2[3], int n) {
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

    for (int i = 0; i < n; ++i) {
        double t = static_cast<double>(i) / n;
        double x = (1 - t) * p1[0] + t * p2[0];
        double y = (1 - t) * p1[1] + t * p2[1];
        double z = (1 - t) * p1[2] + t * p2[2];
        points->InsertNextPoint(x, y, z);
    }

    return points;
}

QString convertString(const vtkSmartPointer<vtkPoints> &points) {
    QString result;
    if (points != nullptr) {
        QByteArray ba;
        {
            QDataStream ds(ba);
            ds.writeBytes(static_cast<const char *>(points->GetData()->GetVoidPointer(0)),points->GetData()->GetSize());
        }
        QTextStream stream(&result);
        stream<<ba;
    }
    return result;
}
vtkSmartPointer<vtkPoints> pointsFrom(const QByteArray &ba) {
    vtkSmartPointer<vtkPoints> result = vtkSmartPointer<vtkPoints>::New();
    uint len = ba.size();
    if (len > 0) {
        result->Allocate(len);
        QDataStream ds(ba);
        char *buffer = static_cast<char *>(result->GetData()->GetVoidPointer(0));
        ds.readBytes(buffer,len);
    }
    return result;
}
vtkSmartPointer<vtkPoints> pointsFrom(QString in) {
    QByteArray ba;
    {
        QTextStream stream(&in);
        stream>>ba;
    }
    return pointsFrom(ba);
}
vtkVector3d determineEuler(double dtRotMatrix[3][3]) {
    double dRoll = atan2(dtRotMatrix[1][0], dtRotMatrix[0][0]);
    double dCosRoll = cos(dRoll);
    double dSinRoll = sin(dRoll);
    double roll = dRoll;
    double pitch = atan2(-dtRotMatrix[2][0], dCosRoll * dtRotMatrix[0][0] + dSinRoll * dtRotMatrix[1][0]);
    double yaw = atan2(
        dSinRoll * dtRotMatrix[0][2] - dCosRoll * dtRotMatrix[1][2],
        -dSinRoll * dtRotMatrix[0][1] + dCosRoll * dtRotMatrix[1][1]);
    return vtkVector3d(vtkMath::DegreesFromRadians(yaw), vtkMath::DegreesFromRadians(pitch), vtkMath::DegreesFromRadians(roll));

}

vtkSmartPointer<vtkUnsignedCharArray> assignColorsPointData(vtkIdTypeArray* ids, vtkUnsignedCharArray* gridColors) {
    auto idsCnt = ids->GetNumberOfValues();
    vtkSmartPointer<vtkUnsignedCharArray> colors = vtkSmartPointer<vtkUnsignedCharArray>::New();
    colors->SetNumberOfComponents(3);
    colors->SetNumberOfTuples(idsCnt);
    for (int j = 0; j < idsCnt; ++j) {
        unsigned char color[3];
        gridColors->GetTypedTuple(ids->GetValue(j), color);
        colors->SetTypedTuple(j, color);
    }
    return colors;
}

}
