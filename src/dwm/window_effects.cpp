#include "window_effects.h"

#include <dwmapi.h>

WindowEffects::WindowEffects(QObject *parent) : QObject{parent} {}
WindowEffects::~WindowEffects() {
    resetAllWindowBorders();
    resetAllWindowCaptionColors();
    resetAllWindowCaptionTextColors();
}

// -- BORDER COLORS -- //

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

// -- TITLEBAR COLORS -- //

Q_INVOKABLE void WindowEffects::setWindowCaptionColorByHWND(quintptr hwnd, const QColor &color) {
    if (!color.isValid()) {
        qWarning("Invalid color");
        return;
    }

    HWND _hwnd = reinterpret_cast<HWND>(hwnd);
    if (!IsWindow(_hwnd)) return;

    COLORREF colorref = RGB(color.red(), color.green(), color.blue());
    DwmSetWindowAttribute(_hwnd, DWMWA_CAPTION_COLOR, &colorref, sizeof(colorref));
}

Q_INVOKABLE void WindowEffects::resetWindowCaptionColorByHWND(quintptr hwnd) {
    HWND _hwnd = reinterpret_cast<HWND>(hwnd);
    if (!IsWindow(_hwnd)) return;

    COLORREF color = DWMWA_COLOR_DEFAULT;
    DwmSetWindowAttribute(_hwnd, DWMWA_CAPTION_COLOR, &color, sizeof(color));
}

Q_INVOKABLE void WindowEffects::setAllWindowCaptionColors(const QColor &color) {
    if (!color.isValid()) {
        qWarning("Invalid color");
        return;
    }

    COLORREF colorref = RGB(color.red(), color.green(), color.blue());
    EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
        COLORREF colorref = *reinterpret_cast<COLORREF *>(lParam);
        DwmSetWindowAttribute(hwnd, DWMWA_CAPTION_COLOR, &colorref, sizeof(colorref));
        return TRUE;
    }, reinterpret_cast<LPARAM>(&colorref));
}

Q_INVOKABLE void WindowEffects::resetAllWindowCaptionColors() {
    COLORREF colorref = DWMWA_COLOR_DEFAULT;

    EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
        COLORREF colorref = *reinterpret_cast<COLORREF *>(lParam);
        DwmSetWindowAttribute(hwnd, DWMWA_CAPTION_COLOR, &colorref, sizeof(colorref));
        return TRUE;
    }, reinterpret_cast<LPARAM>(&colorref));
}

// -- TITLEBAR TEXT COLORS -- //

Q_INVOKABLE void WindowEffects::setWindowCaptionTextColorByHWND(quintptr hwnd, const QColor &color) {
    if (!color.isValid()) {
        qWarning("Invalid color");
        return;
    }

    HWND _hwnd = reinterpret_cast<HWND>(hwnd);
    if (!IsWindow(_hwnd)) return;

    COLORREF colorref = RGB(color.red(), color.green(), color.blue());
    DwmSetWindowAttribute(_hwnd, DWMWA_TEXT_COLOR, &colorref, sizeof(colorref));
}

Q_INVOKABLE void WindowEffects::resetWindowCaptionTextColorByHWND(quintptr hwnd) {
    HWND _hwnd = reinterpret_cast<HWND>(hwnd);
    if (!IsWindow(_hwnd)) return;

    COLORREF color = DWMWA_COLOR_DEFAULT;
    DwmSetWindowAttribute(_hwnd, DWMWA_TEXT_COLOR, &color, sizeof(color));
}

Q_INVOKABLE void WindowEffects::setAllWindowCaptionTextColors(const QColor &color) {
    if (!color.isValid()) {
        qWarning("Invalid color");
        return;
    }

    COLORREF colorref = RGB(color.red(), color.green(), color.blue());
    EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
        COLORREF colorref = *reinterpret_cast<COLORREF *>(lParam);
        DwmSetWindowAttribute(hwnd, DWMWA_TEXT_COLOR, &colorref, sizeof(colorref));
        return TRUE;
    }, reinterpret_cast<LPARAM>(&colorref));
}

Q_INVOKABLE void WindowEffects::resetAllWindowCaptionTextColors() {
    COLORREF colorref = DWMWA_COLOR_DEFAULT;

    EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
        COLORREF colorref = *reinterpret_cast<COLORREF *>(lParam);
        DwmSetWindowAttribute(hwnd, DWMWA_TEXT_COLOR, &colorref, sizeof(colorref));
        return TRUE;
    }, reinterpret_cast<LPARAM>(&colorref));
}
