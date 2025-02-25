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
namespace KDDWBindingsCore {
Group_wrapper::Group_wrapper(KDDockWidgets::Core::View *parent)
    : ::KDDockWidgets::Core::Group(parent)
{
}
KDDockWidgets::Core::TitleBar *Group_wrapper::actualTitleBar() const
{
    return ::KDDockWidgets::Core::Group::actualTitleBar();
}
void Group_wrapper::addTab(KDDockWidgets::Core::DockWidget *arg__1, KDDockWidgets::InitialOption arg__2)
{
    ::KDDockWidgets::Core::Group::addTab(arg__1, arg__2);
}
void Group_wrapper::addTab(KDDockWidgets::Core::FloatingWindow *floatingWindow, KDDockWidgets::InitialOption arg__2)
{
    ::KDDockWidgets::Core::Group::addTab(floatingWindow, arg__2);
}
void Group_wrapper::addTab(KDDockWidgets::Core::Group *arg__1, KDDockWidgets::InitialOption arg__2)
{
    ::KDDockWidgets::Core::Group::addTab(arg__1, arg__2);
}
bool Group_wrapper::allDockWidgetsHave(KDDockWidgets::DockWidgetOption arg__1) const
{
    return ::KDDockWidgets::Core::Group::allDockWidgetsHave(arg__1);
}
bool Group_wrapper::allDockWidgetsHave(KDDockWidgets::LayoutSaverOption arg__1) const
{
    return ::KDDockWidgets::Core::Group::allDockWidgetsHave(arg__1);
}
bool Group_wrapper::alwaysShowsTabs() const
{
    return ::KDDockWidgets::Core::Group::alwaysShowsTabs();
}
bool Group_wrapper::anyDockWidgetsHas(KDDockWidgets::DockWidgetOption arg__1) const
{
    return ::KDDockWidgets::Core::Group::anyDockWidgetsHas(arg__1);
}
bool Group_wrapper::anyDockWidgetsHas(KDDockWidgets::LayoutSaverOption arg__1) const
{
    return ::KDDockWidgets::Core::Group::anyDockWidgetsHas(arg__1);
}
bool Group_wrapper::anyNonClosable() const
{
    return ::KDDockWidgets::Core::Group::anyNonClosable();
}
bool Group_wrapper::anyNonDockable() const
{
    return ::KDDockWidgets::Core::Group::anyNonDockable();
}
bool Group_wrapper::beingDeletedLater() const
{
    return ::KDDockWidgets::Core::Group::beingDeletedLater();
}
KDDockWidgets::Size Group_wrapper::biggestDockWidgetMaxSize() const
{
    return ::KDDockWidgets::Core::Group::biggestDockWidgetMaxSize();
}
bool Group_wrapper::containsDockWidget(KDDockWidgets::Core::DockWidget *w) const
{
    return ::KDDockWidgets::Core::Group::containsDockWidget(w);
}
bool Group_wrapper::containsMouse(KDDockWidgets::Point globalPos) const
{
    return ::KDDockWidgets::Core::Group::containsMouse(globalPos);
}
KDDockWidgets::Core::DockWidget *Group_wrapper::currentDockWidget() const
{
    return ::KDDockWidgets::Core::Group::currentDockWidget();
}
int Group_wrapper::currentIndex() const
{
    return ::KDDockWidgets::Core::Group::currentIndex();
}
int Group_wrapper::currentTabIndex() const
{
    return ::KDDockWidgets::Core::Group::currentTabIndex();
}
int Group_wrapper::dbg_numFrames()
{
    return ::KDDockWidgets::Core::Group::dbg_numFrames();
}
KDDockWidgets::Core::FloatingWindow *Group_wrapper::detachTab(KDDockWidgets::Core::DockWidget *arg__1)
{
    return ::KDDockWidgets::Core::Group::detachTab(arg__1);
}
KDDockWidgets::Core::DockWidget *Group_wrapper::dockWidgetAt(int index) const
{
    return ::KDDockWidgets::Core::Group::dockWidgetAt(index);
}
int Group_wrapper::dockWidgetCount() const
{
    return ::KDDockWidgets::Core::Group::dockWidgetCount();
}
KDDockWidgets::Size Group_wrapper::dockWidgetsMinSize() const
{
    return ::KDDockWidgets::Core::Group::dockWidgetsMinSize();
}
KDDockWidgets::Rect Group_wrapper::dragRect() const
{
    if (m_dragRectCallback) {
        const void *thisPtr = this;
        return *m_dragRectCallback(const_cast<void *>(thisPtr));
    } else {
        return ::KDDockWidgets::Core::Group::dragRect();
    }
}
KDDockWidgets::Rect Group_wrapper::dragRect_nocallback() const
{
    return ::KDDockWidgets::Core::Group::dragRect();
}
KDDockWidgets::Core::FloatingWindow *Group_wrapper::floatingWindow() const
{
    return ::KDDockWidgets::Core::Group::floatingWindow();
}
void Group_wrapper::focusedWidgetChangedCallback()
{
    if (m_focusedWidgetChangedCallbackCallback) {
        const void *thisPtr = this;
        m_focusedWidgetChangedCallbackCallback(const_cast<void *>(thisPtr));
    } else {
        ::KDDockWidgets::Core::Group::focusedWidgetChangedCallback();
    }
}
void Group_wrapper::focusedWidgetChangedCallback_nocallback()
{
    ::KDDockWidgets::Core::Group::focusedWidgetChangedCallback();
}
bool Group_wrapper::hasNestedMDIDockWidgets() const
{
    return ::KDDockWidgets::Core::Group::hasNestedMDIDockWidgets();
}
bool Group_wrapper::hasSingleDockWidget() const
{
    return ::KDDockWidgets::Core::Group::hasSingleDockWidget();
}
bool Group_wrapper::hasTabsVisible() const
{
    return ::KDDockWidgets::Core::Group::hasTabsVisible();
}
int Group_wrapper::indexOfDockWidget(const KDDockWidgets::Core::DockWidget *arg__1)
{
    return ::KDDockWidgets::Core::Group::indexOfDockWidget(arg__1);
}
void Group_wrapper::insertDockWidget(KDDockWidgets::Core::DockWidget *arg__1, int index)
{
    ::KDDockWidgets::Core::Group::insertDockWidget(arg__1, index);
}
void Group_wrapper::insertWidget(KDDockWidgets::Core::DockWidget *arg__1, int index, KDDockWidgets::InitialOption arg__3)
{
    ::KDDockWidgets::Core::Group::insertWidget(arg__1, index, arg__3);
}
bool Group_wrapper::isCentralFrame() const
{
    return ::KDDockWidgets::Core::Group::isCentralFrame();
}
bool Group_wrapper::isDockable() const
{
    return ::KDDockWidgets::Core::Group::isDockable();
}
bool Group_wrapper::isEmpty() const
{
    return ::KDDockWidgets::Core::Group::isEmpty();
}
bool Group_wrapper::isFloating() const
{
    return ::KDDockWidgets::Core::Group::isFloating();
}
void Group_wrapper::isFocusedChangedCallback()
{
    if (m_isFocusedChangedCallbackCallback) {
        const void *thisPtr = this;
        m_isFocusedChangedCallbackCallback(const_cast<void *>(thisPtr));
    } else {
        ::KDDockWidgets::Core::Group::isFocusedChangedCallback();
    }
}
void Group_wrapper::isFocusedChangedCallback_nocallback()
{
    ::KDDockWidgets::Core::Group::isFocusedChangedCallback();
}
bool Group_wrapper::isInFloatingWindow() const
{
    return ::KDDockWidgets::Core::Group::isInFloatingWindow();
}
bool Group_wrapper::isInMainWindow() const
{
    return ::KDDockWidgets::Core::Group::isInMainWindow();
}
bool Group_wrapper::isMDI() const
{
    return ::KDDockWidgets::Core::Group::isMDI();
}
bool Group_wrapper::isMDIWrapper() const
{
    return ::KDDockWidgets::Core::Group::isMDIWrapper();
}
bool Group_wrapper::isOverlayed() const
{
    return ::KDDockWidgets::Core::Group::isOverlayed();
}
bool Group_wrapper::isTheOnlyGroup() const
{
    return ::KDDockWidgets::Core::Group::isTheOnlyGroup();
}
KDDockWidgets::Core::Item *Group_wrapper::layoutItem() const
{
    return ::KDDockWidgets::Core::Group::layoutItem();
}
KDDockWidgets::Core::MainWindow *Group_wrapper::mainWindow() const
{
    return ::KDDockWidgets::Core::Group::mainWindow();
}
KDDockWidgets::Core::DockWidget *Group_wrapper::mdiDockWidgetWrapper() const
{
    return ::KDDockWidgets::Core::Group::mdiDockWidgetWrapper();
}
KDDockWidgets::Core::DropArea *Group_wrapper::mdiDropAreaWrapper() const
{
    return ::KDDockWidgets::Core::Group::mdiDropAreaWrapper();
}
KDDockWidgets::Core::Group *Group_wrapper::mdiFrame() const
{
    return ::KDDockWidgets::Core::Group::mdiFrame();
}
int Group_wrapper::nonContentsHeight() const
{
    return ::KDDockWidgets::Core::Group::nonContentsHeight();
}
void Group_wrapper::onDockWidgetCountChanged()
{
    ::KDDockWidgets::Core::Group::onDockWidgetCountChanged();
}
void Group_wrapper::onDockWidgetTitleChanged(KDDockWidgets::Core::DockWidget *arg__1)
{
    ::KDDockWidgets::Core::Group::onDockWidgetTitleChanged(arg__1);
}
void Group_wrapper::removeWidget(KDDockWidgets::Core::DockWidget *arg__1)
{
    ::KDDockWidgets::Core::Group::removeWidget(arg__1);
}
void Group_wrapper::renameTab(int index, const QString &arg__2)
{
    ::KDDockWidgets::Core::Group::renameTab(index, arg__2);
}
void Group_wrapper::restoreToPreviousPosition()
{
    ::KDDockWidgets::Core::Group::restoreToPreviousPosition();
}
void Group_wrapper::setCurrentDockWidget(KDDockWidgets::Core::DockWidget *arg__1)
{
    ::KDDockWidgets::Core::Group::setCurrentDockWidget(arg__1);
}
void Group_wrapper::setCurrentTabIndex(int index)
{
    ::KDDockWidgets::Core::Group::setCurrentTabIndex(index);
}
void Group_wrapper::setLayout(KDDockWidgets::Core::Layout *arg__1)
{
    ::KDDockWidgets::Core::Group::setLayout(arg__1);
}
void Group_wrapper::setLayoutItem(KDDockWidgets::Core::Item *item)
{
    ::KDDockWidgets::Core::Group::setLayoutItem(item);
}
void Group_wrapper::setParentView_impl(KDDockWidgets::Core::View *parent)
{
    if (m_setParentView_implCallback) {
        const void *thisPtr = this;
        m_setParentView_implCallback(const_cast<void *>(thisPtr), parent);
    } else {
        ::KDDockWidgets::Core::Group::setParentView_impl(parent);
    }
}
void Group_wrapper::setParentView_impl_nocallback(KDDockWidgets::Core::View *parent)
{
    ::KDDockWidgets::Core::Group::setParentView_impl(parent);
}
KDDockWidgets::Core::Stack *Group_wrapper::stack() const
{
    return ::KDDockWidgets::Core::Group::stack();
}
KDDockWidgets::Core::TabBar *Group_wrapper::tabBar() const
{
    return ::KDDockWidgets::Core::Group::tabBar();
}
QString Group_wrapper::title() const
{
    return ::KDDockWidgets::Core::Group::title();
}
KDDockWidgets::Core::TitleBar *Group_wrapper::titleBar() const
{
    return ::KDDockWidgets::Core::Group::titleBar();
}
void Group_wrapper::unoverlay()
{
    ::KDDockWidgets::Core::Group::unoverlay();
}
void Group_wrapper::updateFloatingActions()
{
    ::KDDockWidgets::Core::Group::updateFloatingActions();
}
void Group_wrapper::updateTitleAndIcon()
{
    ::KDDockWidgets::Core::Group::updateTitleAndIcon();
}
void Group_wrapper::updateTitleBarVisibility()
{
    ::KDDockWidgets::Core::Group::updateTitleBarVisibility();
}
int Group_wrapper::userType() const
{
    return ::KDDockWidgets::Core::Group::userType();
}
Group_wrapper::~Group_wrapper()
{
}

}
}
static KDDockWidgets::Core::Group *fromPtr(void *ptr)
{
    return reinterpret_cast<KDDockWidgets::Core::Group *>(ptr);
}
static KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::Group_wrapper *fromWrapperPtr(void *ptr)
{
    return reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::Group_wrapper *>(ptr);
}
extern "C" {
void c_KDDockWidgets__Core__Group_Finalizer(void *cppObj)
{
    delete reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::Group_wrapper *>(cppObj);
}
void *c_KDDockWidgets__Core__Group__constructor_View(void *parent_)
{
    auto parent = reinterpret_cast<KDDockWidgets::Core::View *>(parent_);
    auto ptr = new KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::Group_wrapper(parent);
    return reinterpret_cast<void *>(ptr);
}
// actualTitleBar() const
void *c_KDDockWidgets__Core__Group__actualTitleBar(void *thisObj)
{
    return fromPtr(thisObj)->actualTitleBar();
}
// addTab(KDDockWidgets::Core::DockWidget * arg__1, KDDockWidgets::InitialOption arg__2)
void c_KDDockWidgets__Core__Group__addTab_DockWidget_InitialOption(void *thisObj, void *arg__1_, void *arg__2_)
{
    auto arg__1 = reinterpret_cast<KDDockWidgets::Core::DockWidget *>(arg__1_);
    assert(arg__2_);
    auto &arg__2 = *reinterpret_cast<KDDockWidgets::InitialOption *>(arg__2_);
    fromPtr(thisObj)->addTab(arg__1, arg__2);
}
// addTab(KDDockWidgets::Core::FloatingWindow * floatingWindow, KDDockWidgets::InitialOption arg__2)
void c_KDDockWidgets__Core__Group__addTab_FloatingWindow_InitialOption(void *thisObj, void *floatingWindow_, void *arg__2_)
{
    auto floatingWindow = reinterpret_cast<KDDockWidgets::Core::FloatingWindow *>(floatingWindow_);
    assert(arg__2_);
    auto &arg__2 = *reinterpret_cast<KDDockWidgets::InitialOption *>(arg__2_);
    fromPtr(thisObj)->addTab(floatingWindow, arg__2);
}
// addTab(KDDockWidgets::Core::Group * arg__1, KDDockWidgets::InitialOption arg__2)
void c_KDDockWidgets__Core__Group__addTab_Group_InitialOption(void *thisObj, void *arg__1_, void *arg__2_)
{
    auto arg__1 = reinterpret_cast<KDDockWidgets::Core::Group *>(arg__1_);
    assert(arg__2_);
    auto &arg__2 = *reinterpret_cast<KDDockWidgets::InitialOption *>(arg__2_);
    fromPtr(thisObj)->addTab(arg__1, arg__2);
}
// allDockWidgetsHave(KDDockWidgets::DockWidgetOption arg__1) const
bool c_KDDockWidgets__Core__Group__allDockWidgetsHave_DockWidgetOption(void *thisObj, int arg__1)
{
    return fromPtr(thisObj)->allDockWidgetsHave(static_cast<KDDockWidgets::DockWidgetOption>(arg__1));
}
// allDockWidgetsHave(KDDockWidgets::LayoutSaverOption arg__1) const
bool c_KDDockWidgets__Core__Group__allDockWidgetsHave_LayoutSaverOption(void *thisObj, int arg__1)
{
    return fromPtr(thisObj)->allDockWidgetsHave(static_cast<KDDockWidgets::LayoutSaverOption>(arg__1));
}
// alwaysShowsTabs() const
bool c_KDDockWidgets__Core__Group__alwaysShowsTabs(void *thisObj)
{
    return fromPtr(thisObj)->alwaysShowsTabs();
}
// anyDockWidgetsHas(KDDockWidgets::DockWidgetOption arg__1) const
bool c_KDDockWidgets__Core__Group__anyDockWidgetsHas_DockWidgetOption(void *thisObj, int arg__1)
{
    return fromPtr(thisObj)->anyDockWidgetsHas(static_cast<KDDockWidgets::DockWidgetOption>(arg__1));
}
// anyDockWidgetsHas(KDDockWidgets::LayoutSaverOption arg__1) const
bool c_KDDockWidgets__Core__Group__anyDockWidgetsHas_LayoutSaverOption(void *thisObj, int arg__1)
{
    return fromPtr(thisObj)->anyDockWidgetsHas(static_cast<KDDockWidgets::LayoutSaverOption>(arg__1));
}
// anyNonClosable() const
bool c_KDDockWidgets__Core__Group__anyNonClosable(void *thisObj)
{
    return fromPtr(thisObj)->anyNonClosable();
}
// anyNonDockable() const
bool c_KDDockWidgets__Core__Group__anyNonDockable(void *thisObj)
{
    return fromPtr(thisObj)->anyNonDockable();
}
// beingDeletedLater() const
bool c_KDDockWidgets__Core__Group__beingDeletedLater(void *thisObj)
{
    return fromPtr(thisObj)->beingDeletedLater();
}
// biggestDockWidgetMaxSize() const
void *c_KDDockWidgets__Core__Group__biggestDockWidgetMaxSize(void *thisObj)
{
    return new Dartagnan::ValueWrapper<KDDockWidgets::Size> { fromPtr(thisObj)->biggestDockWidgetMaxSize() };
}
// containsDockWidget(KDDockWidgets::Core::DockWidget * w) const
bool c_KDDockWidgets__Core__Group__containsDockWidget_DockWidget(void *thisObj, void *w_)
{
    auto w = reinterpret_cast<KDDockWidgets::Core::DockWidget *>(w_);
    return fromPtr(thisObj)->containsDockWidget(w);
}
// containsMouse(KDDockWidgets::Point globalPos) const
bool c_KDDockWidgets__Core__Group__containsMouse_Point(void *thisObj, void *globalPos_)
{
    assert(globalPos_);
    auto &globalPos = *reinterpret_cast<KDDockWidgets::Point *>(globalPos_);
    return fromPtr(thisObj)->containsMouse(globalPos);
}
// currentDockWidget() const
void *c_KDDockWidgets__Core__Group__currentDockWidget(void *thisObj)
{
    return fromPtr(thisObj)->currentDockWidget();
}
// currentIndex() const
int c_KDDockWidgets__Core__Group__currentIndex(void *thisObj)
{
    return fromPtr(thisObj)->currentIndex();
}
// currentTabIndex() const
int c_KDDockWidgets__Core__Group__currentTabIndex(void *thisObj)
{
    return fromPtr(thisObj)->currentTabIndex();
}
// dbg_numFrames()
int c_static_KDDockWidgets__Core__Group__dbg_numFrames()
{
    return KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::Group_wrapper::dbg_numFrames();
}
// detachTab(KDDockWidgets::Core::DockWidget * arg__1)
void *c_KDDockWidgets__Core__Group__detachTab_DockWidget(void *thisObj, void *arg__1_)
{
    auto arg__1 = reinterpret_cast<KDDockWidgets::Core::DockWidget *>(arg__1_);
    return fromPtr(thisObj)->detachTab(arg__1);
}
// dockWidgetAt(int index) const
void *c_KDDockWidgets__Core__Group__dockWidgetAt_int(void *thisObj, int index)
{
    return fromPtr(thisObj)->dockWidgetAt(index);
}
// dockWidgetCount() const
int c_KDDockWidgets__Core__Group__dockWidgetCount(void *thisObj)
{
    return fromPtr(thisObj)->dockWidgetCount();
}
// dockWidgetsMinSize() const
void *c_KDDockWidgets__Core__Group__dockWidgetsMinSize(void *thisObj)
{
    return new Dartagnan::ValueWrapper<KDDockWidgets::Size> { fromPtr(thisObj)->dockWidgetsMinSize() };
}
// dragRect() const
void *c_KDDockWidgets__Core__Group__dragRect(void *thisObj)
{
    return new Dartagnan::ValueWrapper<KDDockWidgets::Rect> { [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::Group_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->dragRect_nocallback();} else {    return targetPtr->dragRect();} }() };
}
// floatingWindow() const
void *c_KDDockWidgets__Core__Group__floatingWindow(void *thisObj)
{
    return fromPtr(thisObj)->floatingWindow();
}
// focusedWidgetChangedCallback()
void c_KDDockWidgets__Core__Group__focusedWidgetChangedCallback(void *thisObj)
{
    fromWrapperPtr(thisObj)->focusedWidgetChangedCallback_nocallback();
}
// hasNestedMDIDockWidgets() const
bool c_KDDockWidgets__Core__Group__hasNestedMDIDockWidgets(void *thisObj)
{
    return fromPtr(thisObj)->hasNestedMDIDockWidgets();
}
// hasSingleDockWidget() const
bool c_KDDockWidgets__Core__Group__hasSingleDockWidget(void *thisObj)
{
    return fromPtr(thisObj)->hasSingleDockWidget();
}
// hasTabsVisible() const
bool c_KDDockWidgets__Core__Group__hasTabsVisible(void *thisObj)
{
    return fromPtr(thisObj)->hasTabsVisible();
}
// indexOfDockWidget(const KDDockWidgets::Core::DockWidget * arg__1)
int c_KDDockWidgets__Core__Group__indexOfDockWidget_DockWidget(void *thisObj, void *arg__1_)
{
    auto arg__1 = reinterpret_cast<KDDockWidgets::Core::DockWidget *>(arg__1_);
    return fromPtr(thisObj)->indexOfDockWidget(arg__1);
}
// insertDockWidget(KDDockWidgets::Core::DockWidget * arg__1, int index)
void c_KDDockWidgets__Core__Group__insertDockWidget_DockWidget_int(void *thisObj, void *arg__1_, int index)
{
    auto arg__1 = reinterpret_cast<KDDockWidgets::Core::DockWidget *>(arg__1_);
    fromPtr(thisObj)->insertDockWidget(arg__1, index);
}
// insertWidget(KDDockWidgets::Core::DockWidget * arg__1, int index, KDDockWidgets::InitialOption arg__3)
void c_KDDockWidgets__Core__Group__insertWidget_DockWidget_int_InitialOption(void *thisObj, void *arg__1_, int index, void *arg__3_)
{
    auto arg__1 = reinterpret_cast<KDDockWidgets::Core::DockWidget *>(arg__1_);
    assert(arg__3_);
    auto &arg__3 = *reinterpret_cast<KDDockWidgets::InitialOption *>(arg__3_);
    fromPtr(thisObj)->insertWidget(arg__1, index, arg__3);
}
// isCentralFrame() const
bool c_KDDockWidgets__Core__Group__isCentralFrame(void *thisObj)
{
    return fromPtr(thisObj)->isCentralFrame();
}
// isDockable() const
bool c_KDDockWidgets__Core__Group__isDockable(void *thisObj)
{
    return fromPtr(thisObj)->isDockable();
}
// isEmpty() const
bool c_KDDockWidgets__Core__Group__isEmpty(void *thisObj)
{
    return fromPtr(thisObj)->isEmpty();
}
// isFloating() const
bool c_KDDockWidgets__Core__Group__isFloating(void *thisObj)
{
    return fromPtr(thisObj)->isFloating();
}
// isFocusedChangedCallback()
void c_KDDockWidgets__Core__Group__isFocusedChangedCallback(void *thisObj)
{
    fromWrapperPtr(thisObj)->isFocusedChangedCallback_nocallback();
}
// isInFloatingWindow() const
bool c_KDDockWidgets__Core__Group__isInFloatingWindow(void *thisObj)
{
    return fromPtr(thisObj)->isInFloatingWindow();
}
// isInMainWindow() const
bool c_KDDockWidgets__Core__Group__isInMainWindow(void *thisObj)
{
    return fromPtr(thisObj)->isInMainWindow();
}
// isMDI() const
bool c_KDDockWidgets__Core__Group__isMDI(void *thisObj)
{
    return fromPtr(thisObj)->isMDI();
}
// isMDIWrapper() const
bool c_KDDockWidgets__Core__Group__isMDIWrapper(void *thisObj)
{
    return fromPtr(thisObj)->isMDIWrapper();
}
// isOverlayed() const
bool c_KDDockWidgets__Core__Group__isOverlayed(void *thisObj)
{
    return fromPtr(thisObj)->isOverlayed();
}
// isTheOnlyGroup() const
bool c_KDDockWidgets__Core__Group__isTheOnlyGroup(void *thisObj)
{
    return fromPtr(thisObj)->isTheOnlyGroup();
}
// layoutItem() const
void *c_KDDockWidgets__Core__Group__layoutItem(void *thisObj)
{
    return fromPtr(thisObj)->layoutItem();
}
// mainWindow() const
void *c_KDDockWidgets__Core__Group__mainWindow(void *thisObj)
{
    return fromPtr(thisObj)->mainWindow();
}
// mdiDockWidgetWrapper() const
void *c_KDDockWidgets__Core__Group__mdiDockWidgetWrapper(void *thisObj)
{
    return fromPtr(thisObj)->mdiDockWidgetWrapper();
}
// mdiDropAreaWrapper() const
void *c_KDDockWidgets__Core__Group__mdiDropAreaWrapper(void *thisObj)
{
    return fromPtr(thisObj)->mdiDropAreaWrapper();
}
// mdiFrame() const
void *c_KDDockWidgets__Core__Group__mdiFrame(void *thisObj)
{
    return fromPtr(thisObj)->mdiFrame();
}
// nonContentsHeight() const
int c_KDDockWidgets__Core__Group__nonContentsHeight(void *thisObj)
{
    return fromPtr(thisObj)->nonContentsHeight();
}
// onDockWidgetCountChanged()
void c_KDDockWidgets__Core__Group__onDockWidgetCountChanged(void *thisObj)
{
    fromPtr(thisObj)->onDockWidgetCountChanged();
}
// onDockWidgetTitleChanged(KDDockWidgets::Core::DockWidget * arg__1)
void c_KDDockWidgets__Core__Group__onDockWidgetTitleChanged_DockWidget(void *thisObj, void *arg__1_)
{
    auto arg__1 = reinterpret_cast<KDDockWidgets::Core::DockWidget *>(arg__1_);
    fromPtr(thisObj)->onDockWidgetTitleChanged(arg__1);
}
// removeWidget(KDDockWidgets::Core::DockWidget * arg__1)
void c_KDDockWidgets__Core__Group__removeWidget_DockWidget(void *thisObj, void *arg__1_)
{
    auto arg__1 = reinterpret_cast<KDDockWidgets::Core::DockWidget *>(arg__1_);
    fromPtr(thisObj)->removeWidget(arg__1);
}
// renameTab(int index, const QString & arg__2)
void c_KDDockWidgets__Core__Group__renameTab_int_QString(void *thisObj, int index, const char *arg__2_)
{
    const auto arg__2 = QString::fromUtf8(arg__2_);
    fromPtr(thisObj)->renameTab(index, arg__2);
}
// restoreToPreviousPosition()
void c_KDDockWidgets__Core__Group__restoreToPreviousPosition(void *thisObj)
{
    fromPtr(thisObj)->restoreToPreviousPosition();
}
// setCurrentDockWidget(KDDockWidgets::Core::DockWidget * arg__1)
void c_KDDockWidgets__Core__Group__setCurrentDockWidget_DockWidget(void *thisObj, void *arg__1_)
{
    auto arg__1 = reinterpret_cast<KDDockWidgets::Core::DockWidget *>(arg__1_);
    fromPtr(thisObj)->setCurrentDockWidget(arg__1);
}
// setCurrentTabIndex(int index)
void c_KDDockWidgets__Core__Group__setCurrentTabIndex_int(void *thisObj, int index)
{
    fromPtr(thisObj)->setCurrentTabIndex(index);
}
// setLayout(KDDockWidgets::Core::Layout * arg__1)
void c_KDDockWidgets__Core__Group__setLayout_Layout(void *thisObj, void *arg__1_)
{
    auto arg__1 = reinterpret_cast<KDDockWidgets::Core::Layout *>(arg__1_);
    fromPtr(thisObj)->setLayout(arg__1);
}
// setLayoutItem(KDDockWidgets::Core::Item * item)
void c_KDDockWidgets__Core__Group__setLayoutItem_Item(void *thisObj, void *item_)
{
    auto item = reinterpret_cast<KDDockWidgets::Core::Item *>(item_);
    fromPtr(thisObj)->setLayoutItem(item);
}
// setParentView_impl(KDDockWidgets::Core::View * parent)
void c_KDDockWidgets__Core__Group__setParentView_impl_View(void *thisObj, void *parent_)
{
    auto parent = reinterpret_cast<KDDockWidgets::Core::View *>(parent_);
    fromWrapperPtr(thisObj)->setParentView_impl_nocallback(parent);
}
// stack() const
void *c_KDDockWidgets__Core__Group__stack(void *thisObj)
{
    return fromPtr(thisObj)->stack();
}
// tabBar() const
void *c_KDDockWidgets__Core__Group__tabBar(void *thisObj)
{
    return fromPtr(thisObj)->tabBar();
}
// title() const
void *c_KDDockWidgets__Core__Group__title(void *thisObj)
{
    return new Dartagnan::ValueWrapper<QString> { fromPtr(thisObj)->title() };
}
// titleBar() const
void *c_KDDockWidgets__Core__Group__titleBar(void *thisObj)
{
    return fromPtr(thisObj)->titleBar();
}
// unoverlay()
void c_KDDockWidgets__Core__Group__unoverlay(void *thisObj)
{
    fromPtr(thisObj)->unoverlay();
}
// updateFloatingActions()
void c_KDDockWidgets__Core__Group__updateFloatingActions(void *thisObj)
{
    fromPtr(thisObj)->updateFloatingActions();
}
// updateTitleAndIcon()
void c_KDDockWidgets__Core__Group__updateTitleAndIcon(void *thisObj)
{
    fromPtr(thisObj)->updateTitleAndIcon();
}
// updateTitleBarVisibility()
void c_KDDockWidgets__Core__Group__updateTitleBarVisibility(void *thisObj)
{
    fromPtr(thisObj)->updateTitleBarVisibility();
}
// userType() const
int c_KDDockWidgets__Core__Group__userType(void *thisObj)
{
    return fromPtr(thisObj)->userType();
}
void c_KDDockWidgets__Core__Group__destructor(void *thisObj)
{
    delete fromPtr(thisObj);
}
void c_KDDockWidgets__Core__Group__registerVirtualMethodCallback(void *ptr, void *callback, int methodId)
{
    auto wrapper = fromWrapperPtr(ptr);
    switch (methodId) {
    case 891:
        wrapper->m_dragRectCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::Group_wrapper::Callback_dragRect>(callback);
        break;
    case 893:
        wrapper->m_focusedWidgetChangedCallbackCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::Group_wrapper::Callback_focusedWidgetChangedCallback>(callback);
        break;
    case 908:
        wrapper->m_isFocusedChangedCallbackCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::Group_wrapper::Callback_isFocusedChangedCallback>(callback);
        break;
    case 332:
        wrapper->m_setParentView_implCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::Group_wrapper::Callback_setParentView_impl>(callback);
        break;
    }
}
}
