#pragma once
#ifndef APP_SETTINGS_HPP
#define APP_SETTINGS_HPP

#include <QObject>
#include <QColor>
#include <QQmlEngine>

// macros are evil, but this requires like 60% less boilerplate code
// this macro defines getters, setters and signals

namespace AppDefaults {
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

    QML_PROPERTY(QColor, borderColor, QColor(255, 153, 204)) // #ff99cc (pink)
    QML_PROPERTY(bool, borderEnabled, true)

    QML_PROPERTY(QColor, titlebarColor, QColor(25, 25, 25))
    QML_PROPERTY(bool, titlebarColorEnabled, true)

    QML_PROPERTY(QColor, titlebarTextColor, QColor(255, 255, 255))
    QML_PROPERTY(bool, titlebarTextEnabled, true)

    QML_PROPERTY(QColor, hilightColor, QColor(0, 120, 215))
    QML_PROPERTY(bool, hilightEnabled, true)

    QML_PROPERTY(QColor, hilightTextColor, QColor(255, 255, 255))
    QML_PROPERTY(bool, hilightTextEnabled, true)

    QML_PROPERTY(QColor, hotTrackingColor, QColor(0, 120, 215))
    QML_PROPERTY(bool, hotTrackingEnabled, true)

public:
    explicit AppSettings(QObject *parent = nullptr) : QObject(parent) {}
};

#endif // APP_SETTINGS_HPP
