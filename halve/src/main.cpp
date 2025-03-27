#include <QApplication>
#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QQuickVTKItem.h>
#include <QMessageBox>
#include <QDir>
#include <vtkAutoInit.h> 
#include <QRemoteObjectHost>
#include <QQmlContext>
#include <QStandardPaths>
#include <QQuickStyle>
#include <QCommandLineParser>
#include <kddockwidgets/Config.h>
#include <kddockwidgets/core/DockRegistry.h>
#include <kddockwidgets/qtquick/ViewFactory.h>
#include <kddockwidgets/qtquick/Platform.h>
#include <kddockwidgets/qtquick/views/DockWidget.h>
#include <kddockwidgets/qtquick/views/MainWindow.h>
#include <utility/ModelCache.h>
#include <utility/SimpleChecker.h>
#include "channel/ChannelHost.h"
#include "channel/ChannelWatchWindow.h"
#include "profile/StorageChekcerHost.h"
#include "user/User.h"
#include <combined/Combined.h>
#include <combined/CombinedHost.h>
#include "Translator.h"
#include "profile/ProfileManager.h"
#include "utility/IOWorker.h"
#include "utility/ModelCache.h"
#include "utility/StartoverState.h"
#include <vtksqlite/sqlite3.h>
#include <ScreenGrab.h>
#include <client/windows/handler/exception_handler.h>
#include "utility/exporter/ExporterHost.h"

extern void qmlRegisterTypes();

class CustomViewFactory : public KDDockWidgets::QtQuick::ViewFactory
{
public:
    QUrl titleBarFilename() const override
    {
        return QUrl("qrc:/qml/TitleBar.qml");
    }
};

void initKDDockWidgets(QQmlApplicationEngine *engine) {
    KDDockWidgets::initFrontend(KDDockWidgets::FrontendType::QtQuick);
    auto &config = KDDockWidgets::Config::self();
    auto flags = config.flags();
    flags |= KDDockWidgets::Config::Flag_CloseOnlyCurrentTab;
    flags |= KDDockWidgets::Config::Flag_ShowButtonsOnTabBarIfTitleBarHidden;
    flags |= KDDockWidgets::Config::Flag_HideTitleBarWhenTabsVisible;
    config.setFlags(flags);
    config.setViewFactory(new CustomViewFactory());
    KDDockWidgets::QtQuick::Platform::instance()->setQmlEngine(engine);
}

void watchParentProcessFinished(qint64 pid) {
    new std::thread([pid](){
        HANDLE processHandle = OpenProcess( SYNCHRONIZE  , false, pid);
        if (processHandle != nullptr) {
            WaitForSingleObject(processHandle, -1);
            ExitProcess(0);
        }
    });
}
int runStorageChecker(QApplication &app, const QString &path, qint32 threshold) {
    SimpleChecker simpleChecker("storagechecker.touchmagic.cn");
    if (simpleChecker.running()) {
        return 0;
    }

    IOWorker *ioWorker = new IOWorker();
    ioWorker->init();
    QRemoteObjectHost srcNode(QUrl(QStringLiteral("local:storagechecker")));
    StorageChekcerHost storageChekcerHost(path, threshold); // create simple switch
    srcNode.enableRemoting(&storageChekcerHost); // enable remoting
    auto result = app.exec();
    ioWorker->exit();
    return result;
}
struct ChannelHostOption {
    quint16 keepSave;
    Halve::ChannelMode mode;

};

int runChannel(QApplication& app, const QString& profile,ChannelHostOption &options) {
    SimpleChecker simpleChecker("channel.touchmagic.cn");
    if (simpleChecker.running()) {
        return 0;
    }
    IOWorker *ioWorker = new IOWorker();
    ioWorker->init();
    QRemoteObjectHost srcNode(QUrl(QStringLiteral("local:channel")));
    ChannelHost channelHost; // create simple switch
    srcNode.enableRemoting(&channelHost); // enable remoting
    channelHost.init(profile, options.mode);
    channelHost.setKeepSave(options.keepSave);
    channelHost.setState(ChannelHost::State_Ready);
    auto result = app.exec();
    ioWorker->exit();
    return result;
}

