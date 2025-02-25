#ifndef RESEAU_H
#define RESEAU_H

#include <QColor>
#include <QPointer>
#include "utility/VtkDataWarp.h"
#include <vtkColor.h>

class vtkPolyData;
class vtkCellLocator;
class vtkDataSet;
class vtkIdList;
class vtkKdTree;
class vtkImplicitPolyDataDistance;
class vtkVector3d;

class Reseau : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qint64 id READ id CONSTANT FINAL);
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged);
    Q_PROPERTY(bool apparent READ apparent WRITE setApparent NOTIFY apparentChanged FINAL)
    Q_PROPERTY(QColor dyestuff READ dyestuff WRITE setDyestuff NOTIFY dyestuffChanged FINAL)
    Q_PROPERTY(QString path READ path CONSTANT FINAL)
    Q_PROPERTY(bool buildin READ buildin WRITE setBuildin NOTIFY buildinChanged FINAL)
    Q_PROPERTY(qint32 pointNumber READ pointNumber WRITE setPointNumber NOTIFY pointNumberChanged FINAL)

public:
    Reseau(const QString &path,QObject *parent = nullptr);
    ~Reseau() = default;
    void fromJson(const QJsonObject &v);
    void toJson(QJsonObject &json) const;
    void clean();
    void savePolyData(vtkPolyData *polyData = nullptr);
    void savePointIds();
    void savePointIds(vtkSmartPointer<vtkIdList> ids);
    vtkMTimeType GetMTime();

    qint64 id() const {
        return m_id;
    }
    void setId(const qint64 &id) {
        m_id = id;
    }
    void setName(const QString &v);
    QString getName() const;
    bool apparent() const;
    void setApparent(bool newApparent);

    vtkSmartPointer<vtkIdList> pointIds();
    void setPointIds(vtkSmartPointer<vtkIdList> ids);
    qint32 addPointIds(vtkSmartPointer<vtkIdList> ids);
    void removePointIds(vtkSmartPointer<vtkIdList> ids);

    QColor dyestuff() const;
    void setDyestuff(const QColor &newDyestuff);
    vtkColor3d getDyestuff3d() const;

    QString path() const;
    vtkSmartPointer<vtkPolyData> polyData();
    void setPolyData(vtkSmartPointer<vtkPolyData> polyData);

    void getPoint(vtkIdType id, double point[3]);
    void getPoint(vtkIdType id, vtkVector3d &point);

    std::pair<vtkIdType, double> getClosestPointWithinRadius(const double point[3], double radius);
    std::pair<vtkIdType, double> getClosestPointWithinRadius(const vtkVector3d &point, double radius);

    vtkSmartPointer<vtkIdList> getPointsWithinRadius(const double point[3], double radius);
    vtkSmartPointer<vtkIdList> getPointsWithinRadius(const vtkVector3d &point, double radius);

    double checkPoint(const double point[3]) const;
    double checkPoint(const vtkVector3d &pos) const;
    double checkPoint(double x, double y, double z) const;

    bool buildin() const;
    void setBuildin(bool newBuildin);

    qint32 pointNumber() const;
    void setPointNumber(qint32 newPointNumber);

private:
    QString idsFileName() const;
    void loadIdsData();
    QString polyDataFilePath() const;
    void assignPolyData(vtkSmartPointer<vtkPolyData> polyData);
    vtkKdTree* kdTree();
signals:
    void apparentChanged();
    void nameChanged();
    void dyestuffChanged();
    void pointIdsAdded(IdListWarp::Ptr ids);
    void pointIdsRemoved(IdListWarp::Ptr ids);

    void buildinChanged();
    void changed();
    void pointNumberChanged();

private:
    qint64 m_id;
    QString m_name;
    QString m_path;
    qint32 m_pointNumber = 0;
    bool m_apparent = true;
    QColor m_dyestuff = Qt::lightGray;
    bool m_buildin = false;
    vtkTimeStamp m_mTime;
    vtkSmartPointer<vtkIdList> m_meshPointIds;
    vtkSmartPointer<vtkPolyData> m_polyData;
    vtkSmartPointer<vtkKdTree> m_kdTree;
    mutable vtkSmartPointer<vtkImplicitPolyDataDistance> m_implicitPolyDataDistance;
};

#endif // RESEAU_H
