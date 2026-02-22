#ifndef WINDOW_EFFECTS_H
#define WINDOW_EFFECTS_H

#include <QObject>
#include <QQmlEngine>
#include <QColor>
#include <windows.h>

class WindowEffects : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
public:
    explicit WindowEffects(QObject *parent = nullptr);

    Q_INVOKABLE void setWindowBorderByHWND(quintptr hwnd, const QColor &color);
    Q_INVOKABLE void setWindowBorderByTitle(const QString &title, const QColor &color);
    Q_INVOKABLE void resetWindowBorderByHWND(quintptr hwnd);

    Q_INVOKABLE void setAllWindowBorders(const QColor &color);
    Q_INVOKABLE void resetAllWindowBorders();

signals:
private:
    void setWindowAttribute(HWND hwnd, DWORD attribute, const void *value, DWORD size);
    void setWindowAttribute(quintptr hwnd, DWORD attribute, const void *value, DWORD size); // for QML
};

#endif // WINDOW_EFFECTS_H
