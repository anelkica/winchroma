import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

Rectangle {
    id: toastRoot

    anchors.top: parent.top
    anchors.right: parent.right
    anchors.margins: 16

    implicitWidth: messageRow.width + 32
    implicitHeight: messageRow.height + 20
    width: implicitWidth
    height: implicitHeight

    color: "#CC202020"
    radius: 4
    border.color: palette.accent
    border.width: 1

    opacity: 0
    visible: opacity > 0
    z: 1000

    function show(message) {
        displayTimer.stop()
        fadeOutAnimation.stop()
        
        messageLabel.text = message
        fadeInAnimation.start()
    }

    RowLayout {
        id: messageRow
        anchors.centerIn: parent
        spacing: 10

        Image {
            source: "qrc:/icons/info_24_regular.svg"
            sourceSize.width: 24
            sourceSize.height: 24
            Layout.alignment: Qt.AlignVCenter

            layer.enabled: true
            layer.effect: MultiEffect {
                brightness: 1.0
                colorization: 1.0
                colorizationColor: palette.accent
            }
        }

        Label {
            id: messageLabel
            text: ""
            color: palette.windowText
            font.pixelSize: 14
            font.family: "Segoe UI"
            wrapMode: Text.WordWrap
            Layout.maximumWidth: 350
            Layout.alignment: Qt.AlignVCenter
        }
    }

    MultiEffect {
        anchors.fill: toastRoot
        source: toastRoot
        shadowColor: "#40000000"
        shadowBlur: 8
        shadowHorizontalOffset: 0
        shadowVerticalOffset: 2
    }

    NumberAnimation {
        id: fadeInAnimation
        target: toastRoot
        property: "opacity"
        from: 0
        to: 1
        duration: 200
        easing.type: Easing.OutQuad
        onFinished: {
            displayTimer.start()
        }
    }

    Timer {
        id: displayTimer
        interval: 3000
        onTriggered: {
            fadeOutAnimation.start()
        }
    }

    NumberAnimation {
        id: fadeOutAnimation
        target: toastRoot
        property: "opacity"
        from: 1
        to: 0
        duration: 200
        easing.type: Easing.InQuad
    }
}
