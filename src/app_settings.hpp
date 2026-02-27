#pragma once
#ifndef APP_SETTINGS_HPP
#define APP_SETTINGS_HPP

#include <QObject>
#include <QColor>
#include <QQmlEngine>

// macros are evil, but this requires like 60% less boilerplate code
// this macro defines getters and setters, but signals must be defined explicitly

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
    QML_PROPERTY(bool, titlebarTextColorEnabled, true)
public:
    explicit AppSettings(QObject *parent = nullptr) : QObject(parent) {}
signals:
    void borderColorChanged(const QColor &value);
    void borderEnabledChanged(const bool &value);

    void titlebarColorChanged(const QColor &value);
    void titlebarColorEnabledChanged(const bool &value);

    void titlebarTextColorChanged(const QColor &value);
    void titlebarTextColorEnabledChanged(const bool &value);
};

#endif // APP_SETTINGS_HPP
