#include "registry_manager.h"
#include <qcolor.h>

RegistryManager::RegistryManager(QObject *parent) : QObject{parent} {}

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
    auto result = WriteString(L"Hilight", DEFAULT_HILIGHT);
    if (!result)
        emit errorOccurred(QString::fromStdWString(result.error().ErrorMessage()));

    result = WriteString(L"HilightText", DEFAULT_HILIGHT_TEXT);
    if (!result)
        emit errorOccurred(QString::fromStdWString(result.error().ErrorMessage()));

    result = WriteString(L"HotTrackingColor", DEFAULT_HOT_TRACKING_COLOR);
    if (!result)
        emit errorOccurred(QString::fromStdWString(result.error().ErrorMessage()));
}

QString RegistryManager::colorToRegistryString(const QColor &color) {
    return QString("%1 %2 %3")
        .arg(color.red())
        .arg(color.green())
        .arg(color.blue());
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
