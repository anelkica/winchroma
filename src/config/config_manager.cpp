#include "window_effects.h"
#include "config_manager.h"
#include "app_settings.hpp"
#include "process_utils.h"
#include <QFile>
#include <QStandardPaths>
#include <QDir>
#include <QDesktopServices>
#include <toml.hpp>

using namespace std::literals;

ConfigManager::ConfigManager(QObject *parent) : QObject{parent} { s_instance = this; }
ConfigManager *ConfigManager::instance() {
    return s_instance;
}

Q_INVOKABLE void ConfigManager::openConfig() const {
    QDesktopServices::openUrl(QUrl::fromLocalFile(m_configPath));
}

bool ConfigManager::loadConfig(const QString &path) {
    try {
        const toml::table config = toml::parse_file(path.toStdString());

        if (config.contains("globals")) {
            const auto &globals = config["globals"];
            AppSettings *settings = AppSettings::instance();

            if (const auto *border = globals["border"].as_string())
                settings->setborderColor(QColor(QString::fromStdString(std::string(border->get()))));
            if (const auto *enabled = globals["border_enabled"].as_boolean())
                settings->setborderEnabled(enabled->get());

            if (const auto *titlebar = globals["titlebar"].as_string())
                settings->settitlebarColor(QColor(QString::fromStdString(std::string(titlebar->get()))));
            if (const auto *enabled = globals["titlebar_enabled"].as_boolean())
                settings->settitlebarColorEnabled(enabled->get());

            if (const auto *text = globals["titlebar_text"].as_string())
                settings->settitlebarTextColor(QColor(QString::fromStdString(std::string(text->get()))));
            if (const auto *enabled = globals["titlebar_text_enabled"].as_boolean())
                settings->settitlebarTextEnabled(enabled->get());
        } else {
            qDebug() << "No [globals] section, using AppSettings defaults";
        }

        const auto *rulesArray = config.get_as<toml::array>("rules");
        if (!rulesArray) {
            qDebug() << "No [[rules]] array found";
            m_rules.clear();
        } else {
            m_rules.clear();

            for (const auto &ruleValue : *rulesArray) {
                const auto *ruleTable = ruleValue.as_table();
                if (!ruleTable)
                    continue;

                WindowRule rule;

                // parse name (required)
                auto nameNode = ruleTable->get_as<std::string>("name");
                if (!nameNode) {
                    emit error("rule missing 'name' field");
                    continue;
                }
                rule.name = QString::fromStdString(std::string(nameNode->get()));
                if (rule.name.isEmpty()) {
                    emit error("rule missing 'name' field");
                    continue;
                }

                // parse title regex (optional)
                if (const auto *titleNode = ruleTable->get_as<std::string>("title")) {
                    const QString titlePattern = QString::fromStdString(std::string(titleNode->get()));
                    if (!titlePattern.isEmpty()) {
                        rule.titleRegex = QRegularExpression(titlePattern);
                        if (!rule.titleRegex.isValid()) {
                            emit error(QString("invalid title regex in rule '%1': %2").arg(rule.name, rule.titleRegex.errorString()));
                            continue;
                        }
                    }
                }

                // parse process regex (optional)
                if (const auto *processNode = ruleTable->get_as<std::string>("process")) {
                    const QString processPattern = QString::fromStdString(std::string(processNode->get()));
                    if (!processPattern.isEmpty()) {
                        rule.processRegex = QRegularExpression(processPattern);
                        if (!rule.processRegex.isValid()) {
                            emit error(QString("invalid process regex in rule '%1': %2").arg(rule.name, rule.processRegex.errorString()));
                            continue;
                        }
                    }
                }

                // bail if neither title nor process specified
                if (rule.titleRegex.pattern().isEmpty() && rule.processRegex.pattern().isEmpty()) {
                    emit error(QString("rule '%1' must specify at least 'title' or 'process'").arg(rule.name));
                    continue;
                }

                // parse border color (optional)
                if (const auto *borderNode = ruleTable->get_as<std::string>("border")) {
                    const QString borderStr = QString::fromStdString(std::string(borderNode->get()));
                    if (!borderStr.isEmpty()) {
                        QColor color(borderStr);
                        if (color.isValid())
                            rule.border = color;
                        else
                            emit error(QString("invalid border color '%1' in rule '%2'").arg(borderStr, rule.name));
                    }
                }

                // parse titlebar color (optional)
                if (const auto *titlebarNode = ruleTable->get_as<std::string>("titlebar")) {
                    const QString titlebarStr = QString::fromStdString(std::string(titlebarNode->get()));
                    if (!titlebarStr.isEmpty()) {
                        QColor color(titlebarStr);
                        if (color.isValid())
                            rule.titlebar = color;
                        else
                            emit error(QString("invalid titlebar color '%1' in rule '%2'").arg(titlebarStr, rule.name));
                    }
                }

                // parse titlebar text color (optional)
                if (const auto *titlebarTextNode = ruleTable->get_as<std::string>("titlebar_text")) {
                    const QString titlebarTextStr = QString::fromStdString(std::string(titlebarTextNode->get()));
                    if (!titlebarTextStr.isEmpty()) {
                        QColor color(titlebarTextStr);
                        if (color.isValid())
                            rule.titlebarText = color;
                        else
                            emit error(QString("invalid titlebar_text color '%1' in rule '%2'").arg(titlebarTextStr, rule.name));
                    }
                }

                m_rules.append(rule);
            }

            qDebug() << "Loaded" << m_rules.size() << "rules from:" << path;
        }

        m_configPath = path;
        emit configLoaded();
        return true;

    } catch (const toml::parse_error &err) {
        emit error(QString("failed to parse config: %1").arg(QString::fromUtf8(err.what())));
        return false;
    }
}

