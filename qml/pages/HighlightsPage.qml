import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import winchroma

Item {
    id: highlightsPageRoot

    property bool hasChanges: true

    ColumnLayout {
        anchors.margins: 24
        anchors.fill: parent
        spacing: 8

        Label {
            text: "Highlights"
            font.pixelSize: 28
            font.weight: Font.Bold
            color: palette.windowText
            Layout.alignment: Qt.AlignTop
        }

        Label {
            text: "Customize the color of system selection highlights"
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
                        text: "TEXT HIGHLIGHT COLOR"
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
                    id: textHighlightColorPanel

                    customizationEnabled: AppSettings.hilightEnabled
                    onCustomizationEnabledChanged: {
                        hasChanges = true
                        AppSettings.hilightEnabled = customizationEnabled
                    }

                    onPickedColorChanged: {
                        hasChanges = true
                        AppSettings.hilightColor = textHighlightColorPanel.pickedColor
                    }
                }

                ColumnLayout {
                    Label {
                        text: "MOUSE DRAG COLOR"
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
                    id: mouseDragColorPanel

                    customizationEnabled: AppSettings.hotTrackingEnabled
                    onCustomizationEnabledChanged: {
                        hasChanges = true
                        AppSettings.hotTrackingEnabled = customizationEnabled
                    }

                    onPickedColorChanged: {
                        hasChanges = true
                        AppSettings.hotTrackingColor = mouseDragColorPanel.pickedColor
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
                    RegistryManager.restoreDefaults()

                    confirmDialog.title = "Defaults Restored"
                    confirmDialog.message = "Highlights have been reset to Windows default"
                    confirmDialog.open()

                    // update color panels?

                    hasChanges = true
                }
            }

            Item { Layout.fillWidth: true } // spacer

            // https://invent.kde.org/arnout/qtdeclarative/-/blob/d5171b14251cf4abd7d8ad16288690af53248856/src/quickcontrols/fluentwinui3/Button.qml
            Button {
                text: "Apply"
                highlighted: hasChanges
                enabled: hasChanges
                onClicked: {
                    let textHighlightColorString = RegistryManager.colorToRegistryString(textHighlightColorPanel.pickedColor)
                    let mouseDragColorString = RegistryManager.colorToRegistryString(mouseDragColorPanel.pickedColor)

                    RegistryManager.writeString("Hilight", textHighlightColorString)
                    RegistryManager.writeString("HotTrackingColor", mouseDragColorString)
                    RegistryManager.broadcastColorChange(textHighlightColorPanel.pickedColor, mouseDragColorPanel.pickedColor)

                    hasChanges = false
                }
            }
        }
    }

    Dialog {
        id: confirmDialog

        // https://stackoverflow.com/questions/79379004/qml-binding-loop-detected-for-property-implicitheight/79416343#79416343
        implicitWidth: dialogLabel.implicitWidth + leftPadding + rightPadding
        implicitHeight: implicitHeaderHeight + implicitFooterHeight + dialogLabel.implicitHeight + topPadding + bottomPadding
        modal: true

        anchors.centerIn: parent
        standardButtons: Dialog.Ok

        property alias message: dialogLabel.text

        contentItem: Label {
            id: dialogLabel
            wrapMode: Text.WordWrap
        }
    }
}
