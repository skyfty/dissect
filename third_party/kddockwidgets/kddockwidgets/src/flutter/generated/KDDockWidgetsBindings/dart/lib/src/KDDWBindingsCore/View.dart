/*
  This file is part of KDDockWidgets.

  SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
  Author: Sérgio Martins <sergio.martins@kdab.com>

  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/
import 'dart:ffi' as ffi;
import 'package:ffi/ffi.dart';
import '../TypeHelpers.dart';
import '../../Bindings.dart';
import '../../Bindings_KDDWBindingsCore.dart' as KDDWBindingsCore;
import '../../Bindings_KDDWBindingsFlutter.dart' as KDDWBindingsFlutter;
import '../../LibraryLoader.dart';

var _dylib = Library.instance().dylib;
final _finalizerFunc =
    _dylib.lookup<ffi.NativeFunction<ffi.Void Function(ffi.Pointer)>>(
        'c_KDDockWidgets__Core__View_Finalizer');
final _finalizer = ffi.NativeFinalizer(_finalizerFunc.cast());

class View implements ffi.Finalizable {
  static var s_dartInstanceByCppPtr = Map<int, View>();
  var _thisCpp = null;
  bool _needsAutoDelete = false;
  get thisCpp => _thisCpp;
  set thisCpp(var ptr) {
    _thisCpp = ptr;
    ffi.Pointer<ffi.Void> ptrvoid = ptr.cast<ffi.Void>();
    if (_needsAutoDelete) _finalizer.attach(this, ptrvoid);
  }

  static bool isCached(var cppPointer) {
    return s_dartInstanceByCppPtr.containsKey(cppPointer.address);
  }

  factory View.fromCache(var cppPointer, [needsAutoDelete = false]) {
    return (s_dartInstanceByCppPtr[cppPointer.address] ??
        View.fromCppPointer(cppPointer, needsAutoDelete)) as View;
  }
  View.fromCppPointer(var cppPointer, [this._needsAutoDelete = false]) {
    thisCpp = cppPointer;
  }
  View.init() {} //View(KDDockWidgets::Core::Controller * controller, KDDockWidgets::Core::ViewType arg__2)
  View(KDDWBindingsCore.Controller? controller, int arg__2) {
    final voidstar_Func_voidstar_int func = _dylib
        .lookup<ffi.NativeFunction<voidstar_Func_voidstar_ffi_Int32_FFI>>(
            'c_KDDockWidgets__Core__View__constructor_Controller_ViewType')
        .asFunction();
    thisCpp =
        func(controller == null ? ffi.nullptr : controller.thisCpp, arg__2);
    KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] = this;
    registerCallbacks();
  } // activateWindow()
  activateWindow() {
    final void_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_FFI>>(
            cFunctionSymbolName(343))
        .asFunction();
    func(thisCpp);
  }

  static void activateWindow_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::activateWindow()! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.activateWindow();
  } // asDockWidgetController() const

  KDDWBindingsCore.DockWidget asDockWidgetController() {
    final voidstar_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<voidstar_Func_voidstar_FFI>>(
            'c_KDDockWidgets__Core__View__asDockWidgetController')
        .asFunction();
    ffi.Pointer<void> result = func(thisCpp);
    return KDDWBindingsCore.DockWidget.fromCppPointer(result, false);
  } // asDropAreaController() const

  KDDWBindingsCore.DropArea asDropAreaController() {
    final voidstar_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<voidstar_Func_voidstar_FFI>>(
            'c_KDDockWidgets__Core__View__asDropAreaController')
        .asFunction();
    ffi.Pointer<void> result = func(thisCpp);
    return KDDWBindingsCore.DropArea.fromCppPointer(result, false);
  } // asFloatingWindowController() const

  KDDWBindingsCore.FloatingWindow asFloatingWindowController() {
    final voidstar_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<voidstar_Func_voidstar_FFI>>(
            'c_KDDockWidgets__Core__View__asFloatingWindowController')
        .asFunction();
    ffi.Pointer<void> result = func(thisCpp);
    return KDDWBindingsCore.FloatingWindow.fromCppPointer(result, false);
  } // asGroupController() const

  KDDWBindingsCore.Group asGroupController() {
    final voidstar_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<voidstar_Func_voidstar_FFI>>(
            'c_KDDockWidgets__Core__View__asGroupController')
        .asFunction();
    ffi.Pointer<void> result = func(thisCpp);
    return KDDWBindingsCore.Group.fromCppPointer(result, false);
  } // asLayout() const

  KDDWBindingsCore.Layout asLayout() {
    final voidstar_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<voidstar_Func_voidstar_FFI>>(
            'c_KDDockWidgets__Core__View__asLayout')
        .asFunction();
    ffi.Pointer<void> result = func(thisCpp);
    return KDDWBindingsCore.Layout.fromCppPointer(result, false);
  } // asMainWindowController() const

  KDDWBindingsCore.MainWindow asMainWindowController() {
    final voidstar_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<voidstar_Func_voidstar_FFI>>(
            'c_KDDockWidgets__Core__View__asMainWindowController')
        .asFunction();
    ffi.Pointer<void> result = func(thisCpp);
    return KDDWBindingsCore.MainWindow.fromCppPointer(result, false);
  } // asStackController() const

  KDDWBindingsCore.Stack asStackController() {
    final voidstar_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<voidstar_Func_voidstar_FFI>>(
            'c_KDDockWidgets__Core__View__asStackController')
        .asFunction();
    ffi.Pointer<void> result = func(thisCpp);
    return KDDWBindingsCore.Stack.fromCppPointer(result, false);
  } // asTabBarController() const

  KDDWBindingsCore.TabBar asTabBarController() {
    final voidstar_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<voidstar_Func_voidstar_FFI>>(
            'c_KDDockWidgets__Core__View__asTabBarController')
        .asFunction();
    ffi.Pointer<void> result = func(thisCpp);
    return KDDWBindingsCore.TabBar.fromCppPointer(result, false);
  } // asTitleBarController() const

  KDDWBindingsCore.TitleBar asTitleBarController() {
    final voidstar_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<voidstar_Func_voidstar_FFI>>(
            'c_KDDockWidgets__Core__View__asTitleBarController')
        .asFunction();
    ffi.Pointer<void> result = func(thisCpp);
    return KDDWBindingsCore.TitleBar.fromCppPointer(result, false);
  } // close()

  bool close() {
    final bool_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<bool_Func_voidstar_FFI>>(
            cFunctionSymbolName(353))
        .asFunction();
    return func(thisCpp) != 0;
  }

  static int close_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print("Dart instance not found for View::close()! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.close();
    return result ? 1 : 0;
  } // controller() const

  KDDWBindingsCore.Controller controller() {
    final voidstar_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<voidstar_Func_voidstar_FFI>>(
            'c_KDDockWidgets__Core__View__controller')
        .asFunction();
    ffi.Pointer<void> result = func(thisCpp);
    return KDDWBindingsCore.Controller.fromCppPointer(result, false);
  } // createPlatformWindow()

  createPlatformWindow() {
    final void_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_FFI>>(
            cFunctionSymbolName(355))
        .asFunction();
    func(thisCpp);
  }

  static void createPlatformWindow_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::createPlatformWindow()! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.createPlatformWindow();
  } // deliverViewEventToFilters(KDDockWidgets::Event * e)

  bool deliverViewEventToFilters(Event? e) {
    final bool_Func_voidstar_voidstar func = _dylib
        .lookup<ffi.NativeFunction<bool_Func_voidstar_voidstar_FFI>>(
            'c_KDDockWidgets__Core__View__deliverViewEventToFilters_Event')
        .asFunction();
    return func(thisCpp, e == null ? ffi.nullptr : e.thisCpp) != 0;
  } // dumpDebug()

  dumpDebug() {
    final void_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_FFI>>(
            'c_KDDockWidgets__Core__View__dumpDebug')
        .asFunction();
    func(thisCpp);
  }

  static // equals(const KDDockWidgets::Core::View * one, const KDDockWidgets::Core::View * two)
      bool equals(KDDWBindingsCore.View? one, KDDWBindingsCore.View? two) {
    final bool_Func_voidstar_voidstar func = _dylib
        .lookup<ffi.NativeFunction<bool_Func_voidstar_voidstar_FFI>>(
            'c_static_KDDockWidgets__Core__View__equals_View_View')
        .asFunction();
    return func(one == null ? ffi.nullptr : one.thisCpp,
            two == null ? ffi.nullptr : two.thisCpp) !=
        0;
  } // equals(const KDDockWidgets::Core::View * other) const

  bool equals_2(KDDWBindingsCore.View? other) {
    final bool_Func_voidstar_voidstar func = _dylib
        .lookup<ffi.NativeFunction<bool_Func_voidstar_voidstar_FFI>>(
            'c_KDDockWidgets__Core__View__equals_View')
        .asFunction();
    return func(thisCpp, other == null ? ffi.nullptr : other.thisCpp) != 0;
  }

  static // firstParentOfType(KDDockWidgets::Core::View * view, KDDockWidgets::Core::ViewType arg__2)
      KDDWBindingsCore.Controller firstParentOfType(
          KDDWBindingsCore.View? view, int arg__2) {
    final voidstar_Func_voidstar_int func = _dylib
        .lookup<ffi.NativeFunction<voidstar_Func_voidstar_ffi_Int32_FFI>>(
            'c_static_KDDockWidgets__Core__View__firstParentOfType_View_ViewType')
        .asFunction();
    ffi.Pointer<void> result =
        func(view == null ? ffi.nullptr : view.thisCpp, arg__2);
    return KDDWBindingsCore.Controller.fromCppPointer(result, false);
  } // flags() const

  int flags() {
    final int_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<int_Func_voidstar_FFI>>(
            cFunctionSymbolName(361))
        .asFunction();
    return func(thisCpp);
  }

  static int flags_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::flags() const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.flags();
    return result;
  } // geometry() const

  Rect geometry() {
    final voidstar_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<voidstar_Func_voidstar_FFI>>(
            cFunctionSymbolName(362))
        .asFunction();
    ffi.Pointer<void> result = func(thisCpp);
    return Rect.fromCppPointer(result, true);
  }

  static ffi.Pointer<void> geometry_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::geometry() const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.geometry();
    return result.thisCpp;
  } // grabMouse()

  grabMouse() {
    final void_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_FFI>>(
            cFunctionSymbolName(363))
        .asFunction();
    func(thisCpp);
  }

  static void grabMouse_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::grabMouse()! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.grabMouse();
  }

  static // hardcodedMinimumSize()
      Size hardcodedMinimumSize() {
    final voidstar_Func_void func = _dylib
        .lookup<ffi.NativeFunction<voidstar_Func_void_FFI>>(
            'c_static_KDDockWidgets__Core__View__hardcodedMinimumSize')
        .asFunction();
    ffi.Pointer<void> result = func();
    return Size.fromCppPointer(result, true);
  } // hasFocus() const

  bool hasFocus() {
    final bool_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<bool_Func_voidstar_FFI>>(
            cFunctionSymbolName(366))
        .asFunction();
    return func(thisCpp) != 0;
  }

  static int hasFocus_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::hasFocus() const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.hasFocus();
    return result ? 1 : 0;
  } // height() const

  int height() {
    final int_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<int_Func_voidstar_FFI>>(
            'c_KDDockWidgets__Core__View__height')
        .asFunction();
    return func(thisCpp);
  } // hide()

  hide() {
    final void_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_FFI>>(
            cFunctionSymbolName(368))
        .asFunction();
    func(thisCpp);
  }

  static void hide_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print("Dart instance not found for View::hide()! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.hide();
  } // inDtor() const

  bool inDtor() {
    final bool_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<bool_Func_voidstar_FFI>>(
            'c_KDDockWidgets__Core__View__inDtor')
        .asFunction();
    return func(thisCpp) != 0;
  } // init()

  init() {
    final void_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_FFI>>(
            cFunctionSymbolName(370))
        .asFunction();
    func(thisCpp);
  }

  static void init_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print("Dart instance not found for View::init()! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.init();
  } // isActiveWindow() const

  bool isActiveWindow() {
    final bool_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<bool_Func_voidstar_FFI>>(
            cFunctionSymbolName(372))
        .asFunction();
    return func(thisCpp) != 0;
  }

  static int isActiveWindow_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::isActiveWindow() const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.isActiveWindow();
    return result ? 1 : 0;
  } // isExplicitlyHidden() const

  bool isExplicitlyHidden() {
    final bool_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<bool_Func_voidstar_FFI>>(
            cFunctionSymbolName(373))
        .asFunction();
    return func(thisCpp) != 0;
  }

  static int isExplicitlyHidden_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::isExplicitlyHidden() const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.isExplicitlyHidden();
    return result ? 1 : 0;
  } // isMaximized() const

  bool isMaximized() {
    final bool_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<bool_Func_voidstar_FFI>>(
            cFunctionSymbolName(374))
        .asFunction();
    return func(thisCpp) != 0;
  }

  static int isMaximized_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::isMaximized() const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.isMaximized();
    return result ? 1 : 0;
  } // isMinimized() const

  bool isMinimized() {
    final bool_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<bool_Func_voidstar_FFI>>(
            cFunctionSymbolName(375))
        .asFunction();
    return func(thisCpp) != 0;
  }

  static int isMinimized_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::isMinimized() const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.isMinimized();
    return result ? 1 : 0;
  } // isNull() const

  bool isNull() {
    final bool_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<bool_Func_voidstar_FFI>>(
            cFunctionSymbolName(376))
        .asFunction();
    return func(thisCpp) != 0;
  }

  static int isNull_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::isNull() const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.isNull();
    return result ? 1 : 0;
  } // isRootView() const

  bool isRootView() {
    final bool_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<bool_Func_voidstar_FFI>>(
            cFunctionSymbolName(377))
        .asFunction();
    return func(thisCpp) != 0;
  }

  static int isRootView_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::isRootView() const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.isRootView();
    return result ? 1 : 0;
  } // isVisible() const

  bool isVisible() {
    final bool_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<bool_Func_voidstar_FFI>>(
            cFunctionSymbolName(378))
        .asFunction();
    return func(thisCpp) != 0;
  }

  static int isVisible_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::isVisible() const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.isVisible();
    return result ? 1 : 0;
  } // mapFromGlobal(KDDockWidgets::Point arg__1) const

  Point mapFromGlobal(Point arg__1) {
    final voidstar_Func_voidstar_voidstar func = _dylib
        .lookup<ffi.NativeFunction<voidstar_Func_voidstar_voidstar_FFI>>(
            cFunctionSymbolName(379))
        .asFunction();
    ffi.Pointer<void> result =
        func(thisCpp, arg__1 == null ? ffi.nullptr : arg__1.thisCpp);
    return Point.fromCppPointer(result, true);
  }

  static ffi.Pointer<void> mapFromGlobal_calledFromC(
      ffi.Pointer<void> thisCpp, ffi.Pointer<void> arg__1) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::mapFromGlobal(KDDockWidgets::Point arg__1) const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.mapFromGlobal(Point.fromCppPointer(arg__1));
    return result.thisCpp;
  } // mapTo(KDDockWidgets::Core::View * arg__1, KDDockWidgets::Point arg__2) const

  Point mapTo(KDDWBindingsCore.View? arg__1, Point arg__2) {
    final voidstar_Func_voidstar_voidstar_voidstar func = _dylib
        .lookup<
                ffi.NativeFunction<
                    voidstar_Func_voidstar_voidstar_voidstar_FFI>>(
            cFunctionSymbolName(380))
        .asFunction();
    ffi.Pointer<void> result = func(
        thisCpp,
        arg__1 == null ? ffi.nullptr : arg__1.thisCpp,
        arg__2 == null ? ffi.nullptr : arg__2.thisCpp);
    return Point.fromCppPointer(result, true);
  }

  static ffi.Pointer<void> mapTo_calledFromC(ffi.Pointer<void> thisCpp,
      ffi.Pointer<void>? arg__1, ffi.Pointer<void> arg__2) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::mapTo(KDDockWidgets::Core::View * arg__1, KDDockWidgets::Point arg__2) const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.mapTo(
        (arg__1 == null || arg__1.address == 0)
            ? null
            : KDDWBindingsCore.View.fromCppPointer(arg__1),
        Point.fromCppPointer(arg__2));
    return result.thisCpp;
  } // mapToGlobal(KDDockWidgets::Point arg__1) const

  Point mapToGlobal(Point arg__1) {
    final voidstar_Func_voidstar_voidstar func = _dylib
        .lookup<ffi.NativeFunction<voidstar_Func_voidstar_voidstar_FFI>>(
            cFunctionSymbolName(381))
        .asFunction();
    ffi.Pointer<void> result =
        func(thisCpp, arg__1 == null ? ffi.nullptr : arg__1.thisCpp);
    return Point.fromCppPointer(result, true);
  }

  static ffi.Pointer<void> mapToGlobal_calledFromC(
      ffi.Pointer<void> thisCpp, ffi.Pointer<void> arg__1) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::mapToGlobal(KDDockWidgets::Point arg__1) const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.mapToGlobal(Point.fromCppPointer(arg__1));
    return result.thisCpp;
  } // maxSizeHint() const

  Size maxSizeHint() {
    final voidstar_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<voidstar_Func_voidstar_FFI>>(
            cFunctionSymbolName(382))
        .asFunction();
    ffi.Pointer<void> result = func(thisCpp);
    return Size.fromCppPointer(result, true);
  }

  static ffi.Pointer<void> maxSizeHint_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::maxSizeHint() const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.maxSizeHint();
    return result.thisCpp;
  } // minSize() const

  Size minSize() {
    final voidstar_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<voidstar_Func_voidstar_FFI>>(
            cFunctionSymbolName(383))
        .asFunction();
    ffi.Pointer<void> result = func(thisCpp);
    return Size.fromCppPointer(result, true);
  }

  static ffi.Pointer<void> minSize_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::minSize() const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.minSize();
    return result.thisCpp;
  } // minimumHeight() const

  int minimumHeight() {
    final int_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<int_Func_voidstar_FFI>>(
            'c_KDDockWidgets__Core__View__minimumHeight')
        .asFunction();
    return func(thisCpp);
  } // minimumWidth() const

  int minimumWidth() {
    final int_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<int_Func_voidstar_FFI>>(
            'c_KDDockWidgets__Core__View__minimumWidth')
        .asFunction();
    return func(thisCpp);
  } // move(KDDockWidgets::Point arg__1)

  move(Point arg__1) {
    final void_Func_voidstar_voidstar func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_voidstar_FFI>>(
            'c_KDDockWidgets__Core__View__move_Point')
        .asFunction();
    func(thisCpp, arg__1 == null ? ffi.nullptr : arg__1.thisCpp);
  } // move(int x, int y)

  move_2(int x, int y) {
    final void_Func_voidstar_int_int func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_ffi_Int32_ffi_Int32_FFI>>(
            cFunctionSymbolName(387))
        .asFunction();
    func(thisCpp, x, y);
  }

  static void move_2_calledFromC(ffi.Pointer<void> thisCpp, int x, int y) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::move(int x, int y)! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.move_2(x, y);
  } // normalGeometry() const

  Rect normalGeometry() {
    final voidstar_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<voidstar_Func_voidstar_FFI>>(
            cFunctionSymbolName(388))
        .asFunction();
    ffi.Pointer<void> result = func(thisCpp);
    return Rect.fromCppPointer(result, true);
  }

  static ffi.Pointer<void> normalGeometry_calledFromC(
      ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::normalGeometry() const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.normalGeometry();
    return result.thisCpp;
  } // onResize(KDDockWidgets::Size arg__1)

  bool onResize(Size arg__1) {
    final bool_Func_voidstar_voidstar func = _dylib
        .lookup<ffi.NativeFunction<bool_Func_voidstar_voidstar_FFI>>(
            'c_KDDockWidgets__Core__View__onResize_Size')
        .asFunction();
    return func(thisCpp, arg__1 == null ? ffi.nullptr : arg__1.thisCpp) != 0;
  } // onResize(int h, int w)

  bool onResize_2(int h, int w) {
    final bool_Func_voidstar_int_int func = _dylib
        .lookup<ffi.NativeFunction<bool_Func_voidstar_ffi_Int32_ffi_Int32_FFI>>(
            cFunctionSymbolName(390))
        .asFunction();
    return func(thisCpp, h, w) != 0;
  }

  static int onResize_2_calledFromC(ffi.Pointer<void> thisCpp, int h, int w) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::onResize(int h, int w)! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.onResize_2(h, w);
    return result ? 1 : 0;
  } // pos() const

  Point pos() {
    final voidstar_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<voidstar_Func_voidstar_FFI>>(
            'c_KDDockWidgets__Core__View__pos')
        .asFunction();
    ffi.Pointer<void> result = func(thisCpp);
    return Point.fromCppPointer(result, true);
  } // raise()

  raise() {
    final void_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_FFI>>(
            cFunctionSymbolName(392))
        .asFunction();
    func(thisCpp);
  }

  static void raise_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print("Dart instance not found for View::raise()! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.raise();
  } // raiseAndActivate()

  raiseAndActivate() {
    final void_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_FFI>>(
            cFunctionSymbolName(393))
        .asFunction();
    func(thisCpp);
  }

  static void raiseAndActivate_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::raiseAndActivate()! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.raiseAndActivate();
  } // rect() const

  Rect rect() {
    final voidstar_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<voidstar_Func_voidstar_FFI>>(
            'c_KDDockWidgets__Core__View__rect')
        .asFunction();
    ffi.Pointer<void> result = func(thisCpp);
    return Rect.fromCppPointer(result, true);
  } // releaseKeyboard()

  releaseKeyboard() {
    final void_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_FFI>>(
            cFunctionSymbolName(395))
        .asFunction();
    func(thisCpp);
  }

  static void releaseKeyboard_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::releaseKeyboard()! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.releaseKeyboard();
  } // releaseMouse()

  releaseMouse() {
    final void_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_FFI>>(
            cFunctionSymbolName(396))
        .asFunction();
    func(thisCpp);
  }

  static void releaseMouse_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::releaseMouse()! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.releaseMouse();
  } // resize(KDDockWidgets::Size arg__1)

  resize(Size arg__1) {
    final void_Func_voidstar_voidstar func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_voidstar_FFI>>(
            'c_KDDockWidgets__Core__View__resize_Size')
        .asFunction();
    func(thisCpp, arg__1 == null ? ffi.nullptr : arg__1.thisCpp);
  } // resize(int w, int h)

  resize_2(int w, int h) {
    final void_Func_voidstar_int_int func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_ffi_Int32_ffi_Int32_FFI>>(
            'c_KDDockWidgets__Core__View__resize_int_int')
        .asFunction();
    func(thisCpp, w, h);
  } // setCursor(Qt::CursorShape arg__1)

  setCursor(int arg__1) {
    final void_Func_voidstar_int func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_ffi_Int32_FFI>>(
            cFunctionSymbolName(399))
        .asFunction();
    func(thisCpp, arg__1);
  }

  static void setCursor_calledFromC(ffi.Pointer<void> thisCpp, int arg__1) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::setCursor(Qt::CursorShape arg__1)! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.setCursor(arg__1);
  } // setFixedHeight(int arg__1)

  setFixedHeight(int arg__1) {
    final void_Func_voidstar_int func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_ffi_Int32_FFI>>(
            cFunctionSymbolName(400))
        .asFunction();
    func(thisCpp, arg__1);
  }

  static void setFixedHeight_calledFromC(
      ffi.Pointer<void> thisCpp, int arg__1) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::setFixedHeight(int arg__1)! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.setFixedHeight(arg__1);
  } // setFixedWidth(int arg__1)

  setFixedWidth(int arg__1) {
    final void_Func_voidstar_int func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_ffi_Int32_FFI>>(
            cFunctionSymbolName(401))
        .asFunction();
    func(thisCpp, arg__1);
  }

  static void setFixedWidth_calledFromC(ffi.Pointer<void> thisCpp, int arg__1) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::setFixedWidth(int arg__1)! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.setFixedWidth(arg__1);
  } // setGeometry(KDDockWidgets::Rect arg__1)

  setGeometry(Rect arg__1) {
    final void_Func_voidstar_voidstar func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_voidstar_FFI>>(
            cFunctionSymbolName(402))
        .asFunction();
    func(thisCpp, arg__1 == null ? ffi.nullptr : arg__1.thisCpp);
  }

  static void setGeometry_calledFromC(
      ffi.Pointer<void> thisCpp, ffi.Pointer<void> arg__1) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::setGeometry(KDDockWidgets::Rect arg__1)! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.setGeometry(Rect.fromCppPointer(arg__1));
  } // setHeight(int height)

  setHeight(int height) {
    final void_Func_voidstar_int func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_ffi_Int32_FFI>>(
            cFunctionSymbolName(403))
        .asFunction();
    func(thisCpp, height);
  }

  static void setHeight_calledFromC(ffi.Pointer<void> thisCpp, int height) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::setHeight(int height)! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.setHeight(height);
  } // setMaximumSize(KDDockWidgets::Size sz)

  setMaximumSize(Size sz) {
    final void_Func_voidstar_voidstar func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_voidstar_FFI>>(
            cFunctionSymbolName(404))
        .asFunction();
    func(thisCpp, sz == null ? ffi.nullptr : sz.thisCpp);
  }

  static void setMaximumSize_calledFromC(
      ffi.Pointer<void> thisCpp, ffi.Pointer<void> sz) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::setMaximumSize(KDDockWidgets::Size sz)! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.setMaximumSize(Size.fromCppPointer(sz));
  } // setMinimumSize(KDDockWidgets::Size arg__1)

  setMinimumSize(Size arg__1) {
    final void_Func_voidstar_voidstar func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_voidstar_FFI>>(
            cFunctionSymbolName(405))
        .asFunction();
    func(thisCpp, arg__1 == null ? ffi.nullptr : arg__1.thisCpp);
  }

  static void setMinimumSize_calledFromC(
      ffi.Pointer<void> thisCpp, ffi.Pointer<void> arg__1) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::setMinimumSize(KDDockWidgets::Size arg__1)! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.setMinimumSize(Size.fromCppPointer(arg__1));
  } // setMouseTracking(bool arg__1)

  setMouseTracking(bool arg__1) {
    final void_Func_voidstar_bool func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_ffi_Int8_FFI>>(
            cFunctionSymbolName(406))
        .asFunction();
    func(thisCpp, arg__1 ? 1 : 0);
  }

  static void setMouseTracking_calledFromC(
      ffi.Pointer<void> thisCpp, int arg__1) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::setMouseTracking(bool arg__1)! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.setMouseTracking(arg__1 != 0);
  } // setParent(KDDockWidgets::Core::View * arg__1)

  setParent(KDDWBindingsCore.View? arg__1) {
    final void_Func_voidstar_voidstar func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_voidstar_FFI>>(
            cFunctionSymbolName(407))
        .asFunction();
    func(thisCpp, arg__1 == null ? ffi.nullptr : arg__1.thisCpp);
  }

  static void setParent_calledFromC(
      ffi.Pointer<void> thisCpp, ffi.Pointer<void>? arg__1) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::setParent(KDDockWidgets::Core::View * arg__1)! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.setParent((arg__1 == null || arg__1.address == 0)
        ? null
        : KDDWBindingsCore.View.fromCppPointer(arg__1));
  } // setSize(KDDockWidgets::Size arg__1)

  setSize(Size arg__1) {
    final void_Func_voidstar_voidstar func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_voidstar_FFI>>(
            'c_KDDockWidgets__Core__View__setSize_Size')
        .asFunction();
    func(thisCpp, arg__1 == null ? ffi.nullptr : arg__1.thisCpp);
  } // setSize(int width, int height)

  setSize_2(int width, int height) {
    final void_Func_voidstar_int_int func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_ffi_Int32_ffi_Int32_FFI>>(
            cFunctionSymbolName(409))
        .asFunction();
    func(thisCpp, width, height);
  }

  static void setSize_2_calledFromC(
      ffi.Pointer<void> thisCpp, int width, int height) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::setSize(int width, int height)! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.setSize_2(width, height);
  } // setViewName(const QString & arg__1)

  setViewName(String? arg__1) {
    final void_Func_voidstar_voidstar func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_voidstar_FFI>>(
            cFunctionSymbolName(410))
        .asFunction();
    func(thisCpp, arg__1?.toNativeUtf8() ?? ffi.nullptr);
  }

  static void setViewName_calledFromC(
      ffi.Pointer<void> thisCpp, ffi.Pointer<void>? arg__1) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::setViewName(const QString & arg__1)! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.setViewName(QString.fromCppPointer(arg__1).toDartString());
  } // setVisible(bool arg__1)

  setVisible(bool arg__1) {
    final void_Func_voidstar_bool func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_ffi_Int8_FFI>>(
            cFunctionSymbolName(411))
        .asFunction();
    func(thisCpp, arg__1 ? 1 : 0);
  }

  static void setVisible_calledFromC(ffi.Pointer<void> thisCpp, int arg__1) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::setVisible(bool arg__1)! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.setVisible(arg__1 != 0);
  } // setWidth(int width)

  setWidth(int width) {
    final void_Func_voidstar_int func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_ffi_Int32_FFI>>(
            cFunctionSymbolName(412))
        .asFunction();
    func(thisCpp, width);
  }

  static void setWidth_calledFromC(ffi.Pointer<void> thisCpp, int width) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::setWidth(int width)! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.setWidth(width);
  } // setWindowOpacity(double arg__1)

  setWindowOpacity(double arg__1) {
    final void_Func_voidstar_double func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_ffi_Double_FFI>>(
            cFunctionSymbolName(413))
        .asFunction();
    func(thisCpp, arg__1);
  }

  static void setWindowOpacity_calledFromC(
      ffi.Pointer<void> thisCpp, double arg__1) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::setWindowOpacity(double arg__1)! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.setWindowOpacity(arg__1);
  } // setWindowTitle(const QString & title)

  setWindowTitle(String? title) {
    final void_Func_voidstar_voidstar func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_voidstar_FFI>>(
            cFunctionSymbolName(414))
        .asFunction();
    func(thisCpp, title?.toNativeUtf8() ?? ffi.nullptr);
  }

  static void setWindowTitle_calledFromC(
      ffi.Pointer<void> thisCpp, ffi.Pointer<void>? title) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::setWindowTitle(const QString & title)! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.setWindowTitle(QString.fromCppPointer(title).toDartString());
  } // setZOrder(int arg__1)

  setZOrder(int arg__1) {
    final void_Func_voidstar_int func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_ffi_Int32_FFI>>(
            cFunctionSymbolName(415))
        .asFunction();
    func(thisCpp, arg__1);
  }

  static void setZOrder_calledFromC(ffi.Pointer<void> thisCpp, int arg__1) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::setZOrder(int arg__1)! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.setZOrder(arg__1);
  } // show()

  show() {
    final void_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_FFI>>(
            cFunctionSymbolName(416))
        .asFunction();
    func(thisCpp);
  }

  static void show_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print("Dart instance not found for View::show()! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.show();
  } // showMaximized()

  showMaximized() {
    final void_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_FFI>>(
            cFunctionSymbolName(417))
        .asFunction();
    func(thisCpp);
  }

  static void showMaximized_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::showMaximized()! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.showMaximized();
  } // showMinimized()

  showMinimized() {
    final void_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_FFI>>(
            cFunctionSymbolName(418))
        .asFunction();
    func(thisCpp);
  }

  static void showMinimized_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::showMinimized()! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.showMinimized();
  } // showNormal()

  showNormal() {
    final void_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_FFI>>(
            cFunctionSymbolName(419))
        .asFunction();
    func(thisCpp);
  }

  static void showNormal_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::showNormal()! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.showNormal();
  } // size() const

  Size size() {
    final voidstar_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<voidstar_Func_voidstar_FFI>>(
            'c_KDDockWidgets__Core__View__size')
        .asFunction();
    ffi.Pointer<void> result = func(thisCpp);
    return Size.fromCppPointer(result, true);
  } // update()

  update() {
    final void_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_FFI>>(
            cFunctionSymbolName(421))
        .asFunction();
    func(thisCpp);
  }

  static void update_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print("Dart instance not found for View::update()! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.update();
  } // viewName() const

  QString viewName() {
    final voidstar_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<voidstar_Func_voidstar_FFI>>(
            cFunctionSymbolName(422))
        .asFunction();
    ffi.Pointer<void> result = func(thisCpp);
    return QString.fromCppPointer(result, true);
  }

  static ffi.Pointer<void> viewName_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address];
    if (dartInstance == null) {
      print(
          "Dart instance not found for View::viewName() const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.viewName();
    return result.thisCpp;
  } // width() const

  int width() {
    final int_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<int_Func_voidstar_FFI>>(
            'c_KDDockWidgets__Core__View__width')
        .asFunction();
    return func(thisCpp);
  } // x() const

  int x() {
    final int_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<int_Func_voidstar_FFI>>(
            'c_KDDockWidgets__Core__View__x')
        .asFunction();
    return func(thisCpp);
  } // y() const

  int y() {
    final int_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<int_Func_voidstar_FFI>>(
            'c_KDDockWidgets__Core__View__y')
        .asFunction();
    return func(thisCpp);
  }

  void release() {
    final void_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_FFI>>(
            'c_KDDockWidgets__Core__View__destructor')
        .asFunction();
    func(thisCpp);
  }

  String cFunctionSymbolName(int methodId) {
    switch (methodId) {
      case 343:
        return "c_KDDockWidgets__Core__View__activateWindow";
      case 353:
        return "c_KDDockWidgets__Core__View__close";
      case 355:
        return "c_KDDockWidgets__Core__View__createPlatformWindow";
      case 361:
        return "c_KDDockWidgets__Core__View__flags";
      case 362:
        return "c_KDDockWidgets__Core__View__geometry";
      case 363:
        return "c_KDDockWidgets__Core__View__grabMouse";
      case 366:
        return "c_KDDockWidgets__Core__View__hasFocus";
      case 368:
        return "c_KDDockWidgets__Core__View__hide";
      case 370:
        return "c_KDDockWidgets__Core__View__init";
      case 372:
        return "c_KDDockWidgets__Core__View__isActiveWindow";
      case 373:
        return "c_KDDockWidgets__Core__View__isExplicitlyHidden";
      case 374:
        return "c_KDDockWidgets__Core__View__isMaximized";
      case 375:
        return "c_KDDockWidgets__Core__View__isMinimized";
      case 376:
        return "c_KDDockWidgets__Core__View__isNull";
      case 377:
        return "c_KDDockWidgets__Core__View__isRootView";
      case 378:
        return "c_KDDockWidgets__Core__View__isVisible";
      case 379:
        return "c_KDDockWidgets__Core__View__mapFromGlobal_Point";
      case 380:
        return "c_KDDockWidgets__Core__View__mapTo_View_Point";
      case 381:
        return "c_KDDockWidgets__Core__View__mapToGlobal_Point";
      case 382:
        return "c_KDDockWidgets__Core__View__maxSizeHint";
      case 383:
        return "c_KDDockWidgets__Core__View__minSize";
      case 387:
        return "c_KDDockWidgets__Core__View__move_int_int";
      case 388:
        return "c_KDDockWidgets__Core__View__normalGeometry";
      case 390:
        return "c_KDDockWidgets__Core__View__onResize_int_int";
      case 392:
        return "c_KDDockWidgets__Core__View__raise";
      case 393:
        return "c_KDDockWidgets__Core__View__raiseAndActivate";
      case 395:
        return "c_KDDockWidgets__Core__View__releaseKeyboard";
      case 396:
        return "c_KDDockWidgets__Core__View__releaseMouse";
      case 399:
        return "c_KDDockWidgets__Core__View__setCursor_CursorShape";
      case 400:
        return "c_KDDockWidgets__Core__View__setFixedHeight_int";
      case 401:
        return "c_KDDockWidgets__Core__View__setFixedWidth_int";
      case 402:
        return "c_KDDockWidgets__Core__View__setGeometry_Rect";
      case 403:
        return "c_KDDockWidgets__Core__View__setHeight_int";
      case 404:
        return "c_KDDockWidgets__Core__View__setMaximumSize_Size";
      case 405:
        return "c_KDDockWidgets__Core__View__setMinimumSize_Size";
      case 406:
        return "c_KDDockWidgets__Core__View__setMouseTracking_bool";
      case 407:
        return "c_KDDockWidgets__Core__View__setParent_View";
      case 409:
        return "c_KDDockWidgets__Core__View__setSize_int_int";
      case 410:
        return "c_KDDockWidgets__Core__View__setViewName_QString";
      case 411:
        return "c_KDDockWidgets__Core__View__setVisible_bool";
      case 412:
        return "c_KDDockWidgets__Core__View__setWidth_int";
      case 413:
        return "c_KDDockWidgets__Core__View__setWindowOpacity_double";
      case 414:
        return "c_KDDockWidgets__Core__View__setWindowTitle_QString";
      case 415:
        return "c_KDDockWidgets__Core__View__setZOrder_int";
      case 416:
        return "c_KDDockWidgets__Core__View__show";
      case 417:
        return "c_KDDockWidgets__Core__View__showMaximized";
      case 418:
        return "c_KDDockWidgets__Core__View__showMinimized";
      case 419:
        return "c_KDDockWidgets__Core__View__showNormal";
      case 421:
        return "c_KDDockWidgets__Core__View__update";
      case 422:
        return "c_KDDockWidgets__Core__View__viewName";
    }
    return "";
  }

  static String methodNameFromId(int methodId) {
    switch (methodId) {
      case 343:
        return "activateWindow";
      case 353:
        return "close";
      case 355:
        return "createPlatformWindow";
      case 361:
        return "flags";
      case 362:
        return "geometry";
      case 363:
        return "grabMouse";
      case 366:
        return "hasFocus";
      case 368:
        return "hide";
      case 370:
        return "init";
      case 372:
        return "isActiveWindow";
      case 373:
        return "isExplicitlyHidden";
      case 374:
        return "isMaximized";
      case 375:
        return "isMinimized";
      case 376:
        return "isNull";
      case 377:
        return "isRootView";
      case 378:
        return "isVisible";
      case 379:
        return "mapFromGlobal";
      case 380:
        return "mapTo";
      case 381:
        return "mapToGlobal";
      case 382:
        return "maxSizeHint";
      case 383:
        return "minSize";
      case 387:
        return "move_2";
      case 388:
        return "normalGeometry";
      case 390:
        return "onResize_2";
      case 392:
        return "raise";
      case 393:
        return "raiseAndActivate";
      case 395:
        return "releaseKeyboard";
      case 396:
        return "releaseMouse";
      case 399:
        return "setCursor";
      case 400:
        return "setFixedHeight";
      case 401:
        return "setFixedWidth";
      case 402:
        return "setGeometry";
      case 403:
        return "setHeight";
      case 404:
        return "setMaximumSize";
      case 405:
        return "setMinimumSize";
      case 406:
        return "setMouseTracking";
      case 407:
        return "setParent";
      case 409:
        return "setSize_2";
      case 410:
        return "setViewName";
      case 411:
        return "setVisible";
      case 412:
        return "setWidth";
      case 413:
        return "setWindowOpacity";
      case 414:
        return "setWindowTitle";
      case 415:
        return "setZOrder";
      case 416:
        return "show";
      case 417:
        return "showMaximized";
      case 418:
        return "showMinimized";
      case 419:
        return "showNormal";
      case 421:
        return "update";
      case 422:
        return "viewName";
    }
    throw Error();
  }

  void registerCallbacks() {
    assert(thisCpp != null);
    final RegisterMethodIsReimplementedCallback registerCallback = _dylib
        .lookup<ffi.NativeFunction<RegisterMethodIsReimplementedCallback_FFI>>(
            'c_KDDockWidgets__Core__View__registerVirtualMethodCallback')
        .asFunction();
    final callback343 = ffi.Pointer.fromFunction<void_Func_voidstar_FFI>(
        KDDWBindingsCore.View.activateWindow_calledFromC);
    registerCallback(thisCpp, callback343, 343);
    const callbackExcept353 = 0;
    final callback353 = ffi.Pointer.fromFunction<bool_Func_voidstar_FFI>(
        KDDWBindingsCore.View.close_calledFromC, callbackExcept353);
    registerCallback(thisCpp, callback353, 353);
    final callback355 = ffi.Pointer.fromFunction<void_Func_voidstar_FFI>(
        KDDWBindingsCore.View.createPlatformWindow_calledFromC);
    registerCallback(thisCpp, callback355, 355);
    const callbackExcept361 = 0;
    final callback361 = ffi.Pointer.fromFunction<int_Func_voidstar_FFI>(
        KDDWBindingsCore.View.flags_calledFromC, callbackExcept361);
    registerCallback(thisCpp, callback361, 361);
    final callback362 = ffi.Pointer.fromFunction<voidstar_Func_voidstar_FFI>(
        KDDWBindingsCore.View.geometry_calledFromC);
    registerCallback(thisCpp, callback362, 362);
    final callback363 = ffi.Pointer.fromFunction<void_Func_voidstar_FFI>(
        KDDWBindingsCore.View.grabMouse_calledFromC);
    registerCallback(thisCpp, callback363, 363);
    const callbackExcept366 = 0;
    final callback366 = ffi.Pointer.fromFunction<bool_Func_voidstar_FFI>(
        KDDWBindingsCore.View.hasFocus_calledFromC, callbackExcept366);
    registerCallback(thisCpp, callback366, 366);
    final callback368 = ffi.Pointer.fromFunction<void_Func_voidstar_FFI>(
        KDDWBindingsCore.View.hide_calledFromC);
    registerCallback(thisCpp, callback368, 368);
    final callback370 = ffi.Pointer.fromFunction<void_Func_voidstar_FFI>(
        KDDWBindingsCore.View.init_calledFromC);
    registerCallback(thisCpp, callback370, 370);
    const callbackExcept372 = 0;
    final callback372 = ffi.Pointer.fromFunction<bool_Func_voidstar_FFI>(
        KDDWBindingsCore.View.isActiveWindow_calledFromC, callbackExcept372);
    registerCallback(thisCpp, callback372, 372);
    const callbackExcept373 = 0;
    final callback373 = ffi.Pointer.fromFunction<bool_Func_voidstar_FFI>(
        KDDWBindingsCore.View.isExplicitlyHidden_calledFromC,
        callbackExcept373);
    registerCallback(thisCpp, callback373, 373);
    const callbackExcept374 = 0;
    final callback374 = ffi.Pointer.fromFunction<bool_Func_voidstar_FFI>(
        KDDWBindingsCore.View.isMaximized_calledFromC, callbackExcept374);
    registerCallback(thisCpp, callback374, 374);
    const callbackExcept375 = 0;
    final callback375 = ffi.Pointer.fromFunction<bool_Func_voidstar_FFI>(
        KDDWBindingsCore.View.isMinimized_calledFromC, callbackExcept375);
    registerCallback(thisCpp, callback375, 375);
    const callbackExcept376 = 0;
    final callback376 = ffi.Pointer.fromFunction<bool_Func_voidstar_FFI>(
        KDDWBindingsCore.View.isNull_calledFromC, callbackExcept376);
    registerCallback(thisCpp, callback376, 376);
    const callbackExcept377 = 0;
    final callback377 = ffi.Pointer.fromFunction<bool_Func_voidstar_FFI>(
        KDDWBindingsCore.View.isRootView_calledFromC, callbackExcept377);
    registerCallback(thisCpp, callback377, 377);
    const callbackExcept378 = 0;
    final callback378 = ffi.Pointer.fromFunction<bool_Func_voidstar_FFI>(
        KDDWBindingsCore.View.isVisible_calledFromC, callbackExcept378);
    registerCallback(thisCpp, callback378, 378);
    final callback379 =
        ffi.Pointer.fromFunction<voidstar_Func_voidstar_voidstar_FFI>(
            KDDWBindingsCore.View.mapFromGlobal_calledFromC);
    registerCallback(thisCpp, callback379, 379);
    final callback380 =
        ffi.Pointer.fromFunction<voidstar_Func_voidstar_voidstar_voidstar_FFI>(
            KDDWBindingsCore.View.mapTo_calledFromC);
    registerCallback(thisCpp, callback380, 380);
    final callback381 =
        ffi.Pointer.fromFunction<voidstar_Func_voidstar_voidstar_FFI>(
            KDDWBindingsCore.View.mapToGlobal_calledFromC);
    registerCallback(thisCpp, callback381, 381);
    final callback382 = ffi.Pointer.fromFunction<voidstar_Func_voidstar_FFI>(
        KDDWBindingsCore.View.maxSizeHint_calledFromC);
    registerCallback(thisCpp, callback382, 382);
    final callback383 = ffi.Pointer.fromFunction<voidstar_Func_voidstar_FFI>(
        KDDWBindingsCore.View.minSize_calledFromC);
    registerCallback(thisCpp, callback383, 383);
    final callback387 =
        ffi.Pointer.fromFunction<void_Func_voidstar_ffi_Int32_ffi_Int32_FFI>(
            KDDWBindingsCore.View.move_2_calledFromC);
    registerCallback(thisCpp, callback387, 387);
    final callback388 = ffi.Pointer.fromFunction<voidstar_Func_voidstar_FFI>(
        KDDWBindingsCore.View.normalGeometry_calledFromC);
    registerCallback(thisCpp, callback388, 388);
    const callbackExcept390 = 0;
    final callback390 =
        ffi.Pointer.fromFunction<bool_Func_voidstar_ffi_Int32_ffi_Int32_FFI>(
            KDDWBindingsCore.View.onResize_2_calledFromC, callbackExcept390);
    registerCallback(thisCpp, callback390, 390);
    final callback392 = ffi.Pointer.fromFunction<void_Func_voidstar_FFI>(
        KDDWBindingsCore.View.raise_calledFromC);
    registerCallback(thisCpp, callback392, 392);
    final callback393 = ffi.Pointer.fromFunction<void_Func_voidstar_FFI>(
        KDDWBindingsCore.View.raiseAndActivate_calledFromC);
    registerCallback(thisCpp, callback393, 393);
    final callback395 = ffi.Pointer.fromFunction<void_Func_voidstar_FFI>(
        KDDWBindingsCore.View.releaseKeyboard_calledFromC);
    registerCallback(thisCpp, callback395, 395);
    final callback396 = ffi.Pointer.fromFunction<void_Func_voidstar_FFI>(
        KDDWBindingsCore.View.releaseMouse_calledFromC);
    registerCallback(thisCpp, callback396, 396);
    final callback399 =
        ffi.Pointer.fromFunction<void_Func_voidstar_ffi_Int32_FFI>(
            KDDWBindingsCore.View.setCursor_calledFromC);
    registerCallback(thisCpp, callback399, 399);
    final callback400 =
        ffi.Pointer.fromFunction<void_Func_voidstar_ffi_Int32_FFI>(
            KDDWBindingsCore.View.setFixedHeight_calledFromC);
    registerCallback(thisCpp, callback400, 400);
    final callback401 =
        ffi.Pointer.fromFunction<void_Func_voidstar_ffi_Int32_FFI>(
            KDDWBindingsCore.View.setFixedWidth_calledFromC);
    registerCallback(thisCpp, callback401, 401);
    final callback402 =
        ffi.Pointer.fromFunction<void_Func_voidstar_voidstar_FFI>(
            KDDWBindingsCore.View.setGeometry_calledFromC);
    registerCallback(thisCpp, callback402, 402);
    final callback403 =
        ffi.Pointer.fromFunction<void_Func_voidstar_ffi_Int32_FFI>(
            KDDWBindingsCore.View.setHeight_calledFromC);
    registerCallback(thisCpp, callback403, 403);
    final callback404 =
        ffi.Pointer.fromFunction<void_Func_voidstar_voidstar_FFI>(
            KDDWBindingsCore.View.setMaximumSize_calledFromC);
    registerCallback(thisCpp, callback404, 404);
    final callback405 =
        ffi.Pointer.fromFunction<void_Func_voidstar_voidstar_FFI>(
            KDDWBindingsCore.View.setMinimumSize_calledFromC);
    registerCallback(thisCpp, callback405, 405);
    final callback406 =
        ffi.Pointer.fromFunction<void_Func_voidstar_ffi_Int8_FFI>(
            KDDWBindingsCore.View.setMouseTracking_calledFromC);
    registerCallback(thisCpp, callback406, 406);
    final callback407 =
        ffi.Pointer.fromFunction<void_Func_voidstar_voidstar_FFI>(
            KDDWBindingsCore.View.setParent_calledFromC);
    registerCallback(thisCpp, callback407, 407);
    final callback409 =
        ffi.Pointer.fromFunction<void_Func_voidstar_ffi_Int32_ffi_Int32_FFI>(
            KDDWBindingsCore.View.setSize_2_calledFromC);
    registerCallback(thisCpp, callback409, 409);
    final callback410 =
        ffi.Pointer.fromFunction<void_Func_voidstar_voidstar_FFI>(
            KDDWBindingsCore.View.setViewName_calledFromC);
    registerCallback(thisCpp, callback410, 410);
    final callback411 =
        ffi.Pointer.fromFunction<void_Func_voidstar_ffi_Int8_FFI>(
            KDDWBindingsCore.View.setVisible_calledFromC);
    registerCallback(thisCpp, callback411, 411);
    final callback412 =
        ffi.Pointer.fromFunction<void_Func_voidstar_ffi_Int32_FFI>(
            KDDWBindingsCore.View.setWidth_calledFromC);
    registerCallback(thisCpp, callback412, 412);
    final callback413 =
        ffi.Pointer.fromFunction<void_Func_voidstar_ffi_Double_FFI>(
            KDDWBindingsCore.View.setWindowOpacity_calledFromC);
    registerCallback(thisCpp, callback413, 413);
    final callback414 =
        ffi.Pointer.fromFunction<void_Func_voidstar_voidstar_FFI>(
            KDDWBindingsCore.View.setWindowTitle_calledFromC);
    registerCallback(thisCpp, callback414, 414);
    final callback415 =
        ffi.Pointer.fromFunction<void_Func_voidstar_ffi_Int32_FFI>(
            KDDWBindingsCore.View.setZOrder_calledFromC);
    registerCallback(thisCpp, callback415, 415);
    final callback416 = ffi.Pointer.fromFunction<void_Func_voidstar_FFI>(
        KDDWBindingsCore.View.show_calledFromC);
    registerCallback(thisCpp, callback416, 416);
    final callback417 = ffi.Pointer.fromFunction<void_Func_voidstar_FFI>(
        KDDWBindingsCore.View.showMaximized_calledFromC);
    registerCallback(thisCpp, callback417, 417);
    final callback418 = ffi.Pointer.fromFunction<void_Func_voidstar_FFI>(
        KDDWBindingsCore.View.showMinimized_calledFromC);
    registerCallback(thisCpp, callback418, 418);
    final callback419 = ffi.Pointer.fromFunction<void_Func_voidstar_FFI>(
        KDDWBindingsCore.View.showNormal_calledFromC);
    registerCallback(thisCpp, callback419, 419);
    final callback421 = ffi.Pointer.fromFunction<void_Func_voidstar_FFI>(
        KDDWBindingsCore.View.update_calledFromC);
    registerCallback(thisCpp, callback421, 421);
    final callback422 = ffi.Pointer.fromFunction<voidstar_Func_voidstar_FFI>(
        KDDWBindingsCore.View.viewName_calledFromC);
    registerCallback(thisCpp, callback422, 422);
  }
}