int runChannelWatch(QApplication& app) {
    SimpleChecker simpleChecker("channel-watch.touchmagic.cn");
    if (simpleChecker.running()) {
        simpleChecker.activateWindow(ChannelWatchWindow::windowTitle);
        return 0;
    }
    IOWorker *ioWorker = new IOWorker();
    ioWorker->init();
    ChannelWatchWindow channelWatch;
    channelWatch.show();
    auto result = app.exec();
    ioWorker->exit();
    return result;
}
QString prepareUserDataPath() {
    QDir defaultUserPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    {
        QSettings setting;
        QString deployDefaultUserPath = setting.value("deploy/defaultUserPath").toString();
        if (!deployDefaultUserPath.isEmpty() && QDir(deployDefaultUserPath).isReadable()) {
            defaultUserPath = deployDefaultUserPath;
        }
    }
    if (!defaultUserPath.exists()) defaultUserPath.mkpath(".");
    return defaultUserPath.filePath("users");
}

void runScreenGrab(const QString &filePath, const QString &waitEventName) {
    if (filePath.isEmpty() || waitEventName.isEmpty()) {
        return;
    }
    ScreenGrab::waitForFinish(filePath.toStdString().c_str(),waitEventName.toStdString().c_str());
}

QQmlApplicationEngine *createQmlEngine(QApplication &app) {
    QQmlApplicationEngine *engine = new QQmlApplicationEngine(&app);
    engine->addImportPath(":/styles");
    QObject::connect(engine, &QQmlApplicationEngine::objectCreationFailed, &app, QApplication::quit,Qt::QueuedConnection);
    return engine;
}

void registerServerInstance(QQmlApplicationEngine *engine) {
    QSettings setting;
    ModelCache *modelCache = new ModelCache(engine);
    qmlRegisterSingletonInstance("Halve", 1, 0, "ModelCache", modelCache);
    Translator *translator = new Translator(setting.value("interface/language", "en").toString(),engine);
    qmlRegisterSingletonInstance("Halve", 1, 0, "Translator", translator);
}

void initBreakpad() {
    QDir appdataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (!appdataPath.exists()) appdataPath.mkdir(".");
    QString dumpsPath = appdataPath.filePath("dumps");
    appdataPath.mkdir(dumpsPath);
    new google_breakpad::ExceptionHandler(dumpsPath.toStdWString().c_str(),nullptr,nullptr,nullptr,google_breakpad::ExceptionHandler::HANDLER_ALL);
}

int runPerform(QApplication &app, ProfileManager* profileManager, const QString &userPath, const QString &profileId) {
    SimpleChecker simpleChecker("perform.touchmagic.cn");
    if (simpleChecker.running()) {
        return 0;
    }
    IOWorker *ioWorker = new IOWorker();
    ioWorker->init();
    QScopedPointer<QQmlApplicationEngine> engine(createQmlEngine(app));
    Q_ASSERT(profileManager != nullptr);
    User* user = profileManager->activeUser(userPath);
    if (user == nullptr) {
        engine->load((QUrl("qrc:/qml/ErrorPage.qml")));
    } else {
        initKDDockWidgets(engine.get());
        registerServerInstance(engine.get());

        QVariantMap initialProperties = {{ "userId", QVariant::fromValue(user->getId())}};
        if (!profileId.isEmpty()) {
            initialProperties["profileId"] = QVariant::fromValue(profileId);
        }
        engine->setInitialProperties(initialProperties);
        engine->load((QUrl("qrc:/qml/PerformPage.qml")));
    }
    auto result = app.exec();
    ioWorker->exit();
    return result;
}

int runStraining(QApplication &app, ProfileManager* profileManager) {
    SimpleChecker simpleChecker("straining.touchmagic.cn");
    if (simpleChecker.running()) {
        return 0;
    }
    Q_ASSERT(profileManager != nullptr);
    profileManager->loadUsers();
    QQmlApplicationEngine *engine = createQmlEngine(app);
    registerServerInstance(engine);
    engine->load((QUrl("qrc:/qml/StrainingPage.qml")));
    return app.exec();
}
int runKiller(QApplication& app, const QString& kid) {
    StartoverState::waitAndKill(kid.toInt());
    return 0;
}

