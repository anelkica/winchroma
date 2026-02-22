#include "process_utils.h"
#include <windows.h>

struct EnumData
{
    DWORD pid;
    HWND hwnd;
};


HWND ProcessUtils::getWindowFromPid(DWORD pid) {
    if (pid == 0) {
        qWarning("Invalid PID");
        return nullptr;
    }

    // pain and suffering
    EnumData result = { pid, nullptr };
    EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
        auto *data = reinterpret_cast<EnumData*>(lParam);
        DWORD windowPid = 0;

        GetWindowThreadProcessId(hwnd, &windowPid);
        if (windowPid == data->pid && IsWindowVisible(hwnd) && GetParent(hwnd) == nullptr) {
            data->hwnd = hwnd;
            return FALSE;
        }

        return TRUE;
    }, reinterpret_cast<LPARAM>(&result));

    if (!result.hwnd)
        qWarning("No window found by PID");

    return result.hwnd;
}

HWND ProcessUtils::getWindowByTitle(const QString &title) {
    if (title.isEmpty()) {
        qWarning("Title is empty");
        return nullptr;
    }

    LPCWSTR str = reinterpret_cast<LPCWSTR>(title.utf16());
    HWND hwnd = FindWindow(nullptr, str);

    if (!hwnd)
        qDebug("No window found by title");

    return hwnd;
}
