#ifndef BREATHCHARTLISTMODEL_H
#define BREATHCHARTLISTMODEL_H

#include <QAbstractListModel>
#include <QPointer>
#include <QVector>

class QXYSeries;
class QAbstractSeries;
class Profile;
class BreathOptions;
class vtkPoints;

class BreathChartListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(Profile* profile WRITE setProfile READ profile NOTIFY profileChanged);

public:
    enum DataRoles{
        First = Qt::UserRole + 1,
        AXISY_MIN_VALUE,
        AXISY_MAX_VALUE,
        AXISX_MAX_VALUE
    };
    Q_ENUM(DataRoles)
public:
    explicit BreathChartListModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    Q_INVOKABLE void setBreathSeries(qint32 index, QAbstractSeries* series);
    void setProfile(Profile *newProfile);
    Profile *profile() const;

signals:
    void profileChanged();

public slots:
    void onCompensatePointsChanged();

private:
    void fillSeriesPoint(QXYSeries* series, QList<QPointF> &pointSeries);
    void fillSeriesPoint(qint32 index, QList<double> &datas);
    void setCompensatePointSeries(qint32 index, vtkPoints *compensatePoints);

private:
    QPointer<Profile> m_profile;
    QPointer<BreathOptions> m_breathOptions;
    struct ChartParam {
        double ymin = 0;
        double ymax = 0;
        double xmax = 0;
    };
    std::array<ChartParam, 3> m_list;
    QHash<qint32, QXYSeries*> m_breathSeries;

};

#endif // BREATHCHARTLISTMODEL_H
