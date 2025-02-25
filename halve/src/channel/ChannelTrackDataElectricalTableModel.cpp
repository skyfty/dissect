#include "ChannelTrackDataElectricalTableModel.h"
#include "HalveType.h"

ChannelTrackDataElectricalTableModel::ChannelTrackDataElectricalTableModel(QObject *parent)
    : QStandardItemModel{parent}
{
    QStringList electricalHeader;
    electricalHeader << "x" << "y"<< "z";
    setHorizontalHeaderLabels(electricalHeader);

    QString initValue = "0.00";
    for(int i = 0; i < ElectricalPortAmount; ++i) {
        for(int h = 0; h < 3; ++h) {
            setItem(i, h, new QStandardItem(initValue));
        }
    }
}
