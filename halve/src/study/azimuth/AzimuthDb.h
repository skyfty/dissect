#ifndef AZIMUTHDB_H
#define AZIMUTHDB_H

#include "study/azimuth/AzimuthBuildinDb.h"

class AzimuthDb : public AzimuthBuildinDb
{
    Q_OBJECT
public:
    AzimuthDb(const QString& path,QObject *parent = nullptr);

    bool save();
    void load();
    using AzimuthBuildinDb::load;
    void backup(const QString &path);

    void remove(Azimuth* azimuth);
    void remove(int row);
    void add(const QString &name, const CameraInfo &pos);

private:
    void setAzimuthPosition(Azimuth* azimuth, const vtkVector3d &pos,const vtkVector3d &viewUp);
    QString getStoragePath(const QString &path) const;

signals:
    void added(const Azimuth *melt);
    void deleted(const Azimuth* melt);
    void changed(const Azimuth* melt);

private:
    QString m_path;
};

#endif // AZIMUTHDB_H
