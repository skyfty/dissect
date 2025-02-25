/*
  This file is part of KDDockWidgets.

  SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
  Author: Sérgio Martins <sergio.martins@kdab.com>

  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/

#pragma once

// TODOm4: Remove ?
#ifndef DOCKS_DEVELOPER_MODE
#define DOCKS_DEVELOPER_MODE
#define DOCKS_TESTING_METHODS
#endif

#ifndef QT_NO_CAST_TO_ASCII
#define QT_NO_CAST_TO_ASCII
#endif

#ifndef QT_NO_CAST_FROM_ASCII
#define QT_NO_CAST_FROM_ASCII
#endif

#ifndef QT_NO_URL_CAST_FROM_STRING
#define QT_NO_URL_CAST_FROM_STRING
#endif

#ifndef QT_NO_CAST_FROM_BYTEARRAY
#define QT_NO_CAST_FROM_BYTEARRAY
#endif

#ifndef QT_NO_KEYWORDS
#define QT_NO_KEYWORDS
#endif

#include "Platform.h"
#include "ViewFactory.h"
#include "Window_p.h"

#include "views/View.h"
#include "views/Stack.h"
#include "views/Group.h"
#include "views/TabBar.h"
#include "views/TitleBar.h"
#include "views/DockWidget.h"
#include "views/MainWindow.h"
#include "views/DropArea.h"
#include "views/ClassicIndicatorsWindow.h"

#include "../core/DelayedCall_p.h"
#include "../core/ViewFactory.h"
#include "../core/Layout.h"
#include "../core/DropArea.h"
#include "../core/Group.h"
#include "../core/Stack.h"
#include "../core/TabBar.h"
#include "../core/TitleBar.h"
#include "../core/Separator.h"
#include "../core/SideBar.h"
#include "../core/MainWindow.h"
#include "../core/FloatingWindow.h"
#include "../core/DropIndicatorOverlay.h"
#include "../core/indicators/ClassicDropIndicatorOverlay.h"
#include "../core/layouting/Item_p.h"
#include "../core/views/ClassicIndicatorWindowViewInterface.h"
#include "../core/DockRegistry.h"

#include "../KDDockWidgets.h"
#include "../Config.h"

#include "../qtcompat/string_p.h"
#include "../qtcompat/Object_p.h"
#include "../qtcompat/geometry_helpers_p.h"
