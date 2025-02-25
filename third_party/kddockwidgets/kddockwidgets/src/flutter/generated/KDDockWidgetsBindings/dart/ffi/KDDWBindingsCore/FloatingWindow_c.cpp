/*
  This file is part of KDDockWidgets.

  SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
  Author: Sérgio Martins <sergio.martins@kdab.com>

  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/
#include "FloatingWindow_c.h"


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
FloatingWindow_wrapper::FloatingWindow_wrapper(KDDockWidgets::Core::Group *group, KDDockWidgets::Rect suggestedGeometry, KDDockWidgets::Core::MainWindow *parent)
    : ::KDDockWidgets::Core::FloatingWindow(group, suggestedGeometry, parent)
{
}
FloatingWindow_wrapper::FloatingWindow_wrapper(KDDockWidgets::Rect suggestedGeometry, KDDockWidgets::Core::MainWindow *parent)
    : ::KDDockWidgets::Core::FloatingWindow(suggestedGeometry, parent)
{
}
void FloatingWindow_wrapper::addDockWidget(KDDockWidgets::Core::DockWidget *arg__1, KDDockWidgets::Location location, KDDockWidgets::Core::DockWidget *relativeTo, KDDockWidgets::InitialOption arg__4)
{
    ::KDDockWidgets::Core::FloatingWindow::addDockWidget(arg__1, location, relativeTo, arg__4);
}
bool FloatingWindow_wrapper::allDockWidgetsHave(KDDockWidgets::DockWidgetOption arg__1) const
{
    return ::KDDockWidgets::Core::FloatingWindow::allDockWidgetsHave(arg__1);
}
bool FloatingWindow_wrapper::allDockWidgetsHave(KDDockWidgets::LayoutSaverOption arg__1) const
{
    return ::KDDockWidgets::Core::FloatingWindow::allDockWidgetsHave(arg__1);
}
bool FloatingWindow_wrapper::anyDockWidgetsHas(KDDockWidgets::DockWidgetOption arg__1) const
{
    return ::KDDockWidgets::Core::FloatingWindow::anyDockWidgetsHas(arg__1);
}
bool FloatingWindow_wrapper::anyDockWidgetsHas(KDDockWidgets::LayoutSaverOption arg__1) const
{
    return ::KDDockWidgets::Core::FloatingWindow::anyDockWidgetsHas(arg__1);
}
bool FloatingWindow_wrapper::anyNonClosable() const
{
    return ::KDDockWidgets::Core::FloatingWindow::anyNonClosable();
}
bool FloatingWindow_wrapper::anyNonDockable() const
{
    return ::KDDockWidgets::Core::FloatingWindow::anyNonDockable();
}
bool FloatingWindow_wrapper::beingDeleted() const
{
    return ::KDDockWidgets::Core::FloatingWindow::beingDeleted();
}
KDDockWidgets::Margins FloatingWindow_wrapper::contentMargins() const
{
    return ::KDDockWidgets::Core::FloatingWindow::contentMargins();
}
KDDockWidgets::Rect FloatingWindow_wrapper::dragRect() const
{
    return ::KDDockWidgets::Core::FloatingWindow::dragRect();
}
KDDockWidgets::Core::DropArea *FloatingWindow_wrapper::dropArea() const
{
    return ::KDDockWidgets::Core::FloatingWindow::dropArea();
}
void FloatingWindow_wrapper::ensureRectIsOnScreen(KDDockWidgets::Rect &geometry)
{
    ::KDDockWidgets::Core::FloatingWindow::ensureRectIsOnScreen(geometry);
}
bool FloatingWindow_wrapper::hasSingleDockWidget() const
{
    return ::KDDockWidgets::Core::FloatingWindow::hasSingleDockWidget();
}
bool FloatingWindow_wrapper::hasSingleFrame() const
{
    return ::KDDockWidgets::Core::FloatingWindow::hasSingleFrame();
}
bool FloatingWindow_wrapper::isInDragArea(KDDockWidgets::Point globalPoint) const
{
    return ::KDDockWidgets::Core::FloatingWindow::isInDragArea(globalPoint);
}
bool FloatingWindow_wrapper::isMDI() const
{
    if (m_isMDICallback) {
        const void *thisPtr = this;
        return m_isMDICallback(const_cast<void *>(thisPtr));
    } else {
        return ::KDDockWidgets::Core::FloatingWindow::isMDI();
    }
}
bool FloatingWindow_wrapper::isMDI_nocallback() const
{
    return ::KDDockWidgets::Core::FloatingWindow::isMDI();
}
bool FloatingWindow_wrapper::isUtilityWindow() const
{
    return ::KDDockWidgets::Core::FloatingWindow::isUtilityWindow();
}
bool FloatingWindow_wrapper::isWindow() const
{
    if (m_isWindowCallback) {
        const void *thisPtr = this;
        return m_isWindowCallback(const_cast<void *>(thisPtr));
    } else {
        return ::KDDockWidgets::Core::FloatingWindow::isWindow();
    }
}
bool FloatingWindow_wrapper::isWindow_nocallback() const
{
    return ::KDDockWidgets::Core::FloatingWindow::isWindow();
}
KDDockWidgets::Core::Layout *FloatingWindow_wrapper::layout() const
{
    return ::KDDockWidgets::Core::FloatingWindow::layout();
}
KDDockWidgets::Core::MainWindow *FloatingWindow_wrapper::mainWindow() const
{
    return ::KDDockWidgets::Core::FloatingWindow::mainWindow();
}
void FloatingWindow_wrapper::maybeCreateResizeHandler()
{
    ::KDDockWidgets::Core::FloatingWindow::maybeCreateResizeHandler();
}
KDDockWidgets::Core::DropArea *FloatingWindow_wrapper::multiSplitter() const
{
    return ::KDDockWidgets::Core::FloatingWindow::multiSplitter();
}
void FloatingWindow_wrapper::scheduleDeleteLater()
{
    ::KDDockWidgets::Core::FloatingWindow::scheduleDeleteLater();
}
void FloatingWindow_wrapper::setParentView_impl(KDDockWidgets::Core::View *parent)
{
    if (m_setParentView_implCallback) {
        const void *thisPtr = this;
        m_setParentView_implCallback(const_cast<void *>(thisPtr), parent);
    } else {
        ::KDDockWidgets::Core::FloatingWindow::setParentView_impl(parent);
    }
}
void FloatingWindow_wrapper::setParentView_impl_nocallback(KDDockWidgets::Core::View *parent)
{
    ::KDDockWidgets::Core::FloatingWindow::setParentView_impl(parent);
}
void FloatingWindow_wrapper::setSuggestedGeometry(KDDockWidgets::Rect suggestedRect)
{
    ::KDDockWidgets::Core::FloatingWindow::setSuggestedGeometry(suggestedRect);
}
KDDockWidgets::Core::DockWidget *FloatingWindow_wrapper::singleDockWidget() const
{
    if (m_singleDockWidgetCallback) {
        const void *thisPtr = this;
        return m_singleDockWidgetCallback(const_cast<void *>(thisPtr));
    } else {
        return ::KDDockWidgets::Core::FloatingWindow::singleDockWidget();
    }
}
KDDockWidgets::Core::DockWidget *FloatingWindow_wrapper::singleDockWidget_nocallback() const
{
    return ::KDDockWidgets::Core::FloatingWindow::singleDockWidget();
}
KDDockWidgets::Core::Group *FloatingWindow_wrapper::singleFrame() const
{
    return ::KDDockWidgets::Core::FloatingWindow::singleFrame();
}
bool FloatingWindow_wrapper::supportsMaximizeButton() const
{
    return ::KDDockWidgets::Core::FloatingWindow::supportsMaximizeButton();
}
bool FloatingWindow_wrapper::supportsMinimizeButton() const
{
    return ::KDDockWidgets::Core::FloatingWindow::supportsMinimizeButton();
}
KDDockWidgets::Core::TitleBar *FloatingWindow_wrapper::titleBar() const
{
    return ::KDDockWidgets::Core::FloatingWindow::titleBar();
}
void FloatingWindow_wrapper::updateTitleAndIcon()
{
    ::KDDockWidgets::Core::FloatingWindow::updateTitleAndIcon();
}
void FloatingWindow_wrapper::updateTitleBarVisibility()
{
    ::KDDockWidgets::Core::FloatingWindow::updateTitleBarVisibility();
}
int FloatingWindow_wrapper::userType() const
{
    return ::KDDockWidgets::Core::FloatingWindow::userType();
}
FloatingWindow_wrapper::~FloatingWindow_wrapper()
{
}

}
}
static KDDockWidgets::Core::FloatingWindow *fromPtr(void *ptr)
{
    return reinterpret_cast<KDDockWidgets::Core::FloatingWindow *>(ptr);
}
static KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::FloatingWindow_wrapper *fromWrapperPtr(void *ptr)
{
    return reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::FloatingWindow_wrapper *>(ptr);
}
extern "C" {
void c_KDDockWidgets__Core__FloatingWindow_Finalizer(void *cppObj)
{
    delete reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::FloatingWindow_wrapper *>(cppObj);
}
void *c_KDDockWidgets__Core__FloatingWindow__constructor_Group_Rect_MainWindow(void *group_, void *suggestedGeometry_, void *parent_)
{
    auto group = reinterpret_cast<KDDockWidgets::Core::Group *>(group_);
    assert(suggestedGeometry_);
    auto &suggestedGeometry = *reinterpret_cast<KDDockWidgets::Rect *>(suggestedGeometry_);
    auto parent = reinterpret_cast<KDDockWidgets::Core::MainWindow *>(parent_);
    auto ptr = new KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::FloatingWindow_wrapper(group, suggestedGeometry, parent);
    return reinterpret_cast<void *>(ptr);
}
void *c_KDDockWidgets__Core__FloatingWindow__constructor_Rect_MainWindow(void *suggestedGeometry_, void *parent_)
{
    assert(suggestedGeometry_);
    auto &suggestedGeometry = *reinterpret_cast<KDDockWidgets::Rect *>(suggestedGeometry_);
    auto parent = reinterpret_cast<KDDockWidgets::Core::MainWindow *>(parent_);
    auto ptr = new KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::FloatingWindow_wrapper(suggestedGeometry, parent);
    return reinterpret_cast<void *>(ptr);
}
// addDockWidget(KDDockWidgets::Core::DockWidget * arg__1, KDDockWidgets::Location location, KDDockWidgets::Core::DockWidget * relativeTo, KDDockWidgets::InitialOption arg__4)
void c_KDDockWidgets__Core__FloatingWindow__addDockWidget_DockWidget_Location_DockWidget_InitialOption(void *thisObj, void *arg__1_, int location, void *relativeTo_, void *arg__4_)
{
    auto arg__1 = reinterpret_cast<KDDockWidgets::Core::DockWidget *>(arg__1_);
    auto relativeTo = reinterpret_cast<KDDockWidgets::Core::DockWidget *>(relativeTo_);
    assert(arg__4_);
    auto &arg__4 = *reinterpret_cast<KDDockWidgets::InitialOption *>(arg__4_);
    fromPtr(thisObj)->addDockWidget(arg__1, static_cast<KDDockWidgets::Location>(location), relativeTo, arg__4);
}
// allDockWidgetsHave(KDDockWidgets::DockWidgetOption arg__1) const
bool c_KDDockWidgets__Core__FloatingWindow__allDockWidgetsHave_DockWidgetOption(void *thisObj, int arg__1)
{
    return fromPtr(thisObj)->allDockWidgetsHave(static_cast<KDDockWidgets::DockWidgetOption>(arg__1));
}
// allDockWidgetsHave(KDDockWidgets::LayoutSaverOption arg__1) const
bool c_KDDockWidgets__Core__FloatingWindow__allDockWidgetsHave_LayoutSaverOption(void *thisObj, int arg__1)
{
    return fromPtr(thisObj)->allDockWidgetsHave(static_cast<KDDockWidgets::LayoutSaverOption>(arg__1));
}
// anyDockWidgetsHas(KDDockWidgets::DockWidgetOption arg__1) const
bool c_KDDockWidgets__Core__FloatingWindow__anyDockWidgetsHas_DockWidgetOption(void *thisObj, int arg__1)
{
    return fromPtr(thisObj)->anyDockWidgetsHas(static_cast<KDDockWidgets::DockWidgetOption>(arg__1));
}
// anyDockWidgetsHas(KDDockWidgets::LayoutSaverOption arg__1) const
bool c_KDDockWidgets__Core__FloatingWindow__anyDockWidgetsHas_LayoutSaverOption(void *thisObj, int arg__1)
{
    return fromPtr(thisObj)->anyDockWidgetsHas(static_cast<KDDockWidgets::LayoutSaverOption>(arg__1));
}
// anyNonClosable() const
bool c_KDDockWidgets__Core__FloatingWindow__anyNonClosable(void *thisObj)
{
    return fromPtr(thisObj)->anyNonClosable();
}
// anyNonDockable() const
bool c_KDDockWidgets__Core__FloatingWindow__anyNonDockable(void *thisObj)
{
    return fromPtr(thisObj)->anyNonDockable();
}
// beingDeleted() const
bool c_KDDockWidgets__Core__FloatingWindow__beingDeleted(void *thisObj)
{
    return fromPtr(thisObj)->beingDeleted();
}
// contentMargins() const
void *c_KDDockWidgets__Core__FloatingWindow__contentMargins(void *thisObj)
{
    return new Dartagnan::ValueWrapper<KDDockWidgets::Margins> { fromPtr(thisObj)->contentMargins() };
}
// dragRect() const
void *c_KDDockWidgets__Core__FloatingWindow__dragRect(void *thisObj)
{
    return new Dartagnan::ValueWrapper<KDDockWidgets::Rect> { fromPtr(thisObj)->dragRect() };
}
// dropArea() const
void *c_KDDockWidgets__Core__FloatingWindow__dropArea(void *thisObj)
{
    return fromPtr(thisObj)->dropArea();
}
// ensureRectIsOnScreen(KDDockWidgets::Rect & geometry)
void c_static_KDDockWidgets__Core__FloatingWindow__ensureRectIsOnScreen_Rect(void *geometry_)
{
    assert(geometry_);
    auto &geometry = *reinterpret_cast<KDDockWidgets::Rect *>(geometry_);
    KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::FloatingWindow_wrapper::ensureRectIsOnScreen(geometry);
}
// hasSingleDockWidget() const
bool c_KDDockWidgets__Core__FloatingWindow__hasSingleDockWidget(void *thisObj)
{
    return fromPtr(thisObj)->hasSingleDockWidget();
}
// hasSingleFrame() const
bool c_KDDockWidgets__Core__FloatingWindow__hasSingleFrame(void *thisObj)
{
    return fromPtr(thisObj)->hasSingleFrame();
}
// isInDragArea(KDDockWidgets::Point globalPoint) const
bool c_KDDockWidgets__Core__FloatingWindow__isInDragArea_Point(void *thisObj, void *globalPoint_)
{
    assert(globalPoint_);
    auto &globalPoint = *reinterpret_cast<KDDockWidgets::Point *>(globalPoint_);
    return fromPtr(thisObj)->isInDragArea(globalPoint);
}
// isMDI() const
bool c_KDDockWidgets__Core__FloatingWindow__isMDI(void *thisObj)
{
    return [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::FloatingWindow_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->isMDI_nocallback();} else {    return targetPtr->isMDI();} }();
}
// isUtilityWindow() const
bool c_KDDockWidgets__Core__FloatingWindow__isUtilityWindow(void *thisObj)
{
    return fromPtr(thisObj)->isUtilityWindow();
}
// isWindow() const
bool c_KDDockWidgets__Core__FloatingWindow__isWindow(void *thisObj)
{
    return [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::FloatingWindow_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->isWindow_nocallback();} else {    return targetPtr->isWindow();} }();
}
// layout() const
void *c_KDDockWidgets__Core__FloatingWindow__layout(void *thisObj)
{
    return fromPtr(thisObj)->layout();
}
// mainWindow() const
void *c_KDDockWidgets__Core__FloatingWindow__mainWindow(void *thisObj)
{
    return fromPtr(thisObj)->mainWindow();
}
// maybeCreateResizeHandler()
void c_KDDockWidgets__Core__FloatingWindow__maybeCreateResizeHandler(void *thisObj)
{
    fromPtr(thisObj)->maybeCreateResizeHandler();
}
// multiSplitter() const
void *c_KDDockWidgets__Core__FloatingWindow__multiSplitter(void *thisObj)
{
    return fromPtr(thisObj)->multiSplitter();
}
// scheduleDeleteLater()
void c_KDDockWidgets__Core__FloatingWindow__scheduleDeleteLater(void *thisObj)
{
    fromPtr(thisObj)->scheduleDeleteLater();
}
// setParentView_impl(KDDockWidgets::Core::View * parent)
void c_KDDockWidgets__Core__FloatingWindow__setParentView_impl_View(void *thisObj, void *parent_)
{
    auto parent = reinterpret_cast<KDDockWidgets::Core::View *>(parent_);
    fromWrapperPtr(thisObj)->setParentView_impl_nocallback(parent);
}
// setSuggestedGeometry(KDDockWidgets::Rect suggestedRect)
void c_KDDockWidgets__Core__FloatingWindow__setSuggestedGeometry_Rect(void *thisObj, void *suggestedRect_)
{
    assert(suggestedRect_);
    auto &suggestedRect = *reinterpret_cast<KDDockWidgets::Rect *>(suggestedRect_);
    fromPtr(thisObj)->setSuggestedGeometry(suggestedRect);
}
// singleDockWidget() const
void *c_KDDockWidgets__Core__FloatingWindow__singleDockWidget(void *thisObj)
{
    return [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::FloatingWindow_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->singleDockWidget_nocallback();} else {    return targetPtr->singleDockWidget();} }();
}
// singleFrame() const
void *c_KDDockWidgets__Core__FloatingWindow__singleFrame(void *thisObj)
{
    return fromPtr(thisObj)->singleFrame();
}
// supportsMaximizeButton() const
bool c_KDDockWidgets__Core__FloatingWindow__supportsMaximizeButton(void *thisObj)
{
    return fromPtr(thisObj)->supportsMaximizeButton();
}
// supportsMinimizeButton() const
bool c_KDDockWidgets__Core__FloatingWindow__supportsMinimizeButton(void *thisObj)
{
    return fromPtr(thisObj)->supportsMinimizeButton();
}
// titleBar() const
void *c_KDDockWidgets__Core__FloatingWindow__titleBar(void *thisObj)
{
    return fromPtr(thisObj)->titleBar();
}
// updateTitleAndIcon()
void c_KDDockWidgets__Core__FloatingWindow__updateTitleAndIcon(void *thisObj)
{
    fromPtr(thisObj)->updateTitleAndIcon();
}
// updateTitleBarVisibility()
void c_KDDockWidgets__Core__FloatingWindow__updateTitleBarVisibility(void *thisObj)
{
    fromPtr(thisObj)->updateTitleBarVisibility();
}
// userType() const
int c_KDDockWidgets__Core__FloatingWindow__userType(void *thisObj)
{
    return fromPtr(thisObj)->userType();
}
void c_KDDockWidgets__Core__FloatingWindow__destructor(void *thisObj)
{
    delete fromPtr(thisObj);
}
void c_KDDockWidgets__Core__FloatingWindow__registerVirtualMethodCallback(void *ptr, void *callback, int methodId)
{
    auto wrapper = fromWrapperPtr(ptr);
    switch (methodId) {
    case 976:
        wrapper->m_isMDICallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::FloatingWindow_wrapper::Callback_isMDI>(callback);
        break;
    case 979:
        wrapper->m_isWindowCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::FloatingWindow_wrapper::Callback_isWindow>(callback);
        break;
    case 332:
        wrapper->m_setParentView_implCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::FloatingWindow_wrapper::Callback_setParentView_impl>(callback);
        break;
    case 995:
        wrapper->m_singleDockWidgetCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::FloatingWindow_wrapper::Callback_singleDockWidget>(callback);
        break;
    }
}
}
