#include "SimpleChecker.h"
#include <windows.h>

SimpleChecker::SimpleChecker(const QString &name)
    :m_name(name)
{
}

bool SimpleChecker::running() {
    m_handle = CreateMutexA(nullptr, false, m_name.toStdString().c_str());
    if (m_handle != nullptr) {
        return GetLastError() == ERROR_ALREADY_EXISTS;
    }
    return false;
}

void SimpleChecker::activateWindow(const QString &name)
{
    HWND wnd = FindWindow(NULL, (LPCWSTR)name.unicode());
    if (wnd != NULL) {
        ShowWindow(wnd, SW_RESTORE);
        SetForegroundWindow(wnd);
    }
}
