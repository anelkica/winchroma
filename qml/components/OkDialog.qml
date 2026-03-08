import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import winchroma

Dialog {
    id: okDialog

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
