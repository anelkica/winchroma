#include "window_effects.h"
#include "process_utils.h"

#include <dwmapi.h>

WindowEffects::WindowEffects(QObject *parent) : QObject{parent} {}

Q_INVOKABLE void WindowEffects::setWindowBorderByHWND(quintptr hwnd, const QColor &color) {
    if (!color.isValid()) {
        qWarning("Invalid color");
        return;
    }

    HWND _hwnd = reinterpret_cast<HWND>(hwnd);
    if (!IsWindow(_hwnd)) return;

    COLORREF colorref = RGB(color.red(), color.green(), color.blue());
    DwmSetWindowAttribute(_hwnd, DWMWA_BORDER_COLOR, &colorref, sizeof(colorref));
}

Q_INVOKABLE void WindowEffects::setWindowBorderByTitle(const QString &title, const QColor &color) {
    if (!color.isValid()) {
        qWarning("Invalid color");
        return;
    }

    HWND hwnd = ProcessUtils::getWindowByTitle(title);
    if (!IsWindow(hwnd)) return;

    COLORREF colorref = RGB(color.red(), color.green(), color.blue());
    DwmSetWindowAttribute(hwnd, DWMWA_BORDER_COLOR, &colorref, sizeof(colorref));
}

Q_INVOKABLE void WindowEffects::resetWindowBorderByHWND(quintptr hwnd)
{
    HWND _hwnd = reinterpret_cast<HWND>(hwnd);
    if (!IsWindow(_hwnd)) return;

    COLORREF color = DWMWA_COLOR_DEFAULT; // 0xFFFFFFFF
    DwmSetWindowAttribute(_hwnd, DWMWA_BORDER_COLOR, &color, sizeof(color));
}

Q_INVOKABLE void WindowEffects::setAllWindowBorders(const QColor &color) {
    if (!color.isValid()) {
        qDebug("Invalid color");
        return;
    }

    COLORREF colorref = RGB(color.red(), color.green(), color.blue());

    EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
        COLORREF colorref = *reinterpret_cast<COLORREF *>(lParam);

        DwmSetWindowAttribute(hwnd, DWMWA_BORDER_COLOR, &colorref, sizeof(colorref));
        return TRUE;
    }, reinterpret_cast<LPARAM>(&colorref));
}

Q_INVOKABLE void WindowEffects::resetAllWindowBorders() {
    COLORREF colorref = DWMWA_COLOR_DEFAULT;

    EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
        COLORREF colorref = *reinterpret_cast<COLORREF *>(lParam);
        DwmSetWindowAttribute(hwnd, DWMWA_BORDER_COLOR, &colorref, sizeof(colorref));
        return TRUE;
    }, reinterpret_cast<LPARAM>(&colorref));
}
