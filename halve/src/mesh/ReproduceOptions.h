#ifndef REPRODUCE_H
#define REPRODUCE_H

#include <QObject>
#include <QVector3D>

class ReproduceOptions : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int iterations READ iterations WRITE setIterations NOTIFY iterationsChanged FINAL)
    Q_PROPERTY(int kernelSize READ kernelSize WRITE setKernelSize NOTIFY kernelSizeChanged FINAL)
    Q_PROPERTY(double holeSize READ holeSize WRITE setHoleSize NOTIFY holeSizeChanged FINAL)
    Q_PROPERTY(QString catheterId READ catheterId WRITE setCatheterId NOTIFY catheterIdChanged FINAL)

public:
    explicit ReproduceOptions(QObject *parent = nullptr);
    ~ReproduceOptions() = default;
    void fromJson(const QJsonObject &v);
    QJsonValue toJson() const;

    int iterations() const;
    void setIterations(int newIterations);

    int kernelSize() const;
    void setKernelSize(int newKernelSize);

    double holeSize() const;
    void setHoleSize(double newHoleSize);

    QString catheterId() const;
    void setCatheterId(const QString &newCatheterId);

signals:
    void changed();
    void iterationsChanged();
    void kernelSizeChanged();
    void holeSizeChanged();
    void centerPointChanged();
    void catheterIdChanged();
private:
    int m_iterations = 20;
    int m_kernelSize = 5;
    double m_holeSize = 500.00;
    QString m_catheterId;
    QVector3D m_centerPoint;

};

#endif // REPRODUCE_H
