#include "SerialPortListModel.h"

#include <QSerialPortInfo>

SerialPortListModel::SerialPortListModel(QObject *parent)
    : QAbstractListModel{parent}
{
}

int SerialPortListModel::rowCount(const QModelIndex &parent) const {
    return QSerialPortInfo::availablePorts().count();
}

QVariant SerialPortListModel::data(const QModelIndex &index, int role) const {
    const auto infos = QSerialPortInfo::availablePorts();
    int row = index.row();
    if(row < 0 || row >= infos.count()) {
        return QVariant();
    }
    return infos[row].portName();
}

QHash<int, QByteArray> SerialPortListModel::roleNames() const {
    static const QHash<int, QByteArray> roles {
        { Roles::Name, "name" },
    };
    return roles;
}
