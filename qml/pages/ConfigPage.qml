import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import winchroma

Item {
    id: configPageRoot

    ColumnLayout {
        anchors.margins: 24
        anchors.fill: parent
        spacing: 8

        Label {
            text: "Configuration"
            font.pixelSize: 28
            font.weight: Font.Bold
            color: palette.windowText
            Layout.alignment: Qt.AlignTop
        }

        ColumnLayout {
            spacing: 2

            Label {
                text: "Advanced customization is done through a TOML config file."
                font.pixelSize: 14
                color: palette.windowText
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignTop
                wrapMode: Text.WordWrap
            }

            Label {
                text: "Open the file in any text editor to get started."
                font.pixelSize: 14
                color: palette.windowText
                Layout.fillWidth: true
                wrapMode: Text.WordWrap
            }

            Label {
                text: "The config file supports per-process and per-window rules, allowing you to match windows by process name or title and apply individual border and titlebar colors."
                font.pixelSize: 14
                color: Qt.darker(palette.windowText, 1.2)
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignTop
                Layout.topMargin: 4
                wrapMode: Text.WordWrap
            }
        }

        ScrollView {
            Layout.topMargin: 12
            Layout.fillWidth: true
            Layout.fillHeight: true
            ScrollBar.vertical.policy: ScrollBar.AsNeeded
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 12
            Layout.alignment: Qt.AlignRight

            Button {
                text: "Reload"
                onClicked: {
                    ConfigManager.loadConfig("config.toml")
                    WindowEffects.reapplyEffectsToAllWindows()
                    ConfigManager.reapplyAllRules()
                }
            }

            Button {
                text: "Open in Editor"
                highlighted: true
                onClicked: ConfigManager.openConfig()
            }
        }
    }
}
