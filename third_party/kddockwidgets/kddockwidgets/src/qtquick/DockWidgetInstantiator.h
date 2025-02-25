/*
  This file is part of KDDockWidgets.

  SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
  Author: Sérgio Martins <sergio.martins@kdab.com>

  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/

#ifndef KD_DOCKWIDGET_INSTANTIATOR_P_H
#define KD_DOCKWIDGET_INSTANTIATOR_P_H

#include "qtquick/views/DockWidget.h"
#include "qtquick/views/TitleBar.h"

#include <QQmlParserStatus>
#include <QQuickItem>
#include <QString>

#include <optional>

namespace KDDockWidgets {

/**
 * @brief Indirection helper to instantiate dock widgets from QML
 *
 * "DockWidget {}" in QML won't create a KDDockWidget::DockWidget directly, but instead an
 * DockWidgetInstantiator. DockWidgetInstantiator will then create the DockWidget instance only
 * when the QML parsing ends (and all properties are set)
 *
 * This allows to pass the correct uniqueName to DockWidget's ctor.
 */
class DockWidgetInstantiator : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString uniqueName READ uniqueName WRITE setUniqueName NOTIFY uniqueNameChanged)
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(KDDockWidgets::QtQuick::DockWidget *dockWidget READ dockWidget NOTIFY
                   dockWidgetChanged)
    Q_PROPERTY(QObject *actualTitleBar READ actualTitleBar NOTIFY actualTitleBarChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(bool isFocused READ isFocused NOTIFY isFocusedChanged)
    Q_PROPERTY(bool isFloating READ isFloating WRITE setFloating NOTIFY isFloatingChanged)
    Q_PROPERTY(bool isOpen READ isOpen NOTIFY isOpenChanged)
    Q_PROPERTY(bool isCloseable READ isCloseable  WRITE setCloseable NOTIFY isCloseableChanged)
    Q_PROPERTY(DockWidgetOptions options READ options WRITE setOptions FINAL)

public:
    QString uniqueName() const;
    void setUniqueName(const QString &);

    QString source() const;
    void setSource(const QString &);

    KDDockWidgets::QtQuick::DockWidget *dockWidget() const;
    KDDockWidgets::Core::DockWidget *controller() const;
    QObject *actualTitleBar() const;

    QString title() const;
    void setTitle(const QString &title);

    bool isFocused() const;
    bool isFloating() const;
    bool isOpen() const;
    void setFloating(bool);
    bool isCloseable() const;
    void setCloseable(bool);

    Q_INVOKABLE void addDockWidgetAsTab(QQuickItem *other,
                                        KDDockWidgets::InitialVisibilityOption = {});

    Q_INVOKABLE void addDockWidgetToContainingWindow(QQuickItem *other,
                                                     KDDockWidgets::Location location,
                                                     QQuickItem *relativeTo = nullptr,
                                                     QSize initialSize = {},
                                                     KDDockWidgets::InitialVisibilityOption = {});

    Q_INVOKABLE void setAsCurrentTab();
    Q_INVOKABLE void forceClose();
    Q_INVOKABLE bool close();
    Q_INVOKABLE void open();
    Q_INVOKABLE void show();
    Q_INVOKABLE void raise();
    Q_INVOKABLE void moveToSideBar();
    Q_INVOKABLE void moveToCenter();


    DockWidgetOptions options() const;
    void setOptions(const DockWidgetOptions &newOptions);

protected:
    void classBegin() override;
    void componentComplete() override;

Q_SIGNALS:
    void uniqueNameChanged();
    void sourceChanged();
    void dockWidgetChanged();
    void actualTitleBarChanged();
    void titleChanged(const QString &title);
    void iconChanged();
    void closed();
    void guestViewChanged(QQuickItem *);
    void optionsChanged(KDDockWidgets::DockWidgetOptions);
    void isFocusedChanged(bool);
    void isOverlayedChanged(bool);
    void isFloatingChanged(bool);
    void isOpenChanged(bool);
    void removedFromSideBar();
    void windowActiveAboutToChange(bool activated);
    void isCloseableChanged();


private:
    std::optional<bool> m_isFloating;
    QString m_uniqueName;
    QString m_sourceFilename;
    QString m_title;
    DockWidgetOptions m_options = DockWidgetOption_None;
    Core::DockWidget *m_dockWidget = nullptr;
};
}

#endif
