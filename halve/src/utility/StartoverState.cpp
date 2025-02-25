#include <Windows.h>
#include "StartoverState.h"
#include <QCoreApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QThread>
#include <QWindow>
#include <qprocess.h>

static StartoverState *startoverState = nullptr;

StartoverState::StartoverState(QObject *parent)
    : QObject{parent}
{
    startoverState = this;
}

StartoverState *StartoverState::instance()
{
    return startoverState;
}

void StartoverState::watch(int timeout) {
    Qt::HANDLE startoverState = CreateEventA(nullptr, true, false, "StartoverState");
    QThread *thread = new QThread(nullptr);
    thread->connect(thread, &QThread::started, this, [this,timeout, startoverState](){
        int result = WaitForSingleObject(startoverState, timeout);
        if (result == WAIT_OBJECT_0) {
            emit finished();
        }
        CloseHandle(startoverState);
    });
    thread->start();
}

void StartoverState::waitAndKill(int pid) {
    HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
    if (processHandle != nullptr) {
        if (WaitForSingleObject(processHandle, 10000) == WAIT_TIMEOUT) {
            TerminateProcess(processHandle, -1);
        }
        if (WaitForSingleObject(processHandle, 10000) == WAIT_TIMEOUT) {
            QString cmd = QString("TASKKILL /PID %1 /F").arg(pid);
            WinExec(cmd.toLocal8Bit().data(), SW_HIDE);
        }
        CloseHandle(processHandle);
    }
}

void StartoverState::set() {
    Qt::HANDLE startoverState = OpenEventA(EVENT_ALL_ACCESS, false, "StartoverState");
    if (startoverState != nullptr) {
        SetEvent(startoverState);
        CloseHandle(startoverState);
    }
}

bool StartoverState::isRunning() const {
    Qt::HANDLE startoverState = OpenEventA(EVENT_ALL_ACCESS, false, "StartoverState");
    if (startoverState != nullptr) {
        CloseHandle(startoverState);
    }
    return startoverState != nullptr;
}

void StartoverState::exit(int code) {
    QProcess* process = new QProcess();
    QStringList arguments;
    arguments << "--kid"
              << QString("%1").arg(QCoreApplication::applicationPid())
              << "--process-type" << "killer";
    process->start(QCoreApplication::applicationFilePath(), arguments);
    process->waitForStarted();
    QCoreApplication::exit(code);
}

void StartoverState::raise(QWindow *wnd) {
    if (wnd != nullptr) {
        SetWindowPos((HWND)(wnd->winId()), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        SetWindowPos((HWND)(wnd->winId()), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    }
}

static RECT prevWindowRect;
static DWORD prevStyle;
static DWORD prevExStyle;
static BOOL isZoomed = false;

void StartoverState::fullScreen(QWindow *wnd, bool b) {

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    HWND hwnd = (HWND)(wnd->winId());
    if (b) {
        isZoomed = IsZoomed(hwnd);
        GetWindowRect(hwnd, &prevWindowRect);
        prevStyle = GetWindowLong(hwnd, GWL_STYLE);
        prevExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);

        LONG style = GetWindowLong(hwnd, GWL_STYLE);
        style &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU);
        SetWindowLong(hwnd, GWL_STYLE, style);
        LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
        exStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
        SetWindowLong(hwnd, GWL_EXSTYLE, exStyle);
        SetWindowPos(hwnd, HWND_TOP, 0, 0, screenWidth, screenHeight + 1, SWP_NOZORDER | SWP_FRAMECHANGED);
        // SetWindowPos(hwnd, HWND_TOP, 0, 0, screenWidth, screenHeight - 1, SWP_NOZORDER | SWP_FRAMECHANGED);
    } else {
        SetWindowLong(hwnd, GWL_STYLE, prevStyle);
        SetWindowLong(hwnd, GWL_EXSTYLE, prevExStyle);

        if (isZoomed) {
            ShowWindow(hwnd, SW_SHOWMAXIMIZED);
            SetWindowPos(hwnd, NULL, 0, 0,
                         screenWidth, screenHeight,
                         SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
        } else {
            ShowWindow(hwnd, SW_NORMAL);
            SetWindowPos(hwnd, NULL, prevWindowRect.left, prevWindowRect.top,
                         prevWindowRect.right - prevWindowRect.left,
                         prevWindowRect.bottom - prevWindowRect.top -1,
                         SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
        }
    }
}
