import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import winchroma

Item {
    id: titlebarsPageRoot

    property bool hasChanges: true

    Component.onCompleted: {
        titlebarColorPanel.setColor(AppSettings.titlebarColor)
        titlebarTextColorPanel.setColor(AppSettings.titlebarTextColor)
    }

    Connections {
        target: WindowWatcher
        function onWindowCreated(hwnd) {
            if (titlebarColorPanel.customizationEnabled)
                WindowEffects.setWindowCaptionColorByHWND(hwnd, titlebarColorPanel.pickedColor)

            if (titlebarTextColorPanel.customizationEnabled)
                WindowEffects.setWindowCaptionTextColorByHWND(hwnd, titlebarTextColorPanel.pickedColor)
        }
    }

    ColumnLayout {
        anchors.margins: 24
        anchors.fill: parent
        spacing: 8

        Label {
            text: "Titlebars"
            font.pixelSize: 28
            font.weight: Font.Bold
            color: palette.windowText
            Layout.alignment: Qt.AlignTop
        }

        Label {
            text: "Customize the color of titlebar text and background colors"
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
                        text: "TITLEBAR COLOR"
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
                    id: titlebarColorPanel

                    customizationEnabled: AppSettings.titlebarColorEnabled
                    onCustomizationEnabledChanged: {
                        hasChanges = true
                        AppSettings.titlebarColorEnabled = customizationEnabled
                    }

                    onPickedColorChanged: {
                        hasChanges = true
                        AppSettings.titlebarColor = titlebarColorPanel.pickedColor

                        let hwnd = WindowEffects.getPreviewHWND()
                        if (hwnd === 0) return // 0 = doesn't exist

                        WindowEffects.setWindowCaptionColorByHWND(hwnd, titlebarColorPanel.pickedColor)
                    }
                }

                ColumnLayout {
                    Layout.topMargin: 12

                    Label {
                        text: "TITLEBAR TEXT COLOR"
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
                    id: titlebarTextColorPanel

                    customizationEnabled: AppSettings.titlebarTextEnabled
                    onCustomizationEnabledChanged: {
                        hasChanges = true
                        AppSettings.titlebarTextEnabled = customizationEnabled
                    }

                    onPickedColorChanged: {
                        hasChanges = true
                        AppSettings.titlebarTextColor = titlebarTextColorPanel.pickedColor

                        let hwnd = WindowEffects.getPreviewHWND()
                        if (hwnd === 0) return // 0 = doesn't exist

                        WindowEffects.setWindowCaptionTextColorByHWND(hwnd, titlebarTextColorPanel.pickedColor)
                    }
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 12
            Layout.alignment: Qt.AlignRight

            Button {
                text: "Preview"
                onClicked: {
                    let hwnd = WindowEffects.openPreviewWindow();
                }
            }

            // https://invent.kde.org/arnout/qtdeclarative/-/blob/d5171b14251cf4abd7d8ad16288690af53248856/src/quickcontrols/fluentwinui3/Button.qml
            Button {
                text: "Apply"
                highlighted: hasChanges
                enabled: hasChanges
                onClicked: {
                    if (titlebarColorPanel.customizationEnabled)
                        WindowEffects.setAllWindowCaptionColors(titlebarColorPanel.pickedColor)
                    else
                        WindowEffects.resetAllWindowCaptionColors()

                    if (titlebarTextColorPanel.customizationEnabled)
                        WindowEffects.setAllWindowCaptionTextColors(titlebarTextColorPanel.pickedColor)
                    else
                        WindowEffects.resetAllWindowCaptionTextColors()

                    hasChanges = false
                }
            }
        }
    }
}
