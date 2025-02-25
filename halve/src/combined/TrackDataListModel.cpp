#include "TrackDataListModel.h"

#include "utility/ConnectCheck.h"
#include "Combined.h"

#include <vtkMath.h>
#include <vtkMatrix3x3.h>
#include <vtkMatrix4x4.h>
#include <vtkQuaternion.h>
#include <vtkTransform.h>
#include "utility/VtkUtil.h"

using namespace std::placeholders;


TrackDataListModel::TrackDataListModel(QObject *parent)
    : QAbstractListModel{parent}
{
}

void TrackDataListModel::setCombined(Combined* combined) {
    Q_ASSERT(combined != nullptr);
    m_combined = combined;
    QObject::connect(m_combined, &Combined::stateChanged, this, &TrackDataListModel::onStateChanged);
}

void TrackDataListModel::reset()
{
    beginResetModel(); endResetModel();
}

void TrackDataListModel::onTrackDataChanged() {
    m_currentTrackDataList.clear();
    if (m_combined != nullptr) {
        for(const auto &trackData:m_combined->getCurrentTrackDataList()) {
            if (trackData.getStatus() == Halve::TrackStatus_Valid) {
                m_currentTrackDataList.append(trackData);
            }
        }
    }
}
void TrackDataListModel::onStateChanged() {
    beginResetModel();
    m_currentTrackDataList.clear();
    endResetModel();
}

int TrackDataListModel::rowCount(const QModelIndex&) const {
    return m_currentTrackDataList.size();
}

Qt::ItemFlags TrackDataListModel::flags(const QModelIndex& index) const
{
    return Qt::ItemIsSelectable;
}


QString TrackDataListModel::statusToString(uint8_t errorCode)
{
    switch (errorCode)
    {
    case Halve::TrackStatus_Disabled:
        return "Disabled";
    case Halve::TrackStatus_Invalid:
        return "Invalid";
    case Halve::TrackStatus_Missing:
        return "Missing";
    case Halve::TrackStatus_Valid:
        return "Valid";
    default:
        return "UnrecognizedEnumValue";
    }
}
QVariant TrackDataListModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() <0 || index.row() >=m_currentTrackDataList.size() ) {
        return QVariant();
    }
    TrackData trackData = m_currentTrackDataList[index.row()];

    switch (role) {
    case Qt::DisplayRole:
    case NAME:
        return trackData.port();
    case STATUS:
        return trackData.getStatus();
    case STATUS_TIP: {
        if (trackData.isMissing()) {
            return "Missing";
        }else {
            return statusToString(trackData.getStatus());
        }
    }
    case POSITION: {
        double pos[3]{};
        if (!trackData.isMissing()) {
            trackData.getPosition(pos);
        }
        return QVector3D(pos[0], pos[1], pos[2]);
    }
    case EULER_ANGLES: {
        vtkQuaterniond q;
        if (!trackData.isMissing()) {
            trackData.getQuaternion(q);
        }
         double matrix[3][3];
         q.ToMatrix3x3(matrix);
         vtkVector3d euler = vtkutil::determineEuler(matrix);
        return QVector3D(euler[0],euler[1],euler[2]);
    }
    case QUATERNION: {
        vtkQuaterniond q;
        if (!trackData.isMissing()) {
            trackData.getQuaternion(q);
        }
        return QVector4D(q[1],q[2],q[3],q[0]);
    }
    }

    return QVariant();
}


QHash<int, QByteArray> TrackDataListModel::roleNames() const {
    return {
        {NAME, "name"},
        {STATUS, "status"},
        {POSITION, "position"},
        {STATUS_TIP, "statusTip"},
        {QUATERNION, "quaternion"},
        {EULER_ANGLES, "eulerAngles"}
    };
}
