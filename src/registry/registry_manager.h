#pragma once
#ifndef REGISTRYMANAGER_H
#define REGISTRYMANAGER_H

#include <QObject>
#include <QQmlEngine>
#include <WinReg.hpp>
#include <expected>

/*
	REG_SZ       - String       - std::wstring                   <- null-terminated!
	REG_DWORD    - Double Word  - DWORD (unsigned int)
	REG_QWORD    - Quad Word    - ULONGLONG (usigned long long)
	REG_BINARY   - Binary       - std::vector<BYTE>              <- ex. images
	REG_MULTI_SZ - Multi-string - std::vector<std::wstring>      <- null-terminated!
*/

/*
    most important regkeys:
        > HKEY_CURRENT_USER\Control Panel\Colors

        + Hilight          - text selection background color
        + HotTrackingColor - mouse drag selection color
        + HilightText      - change color of selected text
        + Menu             - background of context menus
        + MenuText         - ditto, for text

*/

class RegistryManager : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    explicit RegistryManager(QObject *parent = nullptr);

	std::expected<void, winreg::RegResult> SetKey(HKEY key, const std::wstring& subkey);

	std::expected<DWORD, winreg::RegResult> ReadDword(const std::wstring& valueName) const;
	std::expected<void, winreg::RegResult> WriteDword(const std::wstring& valueName, unsigned int value);

	std::expected<std::wstring, winreg::RegResult> ReadString(const std::wstring& valueName) const;
	std::expected<void, winreg::RegResult> WriteString(const std::wstring& valueName, const std::wstring& value);

    // -- QML HANDLERS -- //
    Q_INVOKABLE void restoreDefaults();

    // sends a global boradcasst to tell windows the selection colors are changed
    Q_INVOKABLE void refreshSettings() {
        DWORD_PTR result;
        SendMessageTimeoutW(
            HWND_BROADCAST,
            WM_SETTINGCHANGE,
            0,
            (LPARAM)L"Control Panel\\Colors",
            SMTO_ABORTIFHUNG,
            5000,
            &result
        );
    }

    Q_INVOKABLE QString colorToRegistryString(const QColor &color);

    Q_INVOKABLE bool setKey(const QString &subkey);

    Q_INVOKABLE unsigned int readDword(const QString &valueName);
    Q_INVOKABLE bool writeDword(const QString &valueName, unsigned int value);

    Q_INVOKABLE QString readString(const QString &valueName);
    Q_INVOKABLE bool writeString(const QString &valueName, const QString &value);

signals:
    void errorOccurred(const QString &message);
private:
	winreg::RegKey m_key;

    static constexpr const wchar_t* DEFAULT_HILIGHT             = L"0 120 215";
    static constexpr const wchar_t* DEFAULT_HILIGHT_TEXT        = L"255 255 255";
    static constexpr const wchar_t* DEFAULT_HOT_TRACKING_COLOR  = L"0 102 204";

};

#endif // !REGISTRYMANAGER_H
