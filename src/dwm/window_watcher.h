#ifndef WINDOW_WATCHER_H
#define WINDOW_WATCHER_H

#include <QObject>
#include <QQmlEngine>
#include <windows.h>

class WindowWatcher : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    explicit WindowWatcher(QObject *parent = nullptr);
    ~WindowWatcher();

    Q_INVOKABLE void startWatching();
    Q_INVOKABLE void stopWatching();
    Q_INVOKABLE bool isWatching() const;
signals:
    void windowCreated(quintptr hwnd); // (HWND = quintptr) Qt throws a hissy fit when HWND is used :p
private:
    HWINEVENTHOOK m_hook = nullptr;

    static inline WindowWatcher *s_instance;
    static void CALLBACK winEventCallback(HWINEVENTHOOK, DWORD, HWND, LONG, LONG, DWORD, DWORD);
};

#endif // WINDOW_WATCHER_H
