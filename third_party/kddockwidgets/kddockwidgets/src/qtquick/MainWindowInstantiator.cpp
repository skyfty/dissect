/*
  This file is part of KDDockWidgets.

  SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
  Author: Sérgio Martins <sergio.martins@kdab.com>

  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/

#include "MainWindowInstantiator.h"
#include "qtquick/views/MainWindowMDI.h"
#include "kddockwidgets/core/DockWidget.h"
#include "kddockwidgets/core/MainWindow.h"
#include "kddockwidgets/core/DockRegistry.h"

#include "DockWidgetInstantiator.h"
#include "Platform.h"

#include <QDateTime>
#include <QDebug>
#include <QString>

using namespace KDDockWidgets;
using namespace KDDockWidgets::QtQuick;

MainWindowInstantiator::MainWindowInstantiator()
    : QQuickItem()
{
}

QString MainWindowInstantiator::uniqueName() const
{
    return m_uniqueName;
}

void MainWindowInstantiator::setUniqueName(const QString &name)
{
    if (name != m_uniqueName) {
        m_uniqueName = name;
        Q_EMIT uniqueNameChanged();
    }
}

MainWindowOptions MainWindowInstantiator::options() const
{
    return m_options;
}

void MainWindowInstantiator::setOptions(MainWindowOptions options)
{
    if (m_options != options) {
        m_options = options;
        Q_EMIT optionsChanged();
    }
}

QVector<QString> MainWindowInstantiator::affinities() const
{
    return m_mainWindow ? m_mainWindow->affinities() : QVector<QString>();
}

bool MainWindowInstantiator::isMDI() const
{
    return m_mainWindow && m_mainWindow->isMDI();
}

void MainWindowInstantiator::addDockWidget(QQuickItem *dockWidget, Location location,
                                           QQuickItem *relativeTo, QSize initialSize,
                                           InitialVisibilityOption option)
{
    if (!dockWidget || !m_mainWindow)
        return;

    Core::DockWidget *dw = Platform::dockWidgetForItem(dockWidget);
    if (!dw) {
        // Can happen if the DockWidgetInstantiatior couldn't create a dockwidget,
        // for example, if using duplicate unique-names
        qWarning() << "MainWindowInstantiator::addDockWidget: Could not find dockwidget";
        return;
    }

    Core::DockWidget *relativeToDw = Platform::dockWidgetForItem(relativeTo);

    m_mainWindow->addDockWidget(dw, location, relativeToDw, { option, initialSize });
}

void MainWindowInstantiator::addDockWidgetAsTab(QQuickItem *dockWidget)
{
    if (!dockWidget || !m_mainWindow)
        return;

    Core::DockWidget *dw = Platform::dockWidgetForItem(dockWidget);
    m_mainWindow->addDockWidgetAsTab(dw);
}

void MainWindowInstantiator::layoutEqually()
{
    if (m_mainWindow)
        m_mainWindow->layoutEqually();
}

void MainWindowInstantiator::layoutParentContainerEqually(QQuickItem *dockWidget)
{
    if (!m_mainWindow || !dockWidget)
        return;

    Core::DockWidget *dw = Platform::dockWidgetForItem(dockWidget);
    m_mainWindow->layoutParentContainerEqually(dw);
}

void MainWindowInstantiator::moveToSideBar(QQuickItem *dockWidget)
{
    if (!m_mainWindow || !dockWidget)
        return;

    Core::DockWidget *dw = Platform::dockWidgetForItem(dockWidget);
    m_mainWindow->moveToSideBar(dw);
}

void MainWindowInstantiator::moveToSideBar(QQuickItem *dockWidget, SideBarLocation loc)
{
    if (!m_mainWindow || !dockWidget)
        return;

    Core::DockWidget *dw = Platform::dockWidgetForItem(dockWidget);
    m_mainWindow->moveToSideBar(dw, loc);
}

void MainWindowInstantiator::restoreFromSideBar(QQuickItem *dockWidget)
{
    if (!m_mainWindow || !dockWidget)
        return;

    Core::DockWidget *dw = Platform::dockWidgetForItem(dockWidget);
    m_mainWindow->restoreFromSideBar(dw);
}

void MainWindowInstantiator::overlayOnSideBar(QQuickItem *dockWidget)
{
    if (!m_mainWindow || !dockWidget)
        return;

    Core::DockWidget *dw = Platform::dockWidgetForItem(dockWidget);
    m_mainWindow->overlayOnSideBar(dw);
}

void MainWindowInstantiator::toggleOverlayOnSideBar(QQuickItem *dockWidget)
{
    if (!m_mainWindow || !dockWidget)
        return;

    Core::DockWidget *dw = Platform::dockWidgetForItem(dockWidget);
    m_mainWindow->toggleOverlayOnSideBar(dw);
}

void MainWindowInstantiator::clearSideBarOverlay(bool deleteFrame)
{
    if (m_mainWindow)
        m_mainWindow->clearSideBarOverlay(deleteFrame);
}

// TODOm3
// SideBar *MainWindowInstantiator::sideBarForDockWidget(const Core::DockWidget *dw) const
// {
//     return m_mainWindow ? m_mainWindow->sideBarForDockWidget(dw) : nullptr;
// }

bool MainWindowInstantiator::sideBarIsVisible(SideBarLocation loc) const
{
    return m_mainWindow && m_mainWindow->sideBarIsVisible(loc);
}

bool MainWindowInstantiator::closeDockWidgets(bool force)
{
    return m_mainWindow && m_mainWindow->closeDockWidgets(force);
}

void MainWindowInstantiator::toggleAllFloatWidget()
{
    for (KDDockWidgets::Core::DockWidget* dockWidget : DockRegistry::self()->dockwidgets()) {
        if (dockWidget->isFloating() ) {
            if (dockWidget->isOpen()) {
                dockWidget->close();
            } else {
                dockWidget->open();
            }
        }
    }
}

void MainWindowInstantiator::classBegin()
{
    // Nothing interesting to do here.
}

void MainWindowInstantiator::componentComplete()
{
    if (m_uniqueName.isEmpty()) {
        qWarning() << Q_FUNC_INFO
                   << "Each DockWidget need an unique name. Set the uniqueName property.";
        return;
    }

    if (DockRegistry::self()->containsMainWindow(m_uniqueName)) {
        // MainWindow already exists
        return;
    }

    if (m_uniqueName.isEmpty()) {
        qWarning() << Q_FUNC_INFO << "Name can't be empty";
        return;
    }

    if (m_mainWindow) {
        qWarning() << Q_FUNC_INFO << "Main window is already initialized";
        return;
    }

    const auto mainWindowOptions = MainWindowOptions(m_options);

    Core::View *view = nullptr;
    if (mainWindowOptions & MainWindowOption_MDI)
        view = new QtQuick::MainWindowMDI(m_uniqueName, this);
    else
        view = new QtQuick::MainWindow(m_uniqueName, mainWindowOptions, this);

    m_mainWindow = view->asMainWindowController();
}

void MainWindowInstantiator::toggleDockWidget(QQuickItem* dockWidget) {
    if (dockWidget == nullptr) {
        return;
    }
    Core::DockWidget* dw = KDDockWidgets::QtQuick::Platform::dockWidgetForItem(dockWidget);

    if (dw->isOpen()) {
        dw->close();
    } else {
        dw->open();
    }
}


void MainWindowInstantiator::toggleDockWidget(const QString &name, const QString& title, KDDockWidgets::Location loc,QQuickItem *relativeTo, QRect suggestedRect,InitialVisibilityOption option) {
    KDDockWidgets::Core::DockWidget* dockWidget = KDDockWidgets::DockRegistry::self()->dockByName(name);
    if (dockWidget == nullptr) {
        auto dw = new KDDockWidgets::QtQuick::DockWidget(name);
        dw->setGuestItem(name);
        dockWidget = dw->dockWidget();
    }
    dockWidget->setTitle(title);

    if (dockWidget->isOpen()) {
        dockWidget->close();
    } else {
        dockWidget->open();
    }
    if (loc != Location_None) {
        Core::DockWidget *relativeToDw = Platform::dockWidgetForItem(relativeTo);
        m_mainWindow->addDockWidget(dockWidget, loc, relativeToDw, { option, suggestedRect.size() });
    } else if (!suggestedRect.isNull()) {
        const Point originalCenter = suggestedRect.center();
        suggestedRect.moveCenter(originalCenter);
        dockWidget->setFloatingGeometry(suggestedRect);
    }
}

void MainWindowInstantiator::createWidget(const QString& title, const QString &genre) {
    KDDockWidgets::QtQuick::DockWidget* curdock = nullptr;
    for (KDDockWidgets::Core::DockWidget* dw2 : DockRegistry::self()->dockwidgets()) {
        if (!dw2->isFloating() && !dw2->isTabbed() && !dw2->isInSideBar()) {
            auto type = dw2->property("WidgetType");
            if (type.toString() == genre) {
                curdock = dynamic_cast<KDDockWidgets::QtQuick::DockWidget*>(dw2->view());
                break;
            }
        }
    }

    QString name = QString(QLatin1String("Widget_%1")).arg(QDateTime::currentMSecsSinceEpoch());
    auto dw = new KDDockWidgets::QtQuick::DockWidget(name, DockWidgetOption::DockWidgetOption_DeleteOnClose);
    dw->setGuestItem(genre);
    dw->setTitle(title);
    dw->dockWidget()->setProperty("WidgetType", genre);
    auto mainArea = KDDockWidgets::DockRegistry::self()->mainDockingAreas().constFirst();
    if (curdock != nullptr) {
        mainArea->addDockWidget(dw, KDDockWidgets::Location::Location_OnRight, curdock);
    }else {
        mainArea->addDockWidget(name, KDDockWidgets::Location::Location_OnTop);
    }
}
