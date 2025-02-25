#include "BreathChartListModel.h"
#include "halitus/BreathOptions.h"
#include "profile/Profile.h"

#include <QXYSeries>
#include <QChart>
#include <QGraphicsLayout>
#include <vtkPoints.h>

BreathChartListModel::BreathChartListModel(QObject *parent)
    : QAbstractListModel{parent}
{}

void BreathChartListModel::setProfile(Profile *newProfile) {
    if (m_profile == newProfile) {
        return;
    }
    m_profile = newProfile;
    m_breathOptions =  m_profile->breathOptions();
    QObject::connect(m_breathOptions, &BreathOptions::compensatePointsChanged, this, &BreathChartListModel::onCompensatePointsChanged);

}

Profile *BreathChartListModel::profile() const {
    return m_profile;
}
int BreathChartListModel::rowCount(const QModelIndex&) const {
    return m_list.size();
}

Qt::ItemFlags BreathChartListModel::flags(const QModelIndex& index) const {
    return Qt::ItemIsSelectable;
}

void BreathChartListModel::setBreathSeries(qint32 index, QAbstractSeries* series) {
    if (series != nullptr) {
        QXYSeries *xySeries = static_cast<QXYSeries*>(series);;
        m_breathSeries[index] = xySeries;
        xySeries->chart()->layout()->setContentsMargins(0, 0, 0, 0);
        setCompensatePointSeries(index, m_breathOptions->compensatePoints());
    }
}

QVariant BreathChartListModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }
    const ChartParam &cp = m_list[index.row()];
    switch (role) {
    case AXISY_MIN_VALUE:
        return cp.ymin;
    case AXISY_MAX_VALUE:
        return cp.ymax;
    case AXISX_MAX_VALUE:
        return cp.xmax;
    }
    return QVariant();
}

QHash<int, QByteArray> BreathChartListModel::roleNames() const {
    return {
            {AXISY_MIN_VALUE, "axisyMinValue"},
            {AXISY_MAX_VALUE, "axisyMaxValue"},
            {AXISX_MAX_VALUE, "axisxMaxValue"}
            };
}

void BreathChartListModel::setCompensatePointSeries(qint32 index, vtkPoints *compensatePoints) {
    QList<double> datas;
    for(int i = 0; i < compensatePoints->GetNumberOfPoints(); ++i) {
        vtkVector3d point;
        compensatePoints->GetPoint(i, point.GetData());
        datas.append(point[index]);
    }
    if (!datas.isEmpty()) {
        auto minmaxval = std::minmax_element(datas.begin(), datas.end());
        m_list[index].ymin = *(minmaxval.first);
        m_list[index].ymax = *(minmaxval.second);
    }
    m_list[index].xmax = datas.size();
    fillSeriesPoint(index, datas);
    emit dataChanged(createIndex(index, 0), createIndex(index, 0), QVector<int>() << AXISY_MIN_VALUE << AXISY_MAX_VALUE << AXISX_MAX_VALUE);
}

void BreathChartListModel::onCompensatePointsChanged() {
    beginResetModel();
    vtkSmartPointer<vtkPoints> compensatePoints = m_breathOptions->compensatePoints();
    for (auto i = m_breathSeries.cbegin(), end = m_breathSeries.cend(); i != end; ++i) {
        setCompensatePointSeries(i.key(), compensatePoints);
    }
    endResetModel();
}

void BreathChartListModel::fillSeriesPoint(qint32 index, QList<double> &datas) {
    if (!m_breathSeries.contains(index)) {
        return;
    }
    QXYSeries* xySeries = m_breathSeries[index];
    QList<QPointF> pointSeries = xySeries->points();
    pointSeries.resize(m_list[index].xmax);
    for(qsizetype j = 0; j < datas.size(); ++j) {
        pointSeries[j] = QPointF(0, datas[j]);
    }
    fillSeriesPoint(xySeries, pointSeries);
}

void BreathChartListModel::fillSeriesPoint(QXYSeries *series, QList<QPointF> &pointSeries)
{
    for (qsizetype j = 0; j < pointSeries.size(); ++j) {
        pointSeries[j].setX(j);
    }
    series->replace(pointSeries);
}
