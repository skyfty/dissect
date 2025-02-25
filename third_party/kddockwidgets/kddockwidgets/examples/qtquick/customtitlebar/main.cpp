/*
  This file is part of KDDockWidgets.

  SPDX-FileCopyrightText: 2020-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
  Author: Sergio Martins <sergio.martins@kdab.com>

  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/


#include <kddockwidgets/Config.h>
#include <kddockwidgets/core/DockRegistry.h>
#include <kddockwidgets/qtquick/views/DockWidget.h>
#include <kddockwidgets/qtquick/Platform.h>
#include <kddockwidgets/qtquick/ViewFactory.h>
#include <kddockwidgets/qtquick/views/MainWindow.h>


#include <QQuickView>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

class CustomViewFactory : public KDDockWidgets::QtQuick::ViewFactory
{
public:
    ~CustomViewFactory() override;

    QUrl titleBarFilename() const override
    {
        return QUrl("qrc:/MyTitleBar.qml");
    }

    // Optional, just for illustration purposes
    QUrl separatorFilename() const override
    {
        return QUrl("qrc:/MySeparator.qml");
    }
};

CustomViewFactory::~CustomViewFactory() = default;

int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN
    QGuiApplication::setAttribute(Qt::AA_UseOpenGLES);
#endif
    QGuiApplication app(argc, argv);
    KDDockWidgets::initFrontend(KDDockWidgets::FrontendType::QtQuick);

    auto &config = KDDockWidgets::Config::self();
    auto flags = config.flags() | KDDockWidgets::Config::Flag_TitleBarIsFocusable;

    config.setFlags(flags);
    config.setViewFactory(new CustomViewFactory());

    QQmlApplicationEngine appEngine;
    KDDockWidgets::QtQuick::Platform::instance()->setQmlEngine(&appEngine);
    appEngine.load((QUrl("qrc:/main.qml")));

    auto dw1 = new KDDockWidgets::QtQuick::DockWidget("Dock #1");

    dw1->setGuestItem(QStringLiteral("qrc:/Guest1.qml"));
    dw1->resize(QSize(800, 800));
    dw1->open();

    auto dw2 = new KDDockWidgets::QtQuick::DockWidget("Dock #2");
    dw2->setGuestItem(QStringLiteral("qrc:/Guest2.qml"));
    dw2->resize(QSize(800, 800));
    dw2->open();

    auto dw3 = new KDDockWidgets::QtQuick::DockWidget("Dock #3");
    dw3->setGuestItem(QStringLiteral("qrc:/Guest3.qml"));

    dw1->addDockWidgetToContainingWindow(dw3, KDDockWidgets::Location_OnRight);

    // Access the main area we created in QML with DockingArea {}
    auto mainArea = KDDockWidgets::DockRegistry::self()->mainDockingAreas().constFirst();
    mainArea->addDockWidget(dw2, KDDockWidgets::Location_OnTop);

    return app.exec();
}
