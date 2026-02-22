#pragma once
#include <QString>
#include <windows.h>

#ifndef PROCESS_UTILS_H
#define PROCESS_UTILS_H

class ProcessUtils
{
public:
    static HWND getWindowFromPid(DWORD pid);
    static HWND getWindowByTitle(const QString &title);

    static QList<HWND> getAllWindowsFromPid(DWORD pid); // al windows belonging to PID
};

#endif // PROCESS_UTILS_H
