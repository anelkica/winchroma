import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import winchroma

Item {
    id: bordersPageRoot

    property bool hasChanges: true

    function applyEffectsToCurrentWindows() {
        borderColorPanel.setColor(AppSettings.borderColor) // insurance policy if it doesn't update

        if (borderColorPanel.customizationEnabled)
            WindowEffects.setAllWindowBorders(borderColorPanel.pickedColor)
        else
            WindowEffects.resetAllWindowBorders()

        ConfigManager.reapplyAllRules()
    }

    // this Connection exists because i can't be bothered to figure out why it's not syncing
    // ConfigManager should update borderColor, but BordersPage isn't syncing right, but this fixes it, for now
    Connections {
        target: ConfigManager
        function onConfigLoaded() {
            borderColorPanel.setColor(AppSettings.borderColor)
        }
    }

    Component.onCompleted: {
        borderColorPanel.setColor(AppSettings.borderColor)

        hasChanges = AppSettings.borderEnabled
    }

    ColumnLayout {
        anchors.margins: 24
        anchors.fill: parent
        spacing: 8

        Label {
            text: "Borders"
            font.pixelSize: 28
            font.weight: Font.Bold
            color: palette.windowText
            Layout.alignment: Qt.AlignTop
        }

        Label {
            text: "Customize the color of window borders"
            font.pixelSize: 14
            color: palette.windowText
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
            wrapMode: Text.WordWrap
        }

        ScrollView {
            Layout.topMargin: 12
            Layout.fillWidth: true
            Layout.fillHeight: true
            ScrollBar.vertical.policy: ScrollBar.AsNeeded
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

            ColumnLayout {
                spacing: 12

                ColumnLayout {
                    Label {
                        text: "BORDER COLOR"
                        font.weight: Font.Medium
                        font.pixelSize: 12
                        font.letterSpacing: 0.75
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 1
                        color: palette.midlight
                    }
                }

                ColorPanel {
                    id: borderColorPanel

                    customizationEnabled: AppSettings.borderEnabled
                    onCustomizationEnabledChanged: {
                        hasChanges = true
                        AppSettings.borderEnabled = customizationEnabled
                    }

                    onPickedColorChanged: {
                        hasChanges = true
                        AppSettings.borderColor = borderColorPanel.pickedColor

                        let hwnd = WindowEffects.getPreviewHWND()
                        if (hwnd === 0) return // 0 = doesn't exist

                        WindowEffects.setWindowBorderByHWND(hwnd, borderColorPanel.pickedColor)
                    }
                }
            }
        }


        RowLayout {
            Layout.fillWidth: true
            spacing: 12
            Layout.alignment: Qt.AlignRight

            Button {
                text: "Restore Defaults"
                onClicked: {
                    AppSettings.setBorderDefaults()
                    ConfigManager.saveConfig("config.toml")
                    applyEffectsToCurrentWindows()
                    okDialog.open()

                    hasChanges = false
                }
            }

            Item { Layout.fillWidth: true } // spacer

            Button {
                text: "Preview"
                onClicked: {
                    let hwnd = WindowEffects.openPreviewWindow();
                    WindowEffects.setWindowBorderByHWND(hwnd, borderColorPanel.pickedColor)
                }
            }

            // https://invent.kde.org/arnout/qtdeclarative/-/blob/d5171b14251cf4abd7d8ad16288690af53248856/src/quickcontrols/fluentwinui3/Button.qml
            Button {
                text: "Apply"
                highlighted: hasChanges
                enabled: hasChanges
                onClicked: {
                    applyEffectsToCurrentWindows()
                    ConfigManager.saveConfig("config.toml")

                    hasChanges = false
                }
            }
        }
    }

    OkDialog {
        id: okDialog
        title: "Defaults Restored"
        message: "Borders have been reset to app defaults"
    }
}
