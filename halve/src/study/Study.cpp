#include <kddockwidgets/Config.h>
#include <kddockwidgets/core/DockRegistry.h>
#include "album/AlbumDb.h"
#include "catheter/CatheterDb.h"
#include "channel/Channel.h"
#include "kddockwidgets/core/DockWidget.h"
#include "kddockwidgets/core/FloatingWindow.h"
#include "kddockwidgets/core/TitleBar.h"
#include <kddockwidgets/qtquick/ViewFactory.h>
#include <kddockwidgets/qtquick/Platform.h>
#include <kddockwidgets/qtquick/views/DockWidget.h>
#include <kddockwidgets/qtquick/views/MainWindow.h>
#include <vtkKdTreePointLocator.h>
#include <QDateTime>
#include <vtkBox.h>
#include <vtkMath.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <combined/Combined.h>
#include <profile/Profile.h>
#include <profile/ProfileSnapshot.h>
#include <catheter/Catheter.h>
#include <mesh/Obscurity.h>
#include <utility/IOWorker.h>
#include "reseau/ReseauDb.h"
#include "utility/ConnectCheck.h"
#include "study/alarm/Alarm.h"
#include "study/alarm/AlarmDb.h"
#include <vtkUnstructuredGrid.h>
#include "Study.h"

using namespace KDDockWidgets;
using namespace std::placeholders;


Study::Study(QQuickItem* parent)
    : QQuickItem(parent) {
    m_savePolyDataTimer = new QTimer(this);
    connect(m_savePolyDataTimer, &QTimer::timeout, this, &Study::onSavePolyDataTimerEvent);
}

Study::~Study() {
    qDeleteAll(DockRegistry::self()->floatingWindows(true));
    qDeleteAll(DockRegistry::self()->dockwidgets());
    m_profile->save();
    m_profile->decreasecRef();
}

void Study::setProfile(Profile* profile) {
    if (profile == m_profile) {
        return;
    }
    m_profile = profile;
    QObject::connect(m_profile, &Profile::stateChanged, this, &Study::onProfileStateChanged);

    m_reseauDb = m_profile->reseauDb();
    m_catheterDb = profile->catheterDb();
    QObject::connect(m_catheterDb, &CatheterDb::deleted, this, &Study::onCatheterDeleted);
    QObject::connect(m_catheterDb, &CatheterDb::alined, this, &Study::onAlined);
    QObject::connect(m_catheterDb, &CatheterDb::portChanged, this, &Study::onCatheterPortChanged);

    m_profile->increaseRef();
    emit profileChanged();
}
Profile* Study::profile() const {
    return m_profile;
}


void Study::setCombined(Combined* combined) {
    Q_ASSERT(combined != nullptr);
    Q_ASSERT(m_profile != nullptr);
    m_combined = combined;
    QObject::connect(m_combined, &Combined::centerPointChanged, this, &Study::onCenterPointChanged);
    QObject::connect(m_combined, &Combined::environmentFlagsChanged, this, &Study::onCombinedEnvironmentFlgasChanged);

    vtkVector3d centerPos = m_profile->getCenterPoint();
    if (centerPos.GetX() != -1) {
        m_combined->resetCenterPoint(centerPos);
    }
}

void Study::onChannelModeChanged() {
    Q_ASSERT(m_combined != nullptr);
    m_combined->resetCenterPoint();
    m_combined->checkEnvironment();
    m_combined->checkReproduceCatheter();
}

static struct {
    Halve::AlarmName name;
    Halve::AlarmType type;
} CombinedAlertNames[] = {
    {Halve::AN_THE_CS_CATHETER_CARTRIDGE_IS_NOT_CONFIGURED, Halve::AT_ERROR},
    {Halve::AN_ELECTRIC_REFERENCE_POSITION_IS_TOO_OFFSET, Halve::AT_ERROR},
    {Halve::AN_PRETHORACIC_REFERENCE_CATHETER_CONFIGURATION_ERROR, Halve::AT_WARNING},
    {Halve::AN_BACK_REFERENCE_CATHETER_CONFIGURATION_ERROR, Halve::AT_ERROR},
    {Halve::AN_REPRODUCE_CATHETER_CONFIGURATION_ERROR, Halve::AT_ERROR},
    {Halve::AN_REPRODUCE_CATHETER_NOT_RECEICE_DATA_ERROR, Halve::AT_ERROR},
    {Halve::AN_REPRODUCE_CATHETER_MISSED_ERROR, Halve::AT_WARNING},
    {Halve::AN_CATHETER_PORT_REUSED_ERROR, Halve::AT_WARNING},
    {Halve::AN_REPRODUCE_CATHETER_NOT_IN_CASKET_ERROR, Halve::AT_ERROR},
    {Halve::AN_REPRODUCE_CATHETER_CONSULT_TARGET_CONFIG_ERROR, Halve::AT_ERROR}
};

