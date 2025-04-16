#ifndef FILTEROPTIONITEM_H
#define FILTEROPTIONITEM_H

#include <HalveType.h>
#include <QObject>

class QJsonObject;

class FilterOptionItem : public QObject
{
    Q_OBJECT;
    Q_PROPERTY(double highPass READ highPass WRITE setHighPass NOTIFY highPassChanged FINAL)
    Q_PROPERTY(double lowPass READ lowPass WRITE setLowPass NOTIFY lowPassChanged FINAL)
    Q_PROPERTY(qint32 order READ order WRITE setOrder NOTIFY orderChanged FINAL)
    Q_PROPERTY(bool noise READ noise WRITE setNoise NOTIFY noiseChanged FINAL)
    Q_PROPERTY(bool magnetic READ magnetic WRITE setMagnetic NOTIFY magneticChanged FINAL)
    Q_PROPERTY(bool notch10Hz READ notch10Hz WRITE setNotch10Hz NOTIFY notch10HzChanged FINAL)

public:
    explicit FilterOptionItem(QObject *parent = nullptr);
    void fromJson(const QJsonObject &v);
    QJsonValue toJson();

    double highPass() const;
    void setHighPass(double newHigh);

    double lowPass() const;
    void setLowPass(double newLow);

    qint32 order() const;
    void setOrder(qint32 newOrder);


    bool noise() const;
    void setNoise(bool newNoise);

    bool magnetic() const;
    void setMagnetic(bool newMagnetic);

    bool notch10Hz() const;
    void setNotch10Hz(bool newNotch10Hz);

signals:
    void highPassChanged();
    void lowPassChanged();
    void orderChanged();
    void changed();

    void noiseChanged();
    void magneticChanged();
    void notch10HzChanged();

    void typeChanged();

private:
    double m_highPass = -1;
    double m_lowPass = -1;
    qint32 m_order = 4;
    bool m_noise = true;
    bool m_magnetic = true;
    bool m_notch10Hz = true;

};

#endif // FILTEROPTIONITEM_H
