#include "registry_manager.h"
#include <app_settings.hpp>
#include <qcolor.h>

RegistryManager::RegistryManager(QObject *parent) : QObject{parent} {}

COLORREF RegistryManager::QColorToCOLORREF(const QColor &color) {
    return RGB(color.red(), color.green(), color.blue());
}

std::wstring RegistryManager::QColorToWString(const QColor &color) {
    return QString("%1 %2 %3")
        .arg(color.red())
        .arg(color.green())
        .arg(color.blue())
        .toStdWString();
}

std::expected<void, winreg::RegResult> RegistryManager::SetKey(HKEY key, const std::wstring& subkey) {
	winreg::RegResult result = m_key.TryCreate(key, subkey, KEY_READ | KEY_WRITE);

	if (!result.IsOk())
		return std::unexpected(result);

	return {};
}


std::expected<DWORD, winreg::RegResult> RegistryManager::ReadDword(const std::wstring& valueName) const {
	winreg::RegExpected<DWORD> result = m_key.TryGetDwordValue(valueName);

	if (result.IsValid())
		return result.GetValue();
	else
		return std::unexpected(result.GetError());
};

std::expected<void, winreg::RegResult> RegistryManager::WriteDword(const std::wstring& valueName, unsigned int value) {
	winreg::RegResult result = m_key.TrySetDwordValue(valueName, value);

	if (!result.IsOk())
		return std::unexpected(result);

	return {};
}


std::expected<std::wstring, winreg::RegResult> RegistryManager::ReadString(const std::wstring& valueName) const {
	winreg::RegExpected<std::wstring> result = m_key.TryGetStringValue(valueName);

	if (result.IsValid())
		return result.GetValue();
	else
		return std::unexpected(result.GetError());
}

std::expected<void, winreg::RegResult> RegistryManager::WriteString(const std::wstring& valueName, const std::wstring& value) {
	winreg::RegResult result = m_key.TrySetStringValue(valueName, value);

	if (!result.IsOk())
		return std::unexpected(result);

	return {};
};

// -- QML HANDLERS -- //
void RegistryManager::restoreDefaults() {
    auto result = WriteString(L"Hilight", QColorToWString(AppDefaults::Hilight));
    if (!result)
        emit errorOccurred(QString::fromStdWString(result.error().ErrorMessage()));

    result = WriteString(L"HilightText", QColorToWString(AppDefaults::HilightText));
    if (!result)
        emit errorOccurred(QString::fromStdWString(result.error().ErrorMessage()));

    result = WriteString(L"HotTrackingColor", QColorToWString(AppDefaults::HotTrackingColor));
    if (!result)
        emit errorOccurred(QString::fromStdWString(result.error().ErrorMessage()));

    broadcastColorChange(AppDefaults::Hilight, AppDefaults::HotTrackingColor);
}

void RegistryManager::broadcastColorChange(const QColor &hilight, const QColor &hotTrackingColor) {
    int elements[] = { COLOR_HIGHLIGHT, COLOR_HOTLIGHT }; // hotlight = hotTrackingColor
    COLORREF colors[] = { this->QColorToCOLORREF(hilight), this->QColorToCOLORREF(hotTrackingColor) };
    SetSysColors(2, elements, colors);

    // i'll just broadcast both WM_SYSCOLORCHANGE and WM_SETTINGSCHANGE, hope it works lol
    DWORD_PTR result;
    SendMessageTimeoutW(
        HWND_BROADCAST,
        WM_SYSCOLORCHANGE,
        0, 0,
        SMTO_ABORTIFHUNG,
        2000,
        &result
        );

    SendMessageTimeoutW(
        HWND_BROADCAST,
        WM_SETTINGCHANGE,
        0,
        (LPARAM)L"Control Panel\\Colors",
        SMTO_ABORTIFHUNG,
        2000,
        &result
        );
}

QString RegistryManager::colorToRegistryString(const QColor &color) {
    return QString("%1 %2 %3")
        .arg(color.red())
        .arg(color.green())
        .arg(color.blue());
}

QColor RegistryManager::registryStringToColor(const QString &registryRgbString) {
    if (registryRgbString.isEmpty()) return QColor();

    // "255 255 255" -> ["255", "153", "204"]
    QStringList parts = registryRgbString.split(' ', Qt::SkipEmptyParts);
    if (parts.size() >= 3) {
        return QColor(parts[0].toInt(), parts[1].toInt(), parts[2].toInt());
    }

    return QColor();
}

bool RegistryManager::setKey(const QString &subkey) {
    // there's really no reason not to use HKEY_CURRENT_USER, unless we're gonna read system Accent color?
    auto result = SetKey(HKEY_CURRENT_USER, subkey.toStdWString());
    if (!result) {
        emit errorOccurred(QString::fromStdWString(result.error().ErrorMessage()));
        return false;
    }
    return true;
}

unsigned int RegistryManager::readDword(const QString &valueName) {
    auto result = ReadDword(valueName.toStdWString());
    if (!result) {
        emit errorOccurred(QString::fromStdWString(result.error().ErrorMessage()));
        return 0;
    }
    return result.value();
}

bool RegistryManager::writeDword(const QString &valueName, unsigned int value) {
    auto result = WriteDword(valueName.toStdWString(), value);
    if (!result) {
        emit errorOccurred(QString::fromStdWString(result.error().ErrorMessage()));
        return false;
    }
    return true;
}

QString RegistryManager::readString(const QString &valueName) {
    auto result = ReadString(valueName.toStdWString());
    if (!result) {
        emit errorOccurred(QString::fromStdWString(result.error().ErrorMessage()));
        return {};
    }
    return QString::fromStdWString(result.value());
}

bool RegistryManager::writeString(const QString &valueName, const QString &value) {
    auto result = WriteString(valueName.toStdWString(), value.toStdWString());
    if (!result) {
        emit errorOccurred(QString::fromStdWString(result.error().ErrorMessage()));
        return false;
    }
    return true;
}