void Study::onAlarmDeleted(const Alarm *alarm) {
    quint64 flags = m_combined->environmentFlags();
    auto iter = std::find_if(std::begin(CombinedAlertNames), std::end(CombinedAlertNames),
                 [alarm](const auto &item){return item.name == alarm->name();});
    if (iter != std::end(CombinedAlertNames)) {
        flags &= ~(alarm->name());
    }
    m_combined->setEnvironmentFlags(flags);
}

void Study::onCombinedEnvironmentFlgasChanged() {
    quint64 flags = m_combined->environmentFlags();
    for(auto alert:CombinedAlertNames) {
        if (flags & alert.name) {
            m_profile->alarmDb()->add(alert.name, alert.type);
        } else {
            m_profile->alarmDb()->remove(alert.name);
        }
    }
}

void Study::onReproduceCatheterChanged() {
    Q_ASSERT(m_combined != nullptr);
    m_combined->checkReproduceCatheter();
}

void Study::onAlined() {
    Q_ASSERT(m_combined != nullptr);
    m_combined->checkEnvironment();
    m_combined->checkReproduceCatheter();
    emit m_profile->catheterAlinedChanged();
}

void Study::onCatheterDeleted(Catheter *catheter) {
    Q_ASSERT(catheter != nullptr);
    Q_ASSERT(m_combined != nullptr);
    m_combined->checkCatheterPort(catheter);
    m_combined->checkReproduceCatheter();
}

void Study::onCatheterPortChanged(Catheter *inCatheter) {
    Q_ASSERT(inCatheter != nullptr);
    Q_ASSERT(m_combined != nullptr);
    m_combined->checkCatheterPort(inCatheter);
}

Combined *Study::combined() const {
    return m_combined;
}

void Study::setObscurity(Obscurity *newObscurity) {
    m_obscurity = newObscurity;
    QObject::connect(m_obscurity, &Obscurity::polyDataChanged, this, &Study::onPolyDataChanged);
}

void Study::setChannel(Channel *newChannel) {
    if (newChannel == m_channel) {
        return;
    }
    m_channel = newChannel;
    QObject::connect(m_channel, &Channel::modeChanged, this, &Study::onChannelModeChanged);
    emit channelChanged();
}

Channel* Study::channel() const { return m_channel;}

void Study::onPolyDataChanged(PolyDataWarp::Ptr result) {
    Reseau* currentReseau = m_profile->getCurrentReseau();
    if (currentReseau == nullptr || result == nullptr) {
        return;
    }
    currentReseau->setPolyData(result->Data);
    m_savePolyDataTimer->start(std::chrono::milliseconds(500));
}

void Study::onSavePolyDataTimerEvent() {
    m_savePolyDataTimer->stop();
    Reseau* currentReseau = m_profile->getCurrentReseau();
    if (currentReseau == nullptr) {
        return;
    }
    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    polyData->ShallowCopy(currentReseau->polyData());
    IOWorker::run([currentReseau, polyData] {
        currentReseau->savePolyData(polyData);
    });
    m_reseauDb->save();
}

void Study::onCenterPointChanged() {
    Q_ASSERT(m_catheterDb != nullptr);
    m_catheterDb->clearLastPoint();
    m_profile->setCenterPoint(m_combined->getCenterPoint());
    m_profile->save();
}

void Study::onProfileStateChanged() {
    Q_ASSERT(m_catheterDb != nullptr);
    m_catheterDb->clearLastPoint();
}

