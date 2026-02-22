import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import winchroma

ApplicationWindow {
    id: root
    visible: true
    width: 900
    height: 600
    title: "winchroma"

    Component.onCompleted: {
        WindowWatcher.startWatching()
    }

    Connections {
        target: WindowWatcher
        function onWindowCreated(hwnd) {
            WindowEffects.setWindowBorderByHWND(hwnd, "#FF69B4")
        }
    }

    ColumnLayout {
        anchors.centerIn: parent

        TextField {
            id: colorInput
            Layout.preferredWidth: 200
        }

        Button {
            text: "Add blue borders"
            onClicked: WindowEffects.setAllWindowBorders("#4682B4")
        }
    }
}
