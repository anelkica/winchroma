import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import winchroma

Item {
    id: highlightsPageRoot

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
            text: "Customize the system selection highlight colors"
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
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 12
            Layout.alignment: Qt.AlignRight

            Button {
                text: "Preview"
                onClicked: {
                }
            }

            // https://invent.kde.org/arnout/qtdeclarative/-/blob/d5171b14251cf4abd7d8ad16288690af53248856/src/quickcontrols/fluentwinui3/Button.qml
            Button {
                text: "Apply"
                highlighted: true
                onClicked: {
                }
            }
        }
    }
}
