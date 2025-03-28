#ifndef STAGESCALAR_H
#define STAGESCALAR_H

#include <QObject>
#include <QPointer>
#include <vtkTimeStamp.h>
#include <vtkType.h>

class vtkUnsignedCharArray;
class vtkPoints;
class Profile;
class DissolveDb;
class ScalarDb;
class MappingPointsDb;
class Mapping;
class vtkPolyData;
class vtkUnsignedCharArray;
class MeshColorTransfer;

class StageScalar : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Profile* profile WRITE setProfile READ profile NOTIFY profileChanged);
    Q_PROPERTY(double basis READ basis WRITE setBasis NOTIFY basisChanged FINAL)

public:
    explicit StageScalar(QObject *parent = nullptr);
    void setProfile(Profile* profile);
    Profile *profile() const;
    vtkMTimeType GetMTime();
    bool getVoltageColor(qint64 id, unsigned char color[]) const;
    bool getLatColor(qint64 id, unsigned char color[]) const;
    bool getColor(qint64 id, unsigned char color[]) const;

    double basis() const;
    void setBasis(double newBasis);

private:
    void onScalarDbChanged();

signals:
    void profileChanged();
    void basisChanged();
    void changed();

private:
    QPointer<Profile> m_profile;
    QPointer<MappingPointsDb> m_mappingPointsDb;
    QPointer<ScalarDb> m_scalarDb;
    double m_basis = 1.0;
    vtkTimeStamp m_mTime;
};

#endif // STAGESCALAR_H
