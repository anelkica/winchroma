#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QQuickStyle>

#include <dwmapi.h>
#include <windows.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("FluentWinUI3"); // https://doc.qt.io/qt-6/qtquickcontrols-fluentwinui3.html

    QQmlApplicationEngine engine;

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, [](QObject *obj) {
        if (!obj) return;
        auto *window = qobject_cast<QQuickWindow *>(obj);
        if (!window) return;

        HWND hwnd = reinterpret_cast<HWND>(window->winId());
        if (!hwnd) return;

        // dark titlebar
        COLORREF captionColor = RGB(25, 25, 25);
        DwmSetWindowAttribute(hwnd, DWMWA_CAPTION_COLOR, &captionColor, sizeof(captionColor));

        // pink border
        COLORREF borderColor = RGB(255, 153, 204);
        DwmSetWindowAttribute(hwnd, DWMWA_BORDER_COLOR, &borderColor, sizeof(borderColor));
    });

    engine.loadFromModule("winchroma", "Main");


    return app.exec();
}
