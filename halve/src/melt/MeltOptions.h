#ifndef MELTOPTIONS_H
#define MELTOPTIONS_H

#include <QObject>
#include <QPointer>

class ExposedElectrode;
class MeltSamplingElectrode;
class AppointElectrode;
class CatheterDb;
class ElectrodeSurveyor;

class MeltOptions : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qint64 id READ id WRITE setId NOTIFY idChanged FINAL)
    Q_PROPERTY(qint32 consultDataIndex READ consultDataIndex WRITE setConsultDataIndex NOTIFY consultDataIndexChanged FINAL)
    Q_PROPERTY(qint32 consultListIndex READ consultListIndex NOTIFY consultDataIndexChanged FINAL)
    Q_PROPERTY(qint32 consultListCount READ consultListCount NOTIFY consultListCountChanged FINAL)
    Q_PROPERTY(AppointElectrode *appointElectrode READ appointElectrode CONSTANT FINAL)
    Q_PROPERTY(MeltSamplingElectrode *meltSamplingElectrode READ meltSamplingElectrode CONSTANT FINAL)

public:
    MeltOptions(CatheterDb* catheterDb,QObject *parent = nullptr);
    void fromJson(const QJsonObject &v);
    QJsonObject toJson();

    qint64 id() const;
    void setId(qint64 newId);
    AppointElectrode *appointElectrode();
    MeltSamplingElectrode *meltSamplingElectrode();

    ElectrodeSurveyor *electrodeSurveyor();
    const std::vector<qint32> &consultDataIndexList() const;
    void setConsultDataIndexList(const std::vector<qint32> &newConsultDataIndexList);

    qint32 consultDataIndex() const;
    void setConsultDataIndex(qint32 newConsultDataIndex);
    qint32 nextConsultIndex();
    qint32 previousConsultIndex();
    qint32 scope() const;
    qint32 consultListIndex() const;
    qint32 consultListCount() const;

signals:
    void idChanged();
    void consultDataIndexChanged();
    void consultDataListIndexChanged();
    void meltElectrodeChanged();

    void speedChanged();
    void regulateChanged(int type, double val);

    void consultListCountChanged();


private:
    qint32 indexConsultData(int d) const;
    QString getStoragePath(const QString &path);
private:
    qint64 m_id;
    QPointer<CatheterDb> m_catheterDb;
    QPointer<AppointElectrode> m_appointElectrode;
    QPointer<ElectrodeSurveyor> m_electrodeSurveyor;
    std::vector<qint32> m_consultDataIndexList;
    qint32 m_consultDataIndex = -1;
    QPointer<MeltSamplingElectrode> m_meltSamplingElectrode;

};

#endif // MELTOPTIONS_H
