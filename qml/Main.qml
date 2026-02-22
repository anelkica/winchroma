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


    ColumnLayout {
        anchors.centerIn: parent

        Button {
            text: "Add border"
            onClicked: WindowEffects.setWindowBorder("winchroma")
        }
    }
}
