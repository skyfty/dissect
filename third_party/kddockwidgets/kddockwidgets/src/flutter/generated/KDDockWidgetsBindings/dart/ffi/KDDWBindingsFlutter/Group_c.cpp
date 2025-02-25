/*
  This file is part of KDDockWidgets.

  SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
  Author: Sérgio Martins <sergio.martins@kdab.com>

  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/
#include "Group_c.h"


#include <iostream>

#include <cassert>


namespace Dartagnan {

typedef int (*CleanupCallback)(void *thisPtr);
static CleanupCallback s_cleanupCallback = nullptr;

template<typename T>
struct ValueWrapper
{
    T value;
};

}
namespace KDDockWidgetsBindings_wrappersNS {
namespace KDDWBindingsFlutter {
Group_wrapper::Group_wrapper(KDDockWidgets::Core::Group *controller, KDDockWidgets::Core::View *parent)
    : ::KDDockWidgets::flutter::Group(controller, parent)
{
}
void Group_wrapper::activateWindow()
{
    if (m_activateWindowCallback) {
        const void *thisPtr = this;
        m_activateWindowCallback(const_cast<void *>(thisPtr));
    } else {
        ::KDDockWidgets::flutter::Group::activateWindow();
    }
}
void Group_wrapper::activateWindow_nocallback()
{
    ::KDDockWidgets::flutter::Group::activateWindow();
}
bool Group_wrapper::close()
{
    if (m_closeCallback) {
        const void *thisPtr = this;
        return m_closeCallback(const_cast<void *>(thisPtr));
    } else {
        return ::KDDockWidgets::flutter::Group::close();
    }
}
bool Group_wrapper::close_nocallback()
{
    return ::KDDockWidgets::flutter::Group::close();
}
void Group_wrapper::createPlatformWindow()
{
    if (m_createPlatformWindowCallback) {
        const void *thisPtr = this;
        m_createPlatformWindowCallback(const_cast<void *>(thisPtr));
    } else {
        ::KDDockWidgets::flutter::Group::createPlatformWindow();
    }
}
void Group_wrapper::createPlatformWindow_nocallback()
{
    ::KDDockWidgets::flutter::Group::createPlatformWindow();
}
int Group_wrapper::currentIndex() const
{
    return ::KDDockWidgets::flutter::Group::currentIndex();
}
KDDockWidgets::Rect Group_wrapper::dragRect() const
{
    if (m_dragRectCallback) {
        const void *thisPtr = this;
        return *m_dragRectCallback(const_cast<void *>(thisPtr));
    } else {
        return ::KDDockWidgets::flutter::Group::dragRect();
    }
}
KDDockWidgets::Rect Group_wrapper::dragRect_nocallback() const
{
    return ::KDDockWidgets::flutter::Group::dragRect();
}
Qt::WindowFlags Group_wrapper::flags() const
{
    if (m_flagsCallback) {
        const void *thisPtr = this;
        return m_flagsCallback(const_cast<void *>(thisPtr));
    } else {
        return ::KDDockWidgets::flutter::Group::flags();
    }
}
Qt::WindowFlags Group_wrapper::flags_nocallback() const
{
    return ::KDDockWidgets::flutter::Group::flags();
}
KDDockWidgets::Rect Group_wrapper::geometry() const
{
    if (m_geometryCallback) {
        const void *thisPtr = this;
        return *m_geometryCallback(const_cast<void *>(thisPtr));
    } else {
        return ::KDDockWidgets::flutter::Group::geometry();
    }
}
KDDockWidgets::Rect Group_wrapper::geometry_nocallback() const
{
    return ::KDDockWidgets::flutter::Group::geometry();
}
void Group_wrapper::grabMouse()
{
    if (m_grabMouseCallback) {
        const void *thisPtr = this;
        m_grabMouseCallback(const_cast<void *>(thisPtr));
    } else {
        ::KDDockWidgets::flutter::Group::grabMouse();
    }
}
void Group_wrapper::grabMouse_nocallback()
{
    ::KDDockWidgets::flutter::Group::grabMouse();
}
bool Group_wrapper::hasFocus() const
{
    if (m_hasFocusCallback) {
        const void *thisPtr = this;
        return m_hasFocusCallback(const_cast<void *>(thisPtr));
    } else {
        return ::KDDockWidgets::flutter::Group::hasFocus();
    }
}
bool Group_wrapper::hasFocus_nocallback() const
{
    return ::KDDockWidgets::flutter::Group::hasFocus();
}
void Group_wrapper::hide()
{
    if (m_hideCallback) {
        const void *thisPtr = this;
        m_hideCallback(const_cast<void *>(thisPtr));
    } else {
        ::KDDockWidgets::flutter::Group::hide();
    }
}
void Group_wrapper::hide_nocallback()
{
    ::KDDockWidgets::flutter::Group::hide();
}
void Group_wrapper::init()
{
    if (m_initCallback) {
        const void *thisPtr = this;
        m_initCallback(const_cast<void *>(thisPtr));
    } else {
        ::KDDockWidgets::flutter::Group::init();
    }
}
void Group_wrapper::init_nocallback()
{
    ::KDDockWidgets::flutter::Group::init();
}
bool Group_wrapper::isActiveWindow() const
{
    if (m_isActiveWindowCallback) {
        const void *thisPtr = this;
        return m_isActiveWindowCallback(const_cast<void *>(thisPtr));
    } else {
        return ::KDDockWidgets::flutter::Group::isActiveWindow();
    }
}
bool Group_wrapper::isActiveWindow_nocallback() const
{
    return ::KDDockWidgets::flutter::Group::isActiveWindow();
}
bool Group_wrapper::isExplicitlyHidden() const
{
    if (m_isExplicitlyHiddenCallback) {
        const void *thisPtr = this;
        return m_isExplicitlyHiddenCallback(const_cast<void *>(thisPtr));
    } else {
        return ::KDDockWidgets::flutter::Group::isExplicitlyHidden();
    }
}
bool Group_wrapper::isExplicitlyHidden_nocallback() const
{
    return ::KDDockWidgets::flutter::Group::isExplicitlyHidden();
}
bool Group_wrapper::isMaximized() const
{
    if (m_isMaximizedCallback) {
        const void *thisPtr = this;
        return m_isMaximizedCallback(const_cast<void *>(thisPtr));
    } else {
        return ::KDDockWidgets::flutter::Group::isMaximized();
    }
}
bool Group_wrapper::isMaximized_nocallback() const
{
    return ::KDDockWidgets::flutter::Group::isMaximized();
}
bool Group_wrapper::isMinimized() const
{
    if (m_isMinimizedCallback) {
        const void *thisPtr = this;
        return m_isMinimizedCallback(const_cast<void *>(thisPtr));
    } else {
        return ::KDDockWidgets::flutter::Group::isMinimized();
    }
}
bool Group_wrapper::isMinimized_nocallback() const
{
    return ::KDDockWidgets::flutter::Group::isMinimized();
}
bool Group_wrapper::isMounted() const
{
    if (m_isMountedCallback) {
        const void *thisPtr = this;
        return m_isMountedCallback(const_cast<void *>(thisPtr));
    } else {
        return ::KDDockWidgets::flutter::Group::isMounted();
    }
}
bool Group_wrapper::isMounted_nocallback() const
{
    return ::KDDockWidgets::flutter::Group::isMounted();
}
bool Group_wrapper::isNull() const
{
    if (m_isNullCallback) {
        const void *thisPtr = this;
        return m_isNullCallback(const_cast<void *>(thisPtr));
    } else {
        return ::KDDockWidgets::flutter::Group::isNull();
    }
}
bool Group_wrapper::isNull_nocallback() const
{
    return ::KDDockWidgets::flutter::Group::isNull();
}
bool Group_wrapper::isRootView() const
{
    if (m_isRootViewCallback) {
        const void *thisPtr = this;
        return m_isRootViewCallback(const_cast<void *>(thisPtr));
    } else {
        return ::KDDockWidgets::flutter::Group::isRootView();
    }
}
bool Group_wrapper::isRootView_nocallback() const
{
    return ::KDDockWidgets::flutter::Group::isRootView();
}
bool Group_wrapper::isVisible() const
{
    if (m_isVisibleCallback) {
        const void *thisPtr = this;
        return m_isVisibleCallback(const_cast<void *>(thisPtr));
    } else {
        return ::KDDockWidgets::flutter::Group::isVisible();
    }
}
bool Group_wrapper::isVisible_nocallback() const
{
    return ::KDDockWidgets::flutter::Group::isVisible();
}
KDDockWidgets::Point Group_wrapper::mapFromGlobal(KDDockWidgets::Point globalPt) const
{
    if (m_mapFromGlobalCallback) {
        const void *thisPtr = this;
        return *m_mapFromGlobalCallback(const_cast<void *>(thisPtr), &globalPt);
    } else {
        return ::KDDockWidgets::flutter::Group::mapFromGlobal(globalPt);
    }
}
KDDockWidgets::Point Group_wrapper::mapFromGlobal_nocallback(KDDockWidgets::Point globalPt) const
{
    return ::KDDockWidgets::flutter::Group::mapFromGlobal(globalPt);
}
KDDockWidgets::Point Group_wrapper::mapTo(KDDockWidgets::Core::View *parent, KDDockWidgets::Point pos) const
{
    if (m_mapToCallback) {
        const void *thisPtr = this;
        return *m_mapToCallback(const_cast<void *>(thisPtr), parent, &pos);
    } else {
        return ::KDDockWidgets::flutter::Group::mapTo(parent, pos);
    }
}
KDDockWidgets::Point Group_wrapper::mapTo_nocallback(KDDockWidgets::Core::View *parent, KDDockWidgets::Point pos) const
{
    return ::KDDockWidgets::flutter::Group::mapTo(parent, pos);
}
KDDockWidgets::Point Group_wrapper::mapToGlobal(KDDockWidgets::Point localPt) const
{
    if (m_mapToGlobalCallback) {
        const void *thisPtr = this;
        return *m_mapToGlobalCallback(const_cast<void *>(thisPtr), &localPt);
    } else {
        return ::KDDockWidgets::flutter::Group::mapToGlobal(localPt);
    }
}
KDDockWidgets::Point Group_wrapper::mapToGlobal_nocallback(KDDockWidgets::Point localPt) const
{
    return ::KDDockWidgets::flutter::Group::mapToGlobal(localPt);
}
KDDockWidgets::Size Group_wrapper::maxSizeHint() const
{
    if (m_maxSizeHintCallback) {
        const void *thisPtr = this;
        return *m_maxSizeHintCallback(const_cast<void *>(thisPtr));
    } else {
        return ::KDDockWidgets::flutter::Group::maxSizeHint();
    }
}
KDDockWidgets::Size Group_wrapper::maxSizeHint_nocallback() const
{
    return ::KDDockWidgets::flutter::Group::maxSizeHint();
}
KDDockWidgets::Size Group_wrapper::minSize() const
{
    if (m_minSizeCallback) {
        const void *thisPtr = this;
        return *m_minSizeCallback(const_cast<void *>(thisPtr));
    } else {
        return ::KDDockWidgets::flutter::Group::minSize();
    }
}
KDDockWidgets::Size Group_wrapper::minSize_nocallback() const
{
    return ::KDDockWidgets::flutter::Group::minSize();
}
void Group_wrapper::move(int x, int y)
{
    if (m_move_2Callback) {
        const void *thisPtr = this;
        m_move_2Callback(const_cast<void *>(thisPtr), x, y);
    } else {
        ::KDDockWidgets::flutter::Group::move(x, y);
    }
}
void Group_wrapper::move_nocallback(int x, int y)
{
    ::KDDockWidgets::flutter::Group::move(x, y);
}
int Group_wrapper::nonContentsHeight() const
{
    if (m_nonContentsHeightCallback) {
        const void *thisPtr = this;
        return m_nonContentsHeightCallback(const_cast<void *>(thisPtr));
    } else {
        return ::KDDockWidgets::flutter::Group::nonContentsHeight();
    }
}
int Group_wrapper::nonContentsHeight_nocallback() const
{
    return ::KDDockWidgets::flutter::Group::nonContentsHeight();
}
KDDockWidgets::Rect Group_wrapper::normalGeometry() const
{
    if (m_normalGeometryCallback) {
        const void *thisPtr = this;
        return *m_normalGeometryCallback(const_cast<void *>(thisPtr));
    } else {
        return ::KDDockWidgets::flutter::Group::normalGeometry();
    }
}
KDDockWidgets::Rect Group_wrapper::normalGeometry_nocallback() const
{
    return ::KDDockWidgets::flutter::Group::normalGeometry();
}
void Group_wrapper::onChildAdded(KDDockWidgets::Core::View *childView)
{
    if (m_onChildAddedCallback) {
        const void *thisPtr = this;
        m_onChildAddedCallback(const_cast<void *>(thisPtr), childView);
    } else {
        ::KDDockWidgets::flutter::Group::onChildAdded(childView);
    }
}
void Group_wrapper::onChildAdded_nocallback(KDDockWidgets::Core::View *childView)
{
    ::KDDockWidgets::flutter::Group::onChildAdded(childView);
}
void Group_wrapper::onChildRemoved(KDDockWidgets::Core::View *childView)
{
    if (m_onChildRemovedCallback) {
        const void *thisPtr = this;
        m_onChildRemovedCallback(const_cast<void *>(thisPtr), childView);
    } else {
        ::KDDockWidgets::flutter::Group::onChildRemoved(childView);
    }
}
void Group_wrapper::onChildRemoved_nocallback(KDDockWidgets::Core::View *childView)
{
    ::KDDockWidgets::flutter::Group::onChildRemoved(childView);
}
void Group_wrapper::onChildVisibilityChanged(KDDockWidgets::Core::View *childView)
{
    if (m_onChildVisibilityChangedCallback) {
        const void *thisPtr = this;
        m_onChildVisibilityChangedCallback(const_cast<void *>(thisPtr), childView);
    } else {
        ::KDDockWidgets::flutter::Group::onChildVisibilityChanged(childView);
    }
}
void Group_wrapper::onChildVisibilityChanged_nocallback(KDDockWidgets::Core::View *childView)
{
    ::KDDockWidgets::flutter::Group::onChildVisibilityChanged(childView);
}
void Group_wrapper::onGeometryChanged()
{
    if (m_onGeometryChangedCallback) {
        const void *thisPtr = this;
        m_onGeometryChangedCallback(const_cast<void *>(thisPtr));
    } else {
        ::KDDockWidgets::flutter::Group::onGeometryChanged();
    }
}
void Group_wrapper::onGeometryChanged_nocallback()
{
    ::KDDockWidgets::flutter::Group::onGeometryChanged();
}
void Group_wrapper::onRebuildRequested()
{
    if (m_onRebuildRequestedCallback) {
        const void *thisPtr = this;
        m_onRebuildRequestedCallback(const_cast<void *>(thisPtr));
    } else {
        ::KDDockWidgets::flutter::Group::onRebuildRequested();
    }
}
void Group_wrapper::onRebuildRequested_nocallback()
{
    ::KDDockWidgets::flutter::Group::onRebuildRequested();
}
bool Group_wrapper::onResize(int h, int w)
{
    if (m_onResize_2Callback) {
        const void *thisPtr = this;
        return m_onResize_2Callback(const_cast<void *>(thisPtr), h, w);
    } else {
        return ::KDDockWidgets::flutter::Group::onResize(h, w);
    }
}
bool Group_wrapper::onResize_nocallback(int h, int w)
{
    return ::KDDockWidgets::flutter::Group::onResize(h, w);
}
void Group_wrapper::raise()
{
    if (m_raiseCallback) {
        const void *thisPtr = this;
        m_raiseCallback(const_cast<void *>(thisPtr));
    } else {
        ::KDDockWidgets::flutter::Group::raise();
    }
}
void Group_wrapper::raise_nocallback()
{
    ::KDDockWidgets::flutter::Group::raise();
}
void Group_wrapper::raiseAndActivate()
{
    if (m_raiseAndActivateCallback) {
        const void *thisPtr = this;
        m_raiseAndActivateCallback(const_cast<void *>(thisPtr));
    } else {
        ::KDDockWidgets::flutter::Group::raiseAndActivate();
    }
}
void Group_wrapper::raiseAndActivate_nocallback()
{
    ::KDDockWidgets::flutter::Group::raiseAndActivate();
}
void Group_wrapper::raiseChild(KDDockWidgets::Core::View *childView)
{
    if (m_raiseChildCallback) {
        const void *thisPtr = this;
        m_raiseChildCallback(const_cast<void *>(thisPtr), childView);
    } else {
        ::KDDockWidgets::flutter::Group::raiseChild(childView);
    }
}
void Group_wrapper::raiseChild_nocallback(KDDockWidgets::Core::View *childView)
{
    ::KDDockWidgets::flutter::Group::raiseChild(childView);
}
void Group_wrapper::raiseWindow(KDDockWidgets::Core::View *rootView)
{
    if (m_raiseWindowCallback) {
        const void *thisPtr = this;
        m_raiseWindowCallback(const_cast<void *>(thisPtr), rootView);
    } else {
        ::KDDockWidgets::flutter::Group::raiseWindow(rootView);
    }
}
void Group_wrapper::raiseWindow_nocallback(KDDockWidgets::Core::View *rootView)
{
    ::KDDockWidgets::flutter::Group::raiseWindow(rootView);
}
void Group_wrapper::releaseKeyboard()
{
    if (m_releaseKeyboardCallback) {
        const void *thisPtr = this;
        m_releaseKeyboardCallback(const_cast<void *>(thisPtr));
    } else {
        ::KDDockWidgets::flutter::Group::releaseKeyboard();
    }
}
void Group_wrapper::releaseKeyboard_nocallback()
{
    ::KDDockWidgets::flutter::Group::releaseKeyboard();
}
void Group_wrapper::releaseMouse()
{
    if (m_releaseMouseCallback) {
        const void *thisPtr = this;
        m_releaseMouseCallback(const_cast<void *>(thisPtr));
    } else {
        ::KDDockWidgets::flutter::Group::releaseMouse();
    }
}
void Group_wrapper::releaseMouse_nocallback()
{
    ::KDDockWidgets::flutter::Group::releaseMouse();
}
void Group_wrapper::setCursor(Qt::CursorShape shape)
{
    if (m_setCursorCallback) {
        const void *thisPtr = this;
        m_setCursorCallback(const_cast<void *>(thisPtr), shape);
    } else {
        ::KDDockWidgets::flutter::Group::setCursor(shape);
    }
}
void Group_wrapper::setCursor_nocallback(Qt::CursorShape shape)
{
    ::KDDockWidgets::flutter::Group::setCursor(shape);
}
void Group_wrapper::setFixedHeight(int h)
{
    if (m_setFixedHeightCallback) {
        const void *thisPtr = this;
        m_setFixedHeightCallback(const_cast<void *>(thisPtr), h);
    } else {
        ::KDDockWidgets::flutter::Group::setFixedHeight(h);
    }
}
void Group_wrapper::setFixedHeight_nocallback(int h)
{
    ::KDDockWidgets::flutter::Group::setFixedHeight(h);
}
void Group_wrapper::setFixedWidth(int w)
{
    if (m_setFixedWidthCallback) {
        const void *thisPtr = this;
        m_setFixedWidthCallback(const_cast<void *>(thisPtr), w);
    } else {
        ::KDDockWidgets::flutter::Group::setFixedWidth(w);
    }
}
void Group_wrapper::setFixedWidth_nocallback(int w)
{
    ::KDDockWidgets::flutter::Group::setFixedWidth(w);
}
void Group_wrapper::setGeometry(KDDockWidgets::Rect geometry)
{
    if (m_setGeometryCallback) {
        const void *thisPtr = this;
        m_setGeometryCallback(const_cast<void *>(thisPtr), &geometry);
    } else {
        ::KDDockWidgets::flutter::Group::setGeometry(geometry);
    }
}
void Group_wrapper::setGeometry_nocallback(KDDockWidgets::Rect geometry)
{
    ::KDDockWidgets::flutter::Group::setGeometry(geometry);
}
void Group_wrapper::setHeight(int h)
{
    if (m_setHeightCallback) {
        const void *thisPtr = this;
        m_setHeightCallback(const_cast<void *>(thisPtr), h);
    } else {
        ::KDDockWidgets::flutter::Group::setHeight(h);
    }
}
void Group_wrapper::setHeight_nocallback(int h)
{
    ::KDDockWidgets::flutter::Group::setHeight(h);
}
void Group_wrapper::setMaximumSize(KDDockWidgets::Size sz)
{
    if (m_setMaximumSizeCallback) {
        const void *thisPtr = this;
        m_setMaximumSizeCallback(const_cast<void *>(thisPtr), &sz);
    } else {
        ::KDDockWidgets::flutter::Group::setMaximumSize(sz);
    }
}
void Group_wrapper::setMaximumSize_nocallback(KDDockWidgets::Size sz)
{
    ::KDDockWidgets::flutter::Group::setMaximumSize(sz);
}
void Group_wrapper::setMinimumSize(KDDockWidgets::Size sz)
{
    if (m_setMinimumSizeCallback) {
        const void *thisPtr = this;
        m_setMinimumSizeCallback(const_cast<void *>(thisPtr), &sz);
    } else {
        ::KDDockWidgets::flutter::Group::setMinimumSize(sz);
    }
}
void Group_wrapper::setMinimumSize_nocallback(KDDockWidgets::Size sz)
{
    ::KDDockWidgets::flutter::Group::setMinimumSize(sz);
}
void Group_wrapper::setMouseTracking(bool enable)
{
    if (m_setMouseTrackingCallback) {
        const void *thisPtr = this;
        m_setMouseTrackingCallback(const_cast<void *>(thisPtr), enable);
    } else {
        ::KDDockWidgets::flutter::Group::setMouseTracking(enable);
    }
}
void Group_wrapper::setMouseTracking_nocallback(bool enable)
{
    ::KDDockWidgets::flutter::Group::setMouseTracking(enable);
}
void Group_wrapper::setParent(KDDockWidgets::Core::View *parent)
{
    if (m_setParentCallback) {
        const void *thisPtr = this;
        m_setParentCallback(const_cast<void *>(thisPtr), parent);
    } else {
        ::KDDockWidgets::flutter::Group::setParent(parent);
    }
}
void Group_wrapper::setParent_nocallback(KDDockWidgets::Core::View *parent)
{
    ::KDDockWidgets::flutter::Group::setParent(parent);
}
void Group_wrapper::setSize(int w, int h)
{
    if (m_setSize_2Callback) {
        const void *thisPtr = this;
        m_setSize_2Callback(const_cast<void *>(thisPtr), w, h);
    } else {
        ::KDDockWidgets::flutter::Group::setSize(w, h);
    }
}
void Group_wrapper::setSize_nocallback(int w, int h)
{
    ::KDDockWidgets::flutter::Group::setSize(w, h);
}
void Group_wrapper::setViewName(const QString &name)
{
    if (m_setViewNameCallback) {
        const void *thisPtr = this;
        m_setViewNameCallback(const_cast<void *>(thisPtr), name);
    } else {
        ::KDDockWidgets::flutter::Group::setViewName(name);
    }
}
void Group_wrapper::setViewName_nocallback(const QString &name)
{
    ::KDDockWidgets::flutter::Group::setViewName(name);
}
void Group_wrapper::setVisible(bool visible)
{
    if (m_setVisibleCallback) {
        const void *thisPtr = this;
        m_setVisibleCallback(const_cast<void *>(thisPtr), visible);
    } else {
        ::KDDockWidgets::flutter::Group::setVisible(visible);
    }
}
void Group_wrapper::setVisible_nocallback(bool visible)
{
    ::KDDockWidgets::flutter::Group::setVisible(visible);
}
void Group_wrapper::setWidth(int w)
{
    if (m_setWidthCallback) {
        const void *thisPtr = this;
        m_setWidthCallback(const_cast<void *>(thisPtr), w);
    } else {
        ::KDDockWidgets::flutter::Group::setWidth(w);
    }
}
void Group_wrapper::setWidth_nocallback(int w)
{
    ::KDDockWidgets::flutter::Group::setWidth(w);
}
void Group_wrapper::setWindowOpacity(double v)
{
    if (m_setWindowOpacityCallback) {
        const void *thisPtr = this;
        m_setWindowOpacityCallback(const_cast<void *>(thisPtr), v);
    } else {
        ::KDDockWidgets::flutter::Group::setWindowOpacity(v);
    }
}
void Group_wrapper::setWindowOpacity_nocallback(double v)
{
    ::KDDockWidgets::flutter::Group::setWindowOpacity(v);
}
void Group_wrapper::setWindowTitle(const QString &title)
{
    if (m_setWindowTitleCallback) {
        const void *thisPtr = this;
        m_setWindowTitleCallback(const_cast<void *>(thisPtr), title);
    } else {
        ::KDDockWidgets::flutter::Group::setWindowTitle(title);
    }
}
void Group_wrapper::setWindowTitle_nocallback(const QString &title)
{
    ::KDDockWidgets::flutter::Group::setWindowTitle(title);
}
void Group_wrapper::setZOrder(int z)
{
    if (m_setZOrderCallback) {
        const void *thisPtr = this;
        m_setZOrderCallback(const_cast<void *>(thisPtr), z);
    } else {
        ::KDDockWidgets::flutter::Group::setZOrder(z);
    }
}
void Group_wrapper::setZOrder_nocallback(int z)
{
    ::KDDockWidgets::flutter::Group::setZOrder(z);
}
void Group_wrapper::show()
{
    if (m_showCallback) {
        const void *thisPtr = this;
        m_showCallback(const_cast<void *>(thisPtr));
    } else {
        ::KDDockWidgets::flutter::Group::show();
    }
}
void Group_wrapper::show_nocallback()
{
    ::KDDockWidgets::flutter::Group::show();
}
void Group_wrapper::showMaximized()
{
    if (m_showMaximizedCallback) {
        const void *thisPtr = this;
        m_showMaximizedCallback(const_cast<void *>(thisPtr));
    } else {
        ::KDDockWidgets::flutter::Group::showMaximized();
    }
}
void Group_wrapper::showMaximized_nocallback()
{
    ::KDDockWidgets::flutter::Group::showMaximized();
}
void Group_wrapper::showMinimized()
{
    if (m_showMinimizedCallback) {
        const void *thisPtr = this;
        m_showMinimizedCallback(const_cast<void *>(thisPtr));
    } else {
        ::KDDockWidgets::flutter::Group::showMinimized();
    }
}
void Group_wrapper::showMinimized_nocallback()
{
    ::KDDockWidgets::flutter::Group::showMinimized();
}
void Group_wrapper::showNormal()
{
    if (m_showNormalCallback) {
        const void *thisPtr = this;
        m_showNormalCallback(const_cast<void *>(thisPtr));
    } else {
        ::KDDockWidgets::flutter::Group::showNormal();
    }
}
void Group_wrapper::showNormal_nocallback()
{
    ::KDDockWidgets::flutter::Group::showNormal();
}
void Group_wrapper::update()
{
    if (m_updateCallback) {
        const void *thisPtr = this;
        m_updateCallback(const_cast<void *>(thisPtr));
    } else {
        ::KDDockWidgets::flutter::Group::update();
    }
}
void Group_wrapper::update_nocallback()
{
    ::KDDockWidgets::flutter::Group::update();
}
QString Group_wrapper::viewName() const
{
    if (m_viewNameCallback) {
        const void *thisPtr = this;
        return *m_viewNameCallback(const_cast<void *>(thisPtr));
    } else {
        return ::KDDockWidgets::flutter::Group::viewName();
    }
}
QString Group_wrapper::viewName_nocallback() const
{
    return ::KDDockWidgets::flutter::Group::viewName();
}
Group_wrapper::~Group_wrapper()
{
}

}
}
static KDDockWidgets::flutter::Group *fromPtr(void *ptr)
{
    return reinterpret_cast<KDDockWidgets::flutter::Group *>(ptr);
}
static KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper *fromWrapperPtr(void *ptr)
{
    return reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper *>(ptr);
}
extern "C" {
void c_KDDockWidgets__flutter__Group_Finalizer(void *cppObj)
{
    delete reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper *>(cppObj);
}
void *c_KDDockWidgets__flutter__Group__constructor_Group_View(void *controller_, void *parent_)
{
    auto controller = reinterpret_cast<KDDockWidgets::Core::Group *>(controller_);
    auto parent = reinterpret_cast<KDDockWidgets::Core::View *>(parent_);
    auto ptr = new KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper(controller, parent);
    return reinterpret_cast<void *>(ptr);
}
// activateWindow()
void c_KDDockWidgets__flutter__Group__activateWindow(void *thisObj)
{
    [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->activateWindow_nocallback();} else {    return targetPtr->activateWindow();} }();
}
// close()
bool c_KDDockWidgets__flutter__Group__close(void *thisObj)
{
    return [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->close_nocallback();} else {    return targetPtr->close();} }();
}
// createPlatformWindow()
void c_KDDockWidgets__flutter__Group__createPlatformWindow(void *thisObj)
{
    [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->createPlatformWindow_nocallback();} else {    return targetPtr->createPlatformWindow();} }();
}
// currentIndex() const
int c_KDDockWidgets__flutter__Group__currentIndex(void *thisObj)
{
    return fromPtr(thisObj)->currentIndex();
}
// dragRect() const
void *c_KDDockWidgets__flutter__Group__dragRect(void *thisObj)
{
    return new Dartagnan::ValueWrapper<KDDockWidgets::Rect> { [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->dragRect_nocallback();} else {    return targetPtr->dragRect();} }() };
}
// flags() const
int c_KDDockWidgets__flutter__Group__flags(void *thisObj)
{
    return [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->flags_nocallback();} else {    return targetPtr->flags();} }();
}
// geometry() const
void *c_KDDockWidgets__flutter__Group__geometry(void *thisObj)
{
    return new Dartagnan::ValueWrapper<KDDockWidgets::Rect> { [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->geometry_nocallback();} else {    return targetPtr->geometry();} }() };
}
// grabMouse()
void c_KDDockWidgets__flutter__Group__grabMouse(void *thisObj)
{
    [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->grabMouse_nocallback();} else {    return targetPtr->grabMouse();} }();
}
// hasFocus() const
bool c_KDDockWidgets__flutter__Group__hasFocus(void *thisObj)
{
    return [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->hasFocus_nocallback();} else {    return targetPtr->hasFocus();} }();
}
// hide()
void c_KDDockWidgets__flutter__Group__hide(void *thisObj)
{
    [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->hide_nocallback();} else {    return targetPtr->hide();} }();
}
// init()
void c_KDDockWidgets__flutter__Group__init(void *thisObj)
{
    [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->init_nocallback();} else {    return targetPtr->init();} }();
}
// isActiveWindow() const
bool c_KDDockWidgets__flutter__Group__isActiveWindow(void *thisObj)
{
    return [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->isActiveWindow_nocallback();} else {    return targetPtr->isActiveWindow();} }();
}
// isExplicitlyHidden() const
bool c_KDDockWidgets__flutter__Group__isExplicitlyHidden(void *thisObj)
{
    return [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->isExplicitlyHidden_nocallback();} else {    return targetPtr->isExplicitlyHidden();} }();
}
// isMaximized() const
bool c_KDDockWidgets__flutter__Group__isMaximized(void *thisObj)
{
    return [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->isMaximized_nocallback();} else {    return targetPtr->isMaximized();} }();
}
// isMinimized() const
bool c_KDDockWidgets__flutter__Group__isMinimized(void *thisObj)
{
    return [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->isMinimized_nocallback();} else {    return targetPtr->isMinimized();} }();
}
// isMounted() const
bool c_KDDockWidgets__flutter__Group__isMounted(void *thisObj)
{
    return [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->isMounted_nocallback();} else {    return targetPtr->isMounted();} }();
}
// isNull() const
bool c_KDDockWidgets__flutter__Group__isNull(void *thisObj)
{
    return [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->isNull_nocallback();} else {    return targetPtr->isNull();} }();
}
// isRootView() const
bool c_KDDockWidgets__flutter__Group__isRootView(void *thisObj)
{
    return [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->isRootView_nocallback();} else {    return targetPtr->isRootView();} }();
}
// isVisible() const
bool c_KDDockWidgets__flutter__Group__isVisible(void *thisObj)
{
    return [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->isVisible_nocallback();} else {    return targetPtr->isVisible();} }();
}
// mapFromGlobal(KDDockWidgets::Point globalPt) const
void *c_KDDockWidgets__flutter__Group__mapFromGlobal_Point(void *thisObj, void *globalPt_)
{
    assert(globalPt_);
    auto &globalPt = *reinterpret_cast<KDDockWidgets::Point *>(globalPt_);
    return new Dartagnan::ValueWrapper<KDDockWidgets::Point> { [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->mapFromGlobal_nocallback(globalPt);} else {    return targetPtr->mapFromGlobal(globalPt);} }() };
}
// mapTo(KDDockWidgets::Core::View * parent, KDDockWidgets::Point pos) const
void *c_KDDockWidgets__flutter__Group__mapTo_View_Point(void *thisObj, void *parent_, void *pos_)
{
    auto parent = reinterpret_cast<KDDockWidgets::Core::View *>(parent_);
    assert(pos_);
    auto &pos = *reinterpret_cast<KDDockWidgets::Point *>(pos_);
    return new Dartagnan::ValueWrapper<KDDockWidgets::Point> { [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->mapTo_nocallback(parent,pos);} else {    return targetPtr->mapTo(parent,pos);} }() };
}
// mapToGlobal(KDDockWidgets::Point localPt) const
void *c_KDDockWidgets__flutter__Group__mapToGlobal_Point(void *thisObj, void *localPt_)
{
    assert(localPt_);
    auto &localPt = *reinterpret_cast<KDDockWidgets::Point *>(localPt_);
    return new Dartagnan::ValueWrapper<KDDockWidgets::Point> { [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->mapToGlobal_nocallback(localPt);} else {    return targetPtr->mapToGlobal(localPt);} }() };
}
// maxSizeHint() const
void *c_KDDockWidgets__flutter__Group__maxSizeHint(void *thisObj)
{
    return new Dartagnan::ValueWrapper<KDDockWidgets::Size> { [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->maxSizeHint_nocallback();} else {    return targetPtr->maxSizeHint();} }() };
}
// minSize() const
void *c_KDDockWidgets__flutter__Group__minSize(void *thisObj)
{
    return new Dartagnan::ValueWrapper<KDDockWidgets::Size> { [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->minSize_nocallback();} else {    return targetPtr->minSize();} }() };
}
// move(int x, int y)
void c_KDDockWidgets__flutter__Group__move_int_int(void *thisObj, int x, int y)
{
    [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->move_nocallback(x,y);} else {    return targetPtr->move(x,y);} }();
}
// nonContentsHeight() const
int c_KDDockWidgets__flutter__Group__nonContentsHeight(void *thisObj)
{
    return fromWrapperPtr(thisObj)->nonContentsHeight_nocallback();
}
// normalGeometry() const
void *c_KDDockWidgets__flutter__Group__normalGeometry(void *thisObj)
{
    return new Dartagnan::ValueWrapper<KDDockWidgets::Rect> { [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->normalGeometry_nocallback();} else {    return targetPtr->normalGeometry();} }() };
}
// onChildAdded(KDDockWidgets::Core::View * childView)
void c_KDDockWidgets__flutter__Group__onChildAdded_View(void *thisObj, void *childView_)
{
    auto childView = reinterpret_cast<KDDockWidgets::Core::View *>(childView_);
    [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->onChildAdded_nocallback(childView);} else {    return targetPtr->onChildAdded(childView);} }();
}
// onChildRemoved(KDDockWidgets::Core::View * childView)
void c_KDDockWidgets__flutter__Group__onChildRemoved_View(void *thisObj, void *childView_)
{
    auto childView = reinterpret_cast<KDDockWidgets::Core::View *>(childView_);
    [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->onChildRemoved_nocallback(childView);} else {    return targetPtr->onChildRemoved(childView);} }();
}
// onChildVisibilityChanged(KDDockWidgets::Core::View * childView)
void c_KDDockWidgets__flutter__Group__onChildVisibilityChanged_View(void *thisObj, void *childView_)
{
    auto childView = reinterpret_cast<KDDockWidgets::Core::View *>(childView_);
    [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->onChildVisibilityChanged_nocallback(childView);} else {    return targetPtr->onChildVisibilityChanged(childView);} }();
}
// onGeometryChanged()
void c_KDDockWidgets__flutter__Group__onGeometryChanged(void *thisObj)
{
    [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->onGeometryChanged_nocallback();} else {    return targetPtr->onGeometryChanged();} }();
}
// onRebuildRequested()
void c_KDDockWidgets__flutter__Group__onRebuildRequested(void *thisObj)
{
    [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->onRebuildRequested_nocallback();} else {    return targetPtr->onRebuildRequested();} }();
}
// onResize(int h, int w)
bool c_KDDockWidgets__flutter__Group__onResize_int_int(void *thisObj, int h, int w)
{
    return [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->onResize_nocallback(h,w);} else {    return targetPtr->onResize(h,w);} }();
}
// raise()
void c_KDDockWidgets__flutter__Group__raise(void *thisObj)
{
    [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->raise_nocallback();} else {    return targetPtr->raise();} }();
}
// raiseAndActivate()
void c_KDDockWidgets__flutter__Group__raiseAndActivate(void *thisObj)
{
    [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->raiseAndActivate_nocallback();} else {    return targetPtr->raiseAndActivate();} }();
}
// raiseChild(KDDockWidgets::Core::View * childView)
void c_KDDockWidgets__flutter__Group__raiseChild_View(void *thisObj, void *childView_)
{
    auto childView = reinterpret_cast<KDDockWidgets::Core::View *>(childView_);
    [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->raiseChild_nocallback(childView);} else {    return targetPtr->raiseChild(childView);} }();
}
// raiseWindow(KDDockWidgets::Core::View * rootView)
void c_KDDockWidgets__flutter__Group__raiseWindow_View(void *thisObj, void *rootView_)
{
    auto rootView = reinterpret_cast<KDDockWidgets::Core::View *>(rootView_);
    [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->raiseWindow_nocallback(rootView);} else {    return targetPtr->raiseWindow(rootView);} }();
}
// releaseKeyboard()
void c_KDDockWidgets__flutter__Group__releaseKeyboard(void *thisObj)
{
    [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->releaseKeyboard_nocallback();} else {    return targetPtr->releaseKeyboard();} }();
}
// releaseMouse()
void c_KDDockWidgets__flutter__Group__releaseMouse(void *thisObj)
{
    [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->releaseMouse_nocallback();} else {    return targetPtr->releaseMouse();} }();
}
// setCursor(Qt::CursorShape shape)
void c_KDDockWidgets__flutter__Group__setCursor_CursorShape(void *thisObj, int shape)
{
    [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->setCursor_nocallback(static_cast<Qt::CursorShape>(shape));} else {    return targetPtr->setCursor(static_cast<Qt::CursorShape>(shape));} }();
}
// setFixedHeight(int h)
void c_KDDockWidgets__flutter__Group__setFixedHeight_int(void *thisObj, int h)
{
    [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->setFixedHeight_nocallback(h);} else {    return targetPtr->setFixedHeight(h);} }();
}
// setFixedWidth(int w)
void c_KDDockWidgets__flutter__Group__setFixedWidth_int(void *thisObj, int w)
{
    [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->setFixedWidth_nocallback(w);} else {    return targetPtr->setFixedWidth(w);} }();
}
// setGeometry(KDDockWidgets::Rect geometry)
void c_KDDockWidgets__flutter__Group__setGeometry_Rect(void *thisObj, void *geometry_)
{
    assert(geometry_);
    auto &geometry = *reinterpret_cast<KDDockWidgets::Rect *>(geometry_);
    [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->setGeometry_nocallback(geometry);} else {    return targetPtr->setGeometry(geometry);} }();
}
// setHeight(int h)
void c_KDDockWidgets__flutter__Group__setHeight_int(void *thisObj, int h)
{
    [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->setHeight_nocallback(h);} else {    return targetPtr->setHeight(h);} }();
}
// setMaximumSize(KDDockWidgets::Size sz)
void c_KDDockWidgets__flutter__Group__setMaximumSize_Size(void *thisObj, void *sz_)
{
    assert(sz_);
    auto &sz = *reinterpret_cast<KDDockWidgets::Size *>(sz_);
    [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->setMaximumSize_nocallback(sz);} else {    return targetPtr->setMaximumSize(sz);} }();
}
// setMinimumSize(KDDockWidgets::Size sz)
void c_KDDockWidgets__flutter__Group__setMinimumSize_Size(void *thisObj, void *sz_)
{
    assert(sz_);
    auto &sz = *reinterpret_cast<KDDockWidgets::Size *>(sz_);
    [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->setMinimumSize_nocallback(sz);} else {    return targetPtr->setMinimumSize(sz);} }();
}
// setMouseTracking(bool enable)
void c_KDDockWidgets__flutter__Group__setMouseTracking_bool(void *thisObj, bool enable)
{
    [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->setMouseTracking_nocallback(enable);} else {    return targetPtr->setMouseTracking(enable);} }();
}
// setParent(KDDockWidgets::Core::View * parent)
void c_KDDockWidgets__flutter__Group__setParent_View(void *thisObj, void *parent_)
{
    auto parent = reinterpret_cast<KDDockWidgets::Core::View *>(parent_);
    [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->setParent_nocallback(parent);} else {    return targetPtr->setParent(parent);} }();
}
// setSize(int w, int h)
void c_KDDockWidgets__flutter__Group__setSize_int_int(void *thisObj, int w, int h)
{
    [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->setSize_nocallback(w,h);} else {    return targetPtr->setSize(w,h);} }();
}
// setViewName(const QString & name)
void c_KDDockWidgets__flutter__Group__setViewName_QString(void *thisObj, const char *name_)
{
    const auto name = QString::fromUtf8(name_);
    [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->setViewName_nocallback(name);} else {    return targetPtr->setViewName(name);} }();
}
// setVisible(bool visible)
void c_KDDockWidgets__flutter__Group__setVisible_bool(void *thisObj, bool visible)
{
    [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->setVisible_nocallback(visible);} else {    return targetPtr->setVisible(visible);} }();
}
// setWidth(int w)
void c_KDDockWidgets__flutter__Group__setWidth_int(void *thisObj, int w)
{
    [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->setWidth_nocallback(w);} else {    return targetPtr->setWidth(w);} }();
}
// setWindowOpacity(double v)
void c_KDDockWidgets__flutter__Group__setWindowOpacity_double(void *thisObj, double v)
{
    [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->setWindowOpacity_nocallback(v);} else {    return targetPtr->setWindowOpacity(v);} }();
}
// setWindowTitle(const QString & title)
void c_KDDockWidgets__flutter__Group__setWindowTitle_QString(void *thisObj, const char *title_)
{
    const auto title = QString::fromUtf8(title_);
    [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->setWindowTitle_nocallback(title);} else {    return targetPtr->setWindowTitle(title);} }();
}
// setZOrder(int z)
void c_KDDockWidgets__flutter__Group__setZOrder_int(void *thisObj, int z)
{
    [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->setZOrder_nocallback(z);} else {    return targetPtr->setZOrder(z);} }();
}
// show()
void c_KDDockWidgets__flutter__Group__show(void *thisObj)
{
    [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->show_nocallback();} else {    return targetPtr->show();} }();
}
// showMaximized()
void c_KDDockWidgets__flutter__Group__showMaximized(void *thisObj)
{
    [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->showMaximized_nocallback();} else {    return targetPtr->showMaximized();} }();
}
// showMinimized()
void c_KDDockWidgets__flutter__Group__showMinimized(void *thisObj)
{
    [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->showMinimized_nocallback();} else {    return targetPtr->showMinimized();} }();
}
// showNormal()
void c_KDDockWidgets__flutter__Group__showNormal(void *thisObj)
{
    [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->showNormal_nocallback();} else {    return targetPtr->showNormal();} }();
}
// update()
void c_KDDockWidgets__flutter__Group__update(void *thisObj)
{
    [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->update_nocallback();} else {    return targetPtr->update();} }();
}
// viewName() const
void *c_KDDockWidgets__flutter__Group__viewName(void *thisObj)
{
    return new Dartagnan::ValueWrapper<QString> { [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->viewName_nocallback();} else {    return targetPtr->viewName();} }() };
}
void c_KDDockWidgets__flutter__Group__destructor(void *thisObj)
{
    delete fromPtr(thisObj);
}
void c_KDDockWidgets__flutter__Group__registerVirtualMethodCallback(void *ptr, void *callback, int methodId)
{
    auto wrapper = fromWrapperPtr(ptr);
    switch (methodId) {
    case 343:
        wrapper->m_activateWindowCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_activateWindow>(callback);
        break;
    case 353:
        wrapper->m_closeCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_close>(callback);
        break;
    case 355:
        wrapper->m_createPlatformWindowCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_createPlatformWindow>(callback);
        break;
    case 506:
        wrapper->m_dragRectCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_dragRect>(callback);
        break;
    case 361:
        wrapper->m_flagsCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_flags>(callback);
        break;
    case 362:
        wrapper->m_geometryCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_geometry>(callback);
        break;
    case 363:
        wrapper->m_grabMouseCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_grabMouse>(callback);
        break;
    case 366:
        wrapper->m_hasFocusCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_hasFocus>(callback);
        break;
    case 368:
        wrapper->m_hideCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_hide>(callback);
        break;
    case 370:
        wrapper->m_initCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_init>(callback);
        break;
    case 372:
        wrapper->m_isActiveWindowCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_isActiveWindow>(callback);
        break;
    case 373:
        wrapper->m_isExplicitlyHiddenCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_isExplicitlyHidden>(callback);
        break;
    case 374:
        wrapper->m_isMaximizedCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_isMaximized>(callback);
        break;
    case 375:
        wrapper->m_isMinimizedCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_isMinimized>(callback);
        break;
    case 445:
        wrapper->m_isMountedCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_isMounted>(callback);
        break;
    case 376:
        wrapper->m_isNullCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_isNull>(callback);
        break;
    case 377:
        wrapper->m_isRootViewCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_isRootView>(callback);
        break;
    case 378:
        wrapper->m_isVisibleCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_isVisible>(callback);
        break;
    case 379:
        wrapper->m_mapFromGlobalCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_mapFromGlobal>(callback);
        break;
    case 380:
        wrapper->m_mapToCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_mapTo>(callback);
        break;
    case 381:
        wrapper->m_mapToGlobalCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_mapToGlobal>(callback);
        break;
    case 382:
        wrapper->m_maxSizeHintCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_maxSizeHint>(callback);
        break;
    case 383:
        wrapper->m_minSizeCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_minSize>(callback);
        break;
    case 387:
        wrapper->m_move_2Callback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_move_2>(callback);
        break;
    case 507:
        wrapper->m_nonContentsHeightCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_nonContentsHeight>(callback);
        break;
    case 388:
        wrapper->m_normalGeometryCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_normalGeometry>(callback);
        break;
    case 448:
        wrapper->m_onChildAddedCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_onChildAdded>(callback);
        break;
    case 449:
        wrapper->m_onChildRemovedCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_onChildRemoved>(callback);
        break;
    case 450:
        wrapper->m_onChildVisibilityChangedCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_onChildVisibilityChanged>(callback);
        break;
    case 452:
        wrapper->m_onGeometryChangedCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_onGeometryChanged>(callback);
        break;
    case 454:
        wrapper->m_onRebuildRequestedCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_onRebuildRequested>(callback);
        break;
    case 390:
        wrapper->m_onResize_2Callback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_onResize_2>(callback);
        break;
    case 392:
        wrapper->m_raiseCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_raise>(callback);
        break;
    case 393:
        wrapper->m_raiseAndActivateCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_raiseAndActivate>(callback);
        break;
    case 457:
        wrapper->m_raiseChildCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_raiseChild>(callback);
        break;
    case 458:
        wrapper->m_raiseWindowCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_raiseWindow>(callback);
        break;
    case 395:
        wrapper->m_releaseKeyboardCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_releaseKeyboard>(callback);
        break;
    case 396:
        wrapper->m_releaseMouseCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_releaseMouse>(callback);
        break;
    case 399:
        wrapper->m_setCursorCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_setCursor>(callback);
        break;
    case 400:
        wrapper->m_setFixedHeightCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_setFixedHeight>(callback);
        break;
    case 401:
        wrapper->m_setFixedWidthCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_setFixedWidth>(callback);
        break;
    case 402:
        wrapper->m_setGeometryCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_setGeometry>(callback);
        break;
    case 403:
        wrapper->m_setHeightCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_setHeight>(callback);
        break;
    case 404:
        wrapper->m_setMaximumSizeCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_setMaximumSize>(callback);
        break;
    case 405:
        wrapper->m_setMinimumSizeCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_setMinimumSize>(callback);
        break;
    case 406:
        wrapper->m_setMouseTrackingCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_setMouseTracking>(callback);
        break;
    case 407:
        wrapper->m_setParentCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_setParent>(callback);
        break;
    case 409:
        wrapper->m_setSize_2Callback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_setSize_2>(callback);
        break;
    case 410:
        wrapper->m_setViewNameCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_setViewName>(callback);
        break;
    case 411:
        wrapper->m_setVisibleCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_setVisible>(callback);
        break;
    case 412:
        wrapper->m_setWidthCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_setWidth>(callback);
        break;
    case 413:
        wrapper->m_setWindowOpacityCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_setWindowOpacity>(callback);
        break;
    case 414:
        wrapper->m_setWindowTitleCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_setWindowTitle>(callback);
        break;
    case 415:
        wrapper->m_setZOrderCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_setZOrder>(callback);
        break;
    case 416:
        wrapper->m_showCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_show>(callback);
        break;
    case 417:
        wrapper->m_showMaximizedCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_showMaximized>(callback);
        break;
    case 418:
        wrapper->m_showMinimizedCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_showMinimized>(callback);
        break;
    case 419:
        wrapper->m_showNormalCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_showNormal>(callback);
        break;
    case 421:
        wrapper->m_updateCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_update>(callback);
        break;
    case 422:
        wrapper->m_viewNameCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsFlutter::Group_wrapper::Callback_viewName>(callback);
        break;
    }
}
}
