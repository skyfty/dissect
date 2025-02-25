#ifndef AZIMUTHBUILDINDB_H
#define AZIMUTHBUILDINDB_H

#include <QObject>

class Azimuth;
class CameraInfo;
class vtkVector3d;

class AzimuthBuildinDb : public QObject
{
    Q_OBJECT
public:
    explicit AzimuthBuildinDb(QObject *parent = nullptr);

    void initDefault();
    qsizetype size() const;
    Azimuth* getData(qint64 id) const;
    Azimuth* getData(int row) const;
    Azimuth* getData(double x, double y, double z) const;
    Azimuth* getData(const QVector3D &pos) const;
    Azimuth* getData(const double pos[3]) const;
    Azimuth* getData(const QString &name) const;
    Azimuth* getData(const vtkVector3d &pos) const;

signals:
    void changed(const Azimuth* melt);

protected:
    void load(const QByteArray &catheterJson);
    Azimuth *load(const QJsonObject &json);
    void connectSignals(Azimuth *azimuth);

protected:
    QList<Azimuth*> m_azimuths;
    Azimuth* m_buildinAzimuths[6]{};
};

#endif // AZIMUTHBUILDINDB_H
