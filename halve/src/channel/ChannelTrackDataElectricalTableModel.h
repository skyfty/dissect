#ifndef CHANNELTRACKDATAELECTRICALTABLEMODEL_H
#define CHANNELTRACKDATAELECTRICALTABLEMODEL_H

#include <QStandardItem>

class ChannelTrackDataElectricalTableModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit ChannelTrackDataElectricalTableModel(QObject *parent = nullptr);

signals:

    // QAbstractItemModel interface
public:
};


#endif // CHANNELTRACKDATAELECTRICALTABLEMODEL_H
