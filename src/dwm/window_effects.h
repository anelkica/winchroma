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
    ~WindowEffects();

    // -- BORDER COLORS -- //

    Q_INVOKABLE void setWindowBorderByHWND(quintptr hwnd, const QColor &color);
    Q_INVOKABLE void resetWindowBorderByHWND(quintptr hwnd);

    Q_INVOKABLE void setAllWindowBorders(const QColor &color);
    Q_INVOKABLE void resetAllWindowBorders();

    // -- TITLEBAR COLORS -- //

    Q_INVOKABLE void setWindowCaptionColorByHWND(quintptr hwnd, const QColor &color);
    Q_INVOKABLE void resetWindowCaptionColorByHWND(quintptr hwnd);

    Q_INVOKABLE void setAllWindowCaptionColors(const QColor &color);
    Q_INVOKABLE void resetAllWindowCaptionColors();

    // -- TITLEBAR TEXT COLORS -- //

    Q_INVOKABLE void setWindowCaptionTextColorByHWND(quintptr hwnd, const QColor &color);
    Q_INVOKABLE void resetWindowCaptionTextColorByHWND(quintptr hwnd);

    Q_INVOKABLE void setAllWindowCaptionTextColors(const QColor &color);
    Q_INVOKABLE void resetAllWindowCaptionTextColors();
signals:
};

#endif // WINDOW_EFFECTS_H
