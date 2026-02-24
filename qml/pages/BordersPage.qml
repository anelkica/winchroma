import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import winchroma

Item {
    id: bordersPageRoot

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
                    onPickedColorChanged: {
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
                text: "Preview"
                onClicked: {
                    let hwnd = WindowEffects.openPreviewWindow();
                    WindowEffects.setWindowBorderByHWND(hwnd, borderColorPanel.pickedColor)
                }
            }

            // https://invent.kde.org/arnout/qtdeclarative/-/blob/d5171b14251cf4abd7d8ad16288690af53248856/src/quickcontrols/fluentwinui3/Button.qml
            Button {
                text: "Apply"
                highlighted: true
                onClicked: {
                    WindowEffects.setAllWindowBorders(borderColorPanel.pickedColor)
                }
            }
        }
    }
}