int runExporter(QApplication& app, const QString &host) {
    IOWorker *ioWorker = new IOWorker();
    ioWorker->init();
    QRemoteObjectHost srcNode(host);
    ExporterHost exporterHost;
    srcNode.enableRemoting(&exporterHost);
    exporterHost.init();
    auto result = app.exec();
    ioWorker->exit();
    return result;
}

void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QFile logFile("log.txt");
    if (!logFile.open(QIODevice::WriteOnly | QIODevice::Append))
        return;

    QTextStream out(&logFile);
    out << msg << Qt::endl;
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setOrganizationName("touchmagic");
    app.setOrganizationDomain("touchmagic.cn");
    app.setApplicationName("halve");
    QSettings::setDefaultFormat(QSettings::IniFormat);

#ifdef NDEBUG
    initBreakpad();
#endif

    QCommandLineParser parser;
    parser.setApplicationDescription("The Halve");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("user", QCoreApplication::translate("main", "User path"));
    QCommandLineOption pidOption("pid", "parent process id", "pid");
    parser.addOption(pidOption);
    QCommandLineOption profileOption("profile", "the work file director", "id");
    parser.addOption(profileOption);
    QCommandLineOption processTypeOption("process-type", "the work file director", "path");
    parser.addOption(processTypeOption);
    QCommandLineOption outFileOption("out-file", "the work file director", "path");
    parser.addOption(outFileOption);
    QCommandLineOption waitEventOption("wait-event", "the work file director", "id");
    parser.addOption(waitEventOption);
    QCommandLineOption hostOption("host", "the work file director", "host");
    parser.addOption(hostOption);
    QCommandLineOption kidOption("kid", "want to kill process id", "kid");
    parser.addOption(kidOption);
    QCommandLineOption keepSaveOption("keep-save", "want to kill process id", "ks");
    parser.addOption(keepSaveOption);
    QCommandLineOption channelModeOption("channel-mode", "channel model", "cm");
    parser.addOption(channelModeOption);
    QCommandLineOption thresholdOption("threshold", "channel model", "cm");
    parser.addOption(thresholdOption);
    parser.process(app);

    if (parser.isSet(pidOption)) {
        watchParentProcessFinished(parser.value(pidOption).toULong());
    }
    QString processType = parser.value(processTypeOption);
    if (processType == "screengrab") {
        if (parser.isSet(outFileOption) && parser.isSet(waitEventOption)) {
            runScreenGrab(parser.value(outFileOption),parser.value(waitEventOption));
        }
        return 0;
    }else if (processType == "channel"){
        ChannelHostOption optons;
        optons.keepSave = parser.value(keepSaveOption).toUShort();
        optons.mode = (Halve::ChannelMode)parser.value(channelModeOption).toUShort();
        return runChannel(app,parser.value(profileOption),optons);
    }else if (processType == "storagechecker"){
        return runStorageChecker(app,parser.value(profileOption), parser.value(thresholdOption).toInt());
    }else if (processType == "channel-watch"){
        return runChannelWatch(app);
    } else if (processType == "killer") {
        return runKiller(app, parser.value(kidOption));
    } else if (processType == "exporter") {
        return runExporter(app, parser.value(hostOption));
    }

    qmlRegisterTypes();
    StartoverState *startoverState = new StartoverState(&app);
    qmlRegisterSingletonInstance("Halve", 1, 0, "StartoverState", startoverState);

    QString userDataPath = prepareUserDataPath();
    std::unique_ptr<ProfileManager> profileManager(new ProfileManager(userDataPath, &app));

    const QStringList args = parser.positionalArguments();
    if (args.size() == 1) {
        QQuickVTKItem::setGraphicsApi();
        profileManager->setState(Halve::PMS_PERFORM);
        return runPerform(app, profileManager.get(), args[0], parser.value(profileOption));
    }
    return runStraining(app, profileManager.get());
}
