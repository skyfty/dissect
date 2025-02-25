#ifndef CHANNELTRACKDATAMAGNETICTABLEMODEL_H
#define CHANNELTRACKDATAMAGNETICTABLEMODEL_H

#include "QStandardItemModel.h"

class ChannelTrackDataMagneticTableModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit ChannelTrackDataMagneticTableModel(QObject *parent = nullptr);
signals:

    // QAbstractItemModel interface
public:
};

#endif // CHANNELTRACKDATAMAGNETICTABLEMODEL_H
