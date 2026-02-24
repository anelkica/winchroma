import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import winchroma

RowLayout {
    property alias pickedColor: borderColorDisplay.color

    Rectangle {
        id: borderColorDisplay
        Layout.preferredWidth: 120
        Layout.preferredHeight: 120
        radius: 12
        color: Qt.rgba(redSlider.value, greenSlider.value, blueSlider.value, 1.0)
        border.color: palette.mid
        border.width: 1
    }

    // sliders next to button
    ColumnLayout {
        TextField {
            text: borderColorDisplay.color.toString().toUpperCase()
            Layout.preferredWidth: parent.width * 0.5
            Layout.alignment: Qt.AlignHCenter

            onEditingFinished: {
                try {
                    let c = Qt.color(text)

                    redSlider.value = c.r
                    greenSlider.value = c.g
                    blueSlider.value = c.b
                } catch(e) {} // invalid hex
            }
        }

        RowLayout {
            Layout.leftMargin: 8
            spacing: 0

            Label {
                text: "R"
                font.pixelSize: 12
                color: palette.windowText
                Layout.preferredWidth: 8
            }

            Slider {
                id: redSlider
                from: 0
                to: 1
                value: 1.0
                Layout.fillWidth: true
            }

            Label {
                text: Math.round(redSlider.value * 255)
                font.pixelSize: 12
                color: palette.windowText
                Layout.preferredWidth: 16
                horizontalAlignment: Text.AlignHCenter
            }
        }

        RowLayout {
            Layout.leftMargin: 8
            spacing: 0

            Label {
                text: "G"
                font.pixelSize: 12
                color: palette.windowText
                Layout.preferredWidth: 8
            }

            Slider {
                id: greenSlider
                from: 0
                to: 1
                value: 1.0
                Layout.fillWidth: true
            }

            Label {
                text: Math.round(greenSlider.value * 255)
                font.pixelSize: 12
                color: palette.windowText
                Layout.preferredWidth: 16
                horizontalAlignment: Text.AlignHCenter
            }
        }

        RowLayout {
            Layout.leftMargin: 8
            spacing: 0

            Label {
                text: "B"
                font.pixelSize: 12
                color: palette.windowText
                Layout.preferredWidth: 8
            }

            Slider {
                id: blueSlider
                from: 0
                to: 1
                value: 1.0
                Layout.fillWidth: true
            }

            Label {
                text: Math.round(blueSlider.value * 255)
                font.pixelSize: 12
                color: palette.windowText
                Layout.preferredWidth: 16
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }
}
