#ifndef AZIMUTH_H
#define AZIMUTH_H

#include <QObject>
#include <vtkVector.h>

class CameraInfo;
class vtkVector3d;

class Azimuth : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged FINAL)
    Q_PROPERTY(bool buildin READ buildin WRITE setBuildin NOTIFY buildinChanged FINAL)
    Q_PROPERTY(qint64 id READ id WRITE setId NOTIFY idChanged FINAL)

public:
    explicit Azimuth(QObject *parent = nullptr);
    Azimuth(const QString &name, const CameraInfo &pos, QObject *parent = nullptr);
    ~Azimuth() = default;

    void fromJson(const QJsonObject &v);
    void toJson(QJsonObject &json) const;

    QString name() const;
    void setName(const QString &newName);

    bool enabled() const;
    void setEnabled(bool newEnabled);

    bool buildin() const;
    void setBuildin(bool newBuildin);

    const double* position() const;
    void setPosition(double x, double y, double z);
    void setPosition(const double *pos);
    void setPosition(const vtkVector3d &pos);


    const double* viewUp() const;
    void setViewUp(double x, double y, double z);
    void setViewUp(const double *pos);
    void setViewUp(const vtkVector3d &pos);

    qint64 id() const;
    void setId(qint64 newId);

signals:
    void nameChanged();
    void enabledChanged();
    void buildinChanged();
    void idChanged();

private:
    qint64 m_id;
    QString m_name;
    bool m_enabled = true;
    bool m_buildin = false;

    union {
        vtkVector3d m_pos;
        struct {
            double m_x;
            double m_y;
            double m_z;
        };
    };

    union {
        vtkVector3d m_viewUp;
        struct {
            double m_vx;
            double m_vy;
            double m_vz;
        };
    };
};

#endif // AZIMUTH_H
