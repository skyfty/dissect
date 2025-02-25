#ifndef COMBINEDHOST_H
#define COMBINEDHOST_H

#include <QObject>
#include "TrackData.h"
#include "Detector.h"
#include "rep_combined_merged.h"

class CombinedApi;
class Profile;
class vtkPolyData;
class CombinedDb;
class vtkQuaterniond;

class CombinedHost : public CombinedSimpleSource
{
    Q_OBJECT

public:
    CombinedHost(QObject *parent = nullptr);
    ~CombinedHost();

public:
    void init(const QString &profile, bool keepSave);
    void connect(const QString& host)  override;
    void startTracking(int interval)  override;
    void stopTracking() override;
    void ping() override;

    Detector::List getDetectors()  override {
        return m_enabledTools;
    }
    TrackData::List getCurrentTrackData();

private:
    void initializeAndEnableTools(Detector::List& enabledTools);
    QString getToolInfo(const std::string &toolHandle);
    State connectCombine(const QString& host);
    QString getVersion();
    QString errorToString(int errorCode);
    void timerEvent(QTimerEvent *event) override;
private:
    int m_timer = 0;
    QString m_host;
    CombinedApi *m_capi;
    QPointer<CombinedDb> m_combinedDb;
    Detector::List m_enabledTools;

};


#endif // COMBINEDHOST_H
