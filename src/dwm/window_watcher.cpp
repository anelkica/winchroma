#include "window_watcher.h"

WindowWatcher::WindowWatcher(QObject *parent) : QObject{parent} {}

WindowWatcher::~WindowWatcher() {
    stopWatching();
}

Q_INVOKABLE void WindowWatcher::startWatching() {
    if (m_hook) stopWatching();

    s_instance = this;
    m_hook = SetWinEventHook(
        EVENT_OBJECT_CREATE, EVENT_OBJECT_CREATE,
        nullptr, winEventCallback,
        0, 0,
        WINEVENT_OUTOFCONTEXT
    );

    if (!m_hook)
        qDebug("Failed to set event hook");
}

Q_INVOKABLE void WindowWatcher::stopWatching() {
    if (!m_hook) return;

    UnhookWinEvent(m_hook);
    m_hook = nullptr;
    s_instance = nullptr;
}

Q_INVOKABLE bool WindowWatcher::isWatching() const {
    return m_hook != nullptr;
}

void CALLBACK WindowWatcher::winEventCallback(
    HWINEVENTHOOK, DWORD event,
    HWND hwnd, LONG idObject,
    LONG, DWORD, DWORD)
{
    // https://learn.microsoft.com/en-us/windows/win32/winmsg/window-features
    if (idObject != OBJID_WINDOW || !hwnd || !s_instance) return;
    if (!IsWindow(hwnd)) return;

    emit s_instance->windowCreated(reinterpret_cast<quintptr>(hwnd));
}
