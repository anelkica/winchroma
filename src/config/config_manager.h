#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <QCoreApplication>
#include <QObject>
#include <QQmlEngine>
#include <QRegularExpression>
#include <QColor>
#include <optional>
#include <QDir>
#include <QList>

struct WindowRule {
    QString name;

    QRegularExpression titleRegex;
    QRegularExpression processRegex;

    std::optional<QColor> border;
    std::optional<QColor> titlebar;
    std::optional<QColor> titlebarText;
};

class ConfigManager : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(QString configPath READ configPath CONSTANT)
public:
    explicit ConfigManager(QObject *parent = nullptr);
    static ConfigManager *instance();


    Q_INVOKABLE void openConfig() const;
    Q_INVOKABLE bool loadConfig(const QString &path);
    Q_INVOKABLE bool saveConfig(const QString &path);

    Q_INVOKABLE bool hasMatchingRule(quintptr hwnd);
    Q_INVOKABLE void applyRulesToWindow(quintptr hwnd);
    Q_INVOKABLE void reapplyAllRules();

    Q_INVOKABLE QString configPath() const { return m_configPath; };
signals:
    void configLoaded();
    void configSaved();
    void ruleApplied(const QString &ruleName);
    void error(const QString &message);

private:
    static inline ConfigManager *s_instance = nullptr;
    std::optional<WindowRule> findMatchingRule(const QString &windowTitle, const QString &processName) const;

    QList<WindowRule> m_rules;
    QString m_configPath = QDir(QCoreApplication::applicationDirPath()).filePath("config.toml");
};

#endif // CONFIG_MANAGER_H
