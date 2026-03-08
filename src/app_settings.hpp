#pragma once
#ifndef APP_SETTINGS_HPP
#define APP_SETTINGS_HPP

#include <QObject>
#include <QColor>
#include <QQmlEngine>

// macros are evil, but this requires like 60% less boilerplate code
// this macro defines getters, setters and signals

// default colors for Windows system
namespace AppDefaults {
    inline const QColor BorderColor = QColor(255, 153, 204); // #ff99cc (pink)
    inline const QColor TitlebarColor = QColor(25, 25, 25);
    inline const QColor TitlebarTextColor = QColor(255, 255, 255);

    inline const QColor Hilight = QColor(0, 120, 215);
    inline const QColor HilightText = QColor(255, 255, 255);
    inline const QColor HotTrackingColor = QColor(0, 102, 204);
}

#define QML_PROPERTY(type, name, defaultValue) \
Q_PROPERTY(type name READ name WRITE set##name NOTIFY name##Changed) \
    public: \
    type name() const { return m_##name; } \
    void set##name(const type& value) { \
        if (m_##name != value) { \
            m_##name = value; \
            emit name##Changed(value); \
    } \
} \
    Q_SIGNAL void name##Changed(const type& value); \
    private: \
    type m_##name = defaultValue;

class AppSettings : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    QML_PROPERTY(QColor, borderColor, AppDefaults::BorderColor)
    QML_PROPERTY(bool, borderEnabled, true)

    QML_PROPERTY(QColor, titlebarColor, AppDefaults::TitlebarColor)
    QML_PROPERTY(bool, titlebarColorEnabled, false)

    QML_PROPERTY(QColor, titlebarTextColor, AppDefaults::TitlebarTextColor)
    QML_PROPERTY(bool, titlebarTextEnabled, false)

    // registry values below. they fetch and write system registry values. not present in config.toml

    QML_PROPERTY(QColor, hilightColor, AppDefaults::Hilight)
    QML_PROPERTY(bool, hilightEnabled, true)

    QML_PROPERTY(QColor, hilightTextColor, AppDefaults::HilightText)
    QML_PROPERTY(bool, hilightTextEnabled, true)

    QML_PROPERTY(QColor, hotTrackingColor, AppDefaults::HotTrackingColor)
    QML_PROPERTY(bool, hotTrackingEnabled, true)

public:
    explicit AppSettings(QObject *parent = nullptr) : QObject(parent) { s_instance = this; }

    static AppSettings *instance() { return s_instance; }

    // reminder: my QML_PROPERTY macro unfortunately sets the name to be lowercase. ;(
    // borderColor -> setborderColor instead of setBorderColor

    Q_INVOKABLE void setBorderDefaults() {
        setborderColor(AppDefaults::BorderColor);
        setborderEnabled(true);
    }

    Q_INVOKABLE void setTitlebarDefaults() {
        settitlebarColor(AppDefaults::TitlebarColor);
        settitlebarTextColor(AppDefaults::TitlebarTextColor);

        settitlebarColorEnabled(false);
        settitlebarTextEnabled(false);
    }

    Q_INVOKABLE void setHilightDefaults() {
        sethilightColor(AppDefaults::Hilight);
        sethilightTextColor(AppDefaults::HilightText);
        sethotTrackingColor(AppDefaults::HotTrackingColor);

        sethilightEnabled(true);
        sethilightTextEnabled(true);
        sethotTrackingEnabled(true);
    }

private:
    static inline AppSettings *s_instance = nullptr;
};

#endif // APP_SETTINGS_HPP
