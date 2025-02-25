#ifndef TRACKDATALISTMODEL_H
#define TRACKDATALISTMODEL_H

#include <QAbstractItemModel>
#include <QPointer>
#include <QVector3D>
#include "combined/CatheterTrack.h"
#include "TrackData.h"

class Combined;


class TrackDataListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(Combined* combined WRITE setCombined FINAL);

public:
    enum DataRoles{
        First = Qt::UserRole + 1,
        NAME,
        STATUS,
        STATUS_TIP,
        POSITION,
        QUATERNION,
        EULER_ANGLES
    };
    Q_ENUM(DataRoles)
public:
    explicit TrackDataListModel(QObject *parent = nullptr);
    ~TrackDataListModel() = default;
    void setCombined(Combined *combined);
    Q_INVOKABLE void reset();

    int rowCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

private slots:
    void onTrackDataChanged();
    void onStateChanged();

private:
    static QString statusToString(uint8_t errorCode);

private:
    QPointer<Combined> m_combined;
    TrackData::List m_currentTrackDataList;

};

#endif // TRACKDATALISTMODEL_H
