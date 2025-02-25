#include "ChannelTrackDataMagneticTableModel.h"
#include "HalveType.h"

ChannelTrackDataMagneticTableModel::ChannelTrackDataMagneticTableModel(QObject *parent)
    : QStandardItemModel{parent}
{
    QStringList electricalHeader;
    electricalHeader << "status" << "x" << "y"<< "z" << "yaw"<<"pitch"<<"roll";
    setHorizontalHeaderLabels(electricalHeader);

    QString initValue = "0.00";
    for(int i = 0; i < MagnetismPortAmount; ++i) {
        setItem(i, 0, new QStandardItem("0"));
        for(int h = 1; h < 7; ++h) {
            setItem(i, h, new QStandardItem(initValue));
        }
    }

}