bool ConfigManager::saveConfig(const QString &path) {
    try {
        AppSettings *settings = AppSettings::instance();

        toml::table globals{
            { "border",         settings->borderColor().name().toStdString() },
            { "border_enabled", settings->borderEnabled() },

            { "titlebar",         settings->titlebarColor().name().toStdString() },
            { "titlebar_enabled", settings->titlebarColorEnabled() },

            { "titlebar_text",         settings->titlebarTextColor().name().toStdString() },
            { "titlebar_text_enabled", settings->titlebarTextEnabled() }
        };

        toml::array rules;
        for (const auto &rule : std::as_const(m_rules)) {
            toml::table ruleTable{
                { "name", rule.name.toStdString() },
            };

            if (!rule.titleRegex.pattern().isEmpty())
                ruleTable.insert_or_assign("title", rule.titleRegex.pattern().toStdString());

            if (!rule.processRegex.pattern().isEmpty())
                ruleTable.insert_or_assign("process", rule.processRegex.pattern().toStdString());

            if (rule.border.has_value())
                ruleTable.insert_or_assign("border", rule.border->name().toStdString());

            if (rule.titlebar.has_value())
                ruleTable.insert_or_assign("titlebar", rule.titlebar->name().toStdString());

            if (rule.titlebarText.has_value())
                ruleTable.insert_or_assign("titlebar_text", rule.titlebarText->name().toStdString());

            rules.push_back(std::move(ruleTable));
        }

        toml::table config;
        config.insert_or_assign("globals", std::move(globals));
        config.insert_or_assign("rules", std::move(rules));

        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            emit error(QString("failed to open config for writing: %1").arg(file.errorString()));
            return false;
        }

        std::ostringstream oss;
        oss << config;
        QTextStream(&file) << QString::fromStdString(oss.str());
        file.close();

        qDebug() << "Saved config to:" << path;
        emit configSaved();
        return true;

    } catch (const std::exception &err) {
        emit error(QString("failed to save config: %1").arg(err.what()));
        return false;
    }
}

bool ConfigManager::hasMatchingRule(quintptr hwnd) {
    if (!hwnd || m_rules.isEmpty())
        return false;

    auto *hwndHandle = reinterpret_cast<HWND>(hwnd);
    QString title = ProcessUtils::getWindowTitleFromHwnd(hwndHandle);
    QString process = ProcessUtils::getProcessNameFromHwnd(hwndHandle);

    return findMatchingRule(title, process).has_value();
}

std::optional<WindowRule> ConfigManager::findMatchingRule(const QString &windowTitle, const QString &processName) {
    for (const auto &rule : std::as_const(m_rules)) {
        bool hasTitlePattern = !rule.titleRegex.pattern().isEmpty();
        bool hasProcessPattern = !rule.processRegex.pattern().isEmpty();

        if (!hasTitlePattern && !hasProcessPattern)
            continue;

        bool titleMatch = !hasTitlePattern || rule.titleRegex.match(windowTitle).hasMatch();
        bool processMatch = !hasProcessPattern || rule.processRegex.match(processName).hasMatch();

        if (titleMatch && processMatch)
            return rule;
    }

    return std::nullopt;
}

void ConfigManager::applyRulesToWindow(quintptr hwnd) {
    if (!hwnd)
        return;

    if (m_rules.isEmpty())
        return;

    auto *hwndHandle = reinterpret_cast<HWND>(hwnd);
    QString windowTitle = ProcessUtils::getWindowTitleFromHwnd(hwndHandle);
    QString processName = ProcessUtils::getProcessNameFromHwnd(hwndHandle);

    auto rule = findMatchingRule(windowTitle, processName);
    if (!rule)
        return;

    WindowEffects *effects = WindowEffects::instance();

    if (rule->border.has_value())
        effects->setWindowBorderByHWND(hwnd, rule->border.value());

    if (rule->titlebar.has_value())
        effects->setWindowCaptionColorByHWND(hwnd, rule->titlebar.value());

    if (rule->titlebarText.has_value())
        effects->setWindowCaptionTextColorByHWND(hwnd, rule->titlebarText.value());

    emit ruleApplied(rule->name);
}

void ConfigManager::reapplyAllRules() {
    if (m_rules.isEmpty())
        return;

    EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
        WindowEffects *effects = WindowEffects::instance();
        ConfigManager *self = reinterpret_cast<ConfigManager*>(lParam);

        QString title = ProcessUtils::getWindowTitleFromHwnd(hwnd);
        QString process = ProcessUtils::getProcessNameFromHwnd(hwnd);

        auto rule = self->findMatchingRule(title, process);
        if (rule) {
            quintptr hwndInt = reinterpret_cast<quintptr>(hwnd);

            if (rule->border.has_value())
                effects->setWindowBorderByHWND(hwndInt, rule->border.value());

            if (rule->titlebar.has_value())
                effects->setWindowCaptionColorByHWND(hwndInt, rule->titlebar.value());

            if (rule->titlebarText.has_value())
                effects->setWindowCaptionTextColorByHWND(hwndInt, rule->titlebarText.value());
        }

        return TRUE;
    }, reinterpret_cast<LPARAM>(this));
}
