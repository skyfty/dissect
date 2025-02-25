#ifndef EcgOptions_H
#define EcgOptions_H

#include <QObject>
#include <QPointer>

class EcgOptions : public QObject
{
    Q_OBJECT;

    Q_PROPERTY(int amplitude READ amplitude WRITE setAmplitude NOTIFY amplitudeChanged FINAL)
    Q_PROPERTY(bool showFiltration READ showFiltration WRITE setShowFiltration NOTIFY showFiltrationChanged FINAL)
    Q_PROPERTY(bool enableFilter READ enableFilter WRITE setEnableFilter NOTIFY enableFilterChanged FINAL)
    Q_PROPERTY(double low READ low WRITE setLow NOTIFY lowChanged FINAL)
    Q_PROPERTY(double high READ high WRITE setHigh NOTIFY highChanged FINAL)

public:
    explicit EcgOptions(QObject *parent = nullptr);
    void fromJson(const QJsonObject &v);
    QJsonValue toJson();

    int amplitude() const;
    void setAmplitude(int newAmplitude);
    bool showFiltration() const;
    void setShowFiltration(bool newShowFiltration);
    bool enableFilter() const;
    void setEnableFilter(bool newEnableFilter);
    double low() const;
    void setLow(double newLow);
    double high() const;
    void setHigh(double newHigh);

signals:
    void highChanged();
    void amplitudeChanged();
    void lowChanged();
    void enableFilterChanged();
    void showFiltrationChanged();

private:
    int m_amplitude = 10;
    bool m_showFiltration = false;
    bool m_enableFilter = false;
    double m_low = 0.0;
    double m_high = 0.0;

};

#endif // EcgOptions_H
