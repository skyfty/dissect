#include "CombinedHost.h"
#include "PortHandleInfo.h"
#include "combined/CombinedDb.h"
#include "combined/TrackData.h"
#include "QDateTime"
#include <vtkLineSource.h>
#include <vtkQuaternion.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkVector.h>
#include <vtkBox.h>
#include <Windows.h>

CombinedHost::CombinedHost(QObject *parent)
    :CombinedSimpleSource(parent)
{
    m_capi = new CombinedApi();
    m_combinedDb = new CombinedDb(this);
}

CombinedHost::~CombinedHost() {
    delete m_capi;
    delete m_combinedDb;
}

void CombinedHost::init(const QString &profile, bool keepSave)
{
    setKeepSave(keepSave);
    m_combinedDb->setProfilePath(profile);
    setState(State_Ready);
}

void CombinedHost::timerEvent(QTimerEvent *event) {
    if (event->timerId() == m_timer) {
        TrackData::List currentTrackDataList = getCurrentTrackData();
        if (!currentTrackDataList.isEmpty()) {
            if(m_combinedDb != nullptr) {
                m_combinedDb->add(currentTrackDataList);
            }
            emit trackDataChanged(currentTrackDataList);
        }
    }
}


QString CombinedHost::getToolInfo(const std::string& toolHandle)
{
    // Get the port handle info from PHINF
    PortHandleInfo info = m_capi->portHandleInfo(toolHandle);

    // Return the ID and SerialNumber the desired string format
    std::string outputString = info.getToolId();
    outputString.append(" s/n:").append(info.getSerialNumber());
    return QString::fromStdString(outputString);
}

void CombinedHost::initializeAndEnableTools(Detector::List& enabledTools)
{
    // Initialize and enable tools
    std::vector<PortHandleInfo> portHandles = m_capi->portHandleSearchRequest(PortHandleSearchRequestOption::NotInit);
    for (qsizetype i = 0; i < portHandles.size(); i++) {
        m_capi->portHandleInitialize(portHandles[i].getPortHandle());
        m_capi->portHandleEnable(portHandles[i].getPortHandle());
    }

    // Print all enabled tools
    portHandles = m_capi->portHandleSearchRequest(PortHandleSearchRequestOption::Enabled);
    // Lookup and store the serial number for each enabled tool
    for (qsizetype i = 0; i < portHandles.size(); i++) {
        QSharedPointer<Detector> detector(new Detector());
        detector->m_toolHandle = (uint16_t)m_capi->stringToInt(portHandles[i].getPortHandle());
        detector->m_toolInfo = getToolInfo(portHandles[i].getPortHandle());
        enabledTools.push_back(detector);
    }
}

CombinedHost::State CombinedHost::connectCombine(const QString& host) {
    m_host = host;
    int errorCode = m_capi->connect(host.toStdString());
    if (errorCode != 0) {
        return State_ConnectFailed;
    }
    errorCode = m_capi->initialize();
    if (errorCode < 0) {
        return State_ConnectFailed;
    }
    initializeAndEnableTools(m_enabledTools);
    setConnected(true);
    return State_Connected;
}

void CombinedHost::connect(const QString& host) {
    m_host = host;
    setState(State_Connecting);
    int errorCode = m_capi->connect(host.toStdString());
    if (errorCode != 0) {
        return setState(State_ConnectFailed);
    }
    errorCode = m_capi->initialize();
    if (errorCode < 0) {
        return setState(State_ConnectFailed);
    }
    initializeAndEnableTools(m_enabledTools);
    setConnected(true);
    setState(State_Connected);
}

void CombinedHost::stopTracking() {
    if (!connected()) {
        return;
    }
    auto errorCode = m_capi->stopTracking();
    if (errorCode < 0) {
        setState(State_TrackError);
    } else {
        killTimer(m_timer);
        m_timer = 0;
        setState(State_Stop);
    }
}

void CombinedHost::ping() {
    if (state() != State_Connecting) {
        emit pinged(m_capi->ping());
    }
}

void CombinedHost::startTracking(int interval) {
    if (!connected()) {
        return;
    }
    auto errorCode = m_capi->startTracking();
    if (errorCode < 0) {
        setState(State_TrackError);
    } else {
        m_timer = startTimer(interval);
        setState(State_Tracking);
    }
}

QString CombinedHost::errorToString(int errorCode) {
    return QString::fromStdString(m_capi->errorToString(errorCode));
}

QString CombinedHost::getVersion() {
    Q_ASSERT(m_capi != nullptr);
    return QString::fromStdString(m_capi->getVersion());
}

TrackData::List CombinedHost::getCurrentTrackData() {
    TrackData::List trackDataList;
    // if (connected()) {
    //     std::vector<ToolData> toolDatas = m_capi->getTrackingDataBX();
    //     for (qsizetype i = 0; i < toolDatas.size(); i++) {
    //         TrackData trackData(toolDatas[i].transform);
    //         trackDataList.append(trackData);
    //     }
    // }
    return trackDataList;
}
