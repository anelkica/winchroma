#include "window_effects.h"
#include "process_utils.h"
#include <windows.h>
#include <dwmapi.h>

WindowEffects::WindowEffects(QObject *parent) : QObject{parent} {}

void WindowEffects::setWindowBorder(const QString &title) {
    HWND hwnd = ProcessUtils::getWindowByTitle(title);
    if (!hwnd) return;

    COLORREF color = RGB(255, 182, 193);
    HRESULT hr = DwmSetWindowAttribute(hwnd, DWMWA_BORDER_COLOR, &color, sizeof(color));

    if (FAILED(hr))
        qWarning("Failed to set border color");
}
