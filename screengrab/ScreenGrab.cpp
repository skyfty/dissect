#include <windows.h>
#include <sstream>
#include <string>
#include "ScreenGrab.h"

bool ScreenGrab::waitForFinish(const char *filepath, const char* eventName) {
    if (filepath == nullptr) {
        return false;
    }
    HANDLE waitEvent = OpenEventA(EVENT_ALL_ACCESS, false, eventName);

    auto scrModeal = LoadLibrary("screen-capture-recorder.dll");
    auto pDllRegisterServer = (HRESULT(WINAPI *)())GetProcAddress(scrModeal, "DllRegisterServer");
    if (pDllRegisterServer == NULL) {
        return false;
    }
    pDllRegisterServer();

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    std::stringstream ss;
    ss << "ffmpeg.exe -y -f dshow -i video=\"screen-capture-recorder\" " << filepath;
    std::string str = ss.str();
    std::unique_ptr<char> p(new char[str.size() + 1]);
    std::strcpy(p.get(), str.c_str());
    if (!CreateProcessA(NULL,p.get(),NULL,NULL, FALSE, CREATE_NO_WINDOW,NULL,NULL,&si,&pi))
        return false;

    HANDLE job_handle = CreateJobObject(NULL, NULL);
    AssignProcessToJobObject(job_handle, pi.hProcess);
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION limit_info;
    memset(&limit_info, 0x0, sizeof(limit_info));
    limit_info.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
    SetInformationJobObject(job_handle, JobObjectExtendedLimitInformation, &limit_info, sizeof(limit_info));

    HANDLE handles[2] = {waitEvent,pi.hProcess};
    int nIndex = WaitForMultipleObjects(2, handles, FALSE, INFINITE);
    if (nIndex == WAIT_OBJECT_0)  {
        AttachConsole(pi.dwProcessId);
        SetConsoleCtrlHandler(NULL,TRUE);
        GenerateConsoleCtrlEvent(CTRL_C_EVENT,0);
        FreeConsole();
        WaitForSingleObject(pi.hProcess, 5000);
    }
    CloseHandle(job_handle);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    {
        STARTUPINFOA si;
        PROCESS_INFORMATION pi;
        ZeroMemory( &si, sizeof(si) );
        si.cb = sizeof(si);
        ZeroMemory( &pi, sizeof(pi) );
        std::stringstream ss;
        ss << "ffmpeg.exe"<<" -y -i " << filepath <<" -ss 00:00:00 -vframes 1 -s 1024x768" << " " << filepath<<  ".jpg";
        std::string str = ss.str();
        std::unique_ptr<char> p(new char[str.size() + 1]);
        std::strcpy(p.get(), str.c_str());
        if (CreateProcessA(NULL,p.get(),NULL,NULL, FALSE,CREATE_NO_WINDOW ,NULL,NULL,&si,&pi)) {
            WaitForSingleObject(pi.hProcess, 5000);
        }
    }

    return true;
}
