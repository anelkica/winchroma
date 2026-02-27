import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import winchroma

/*
    manually changing image color is a pain. remember to set Multieffect
    brightness: 1.0; colorization: 1.0; colorizationColor: color

    otherwise, qt laughs and says no :3
*/
/*
    TODO:
        - separate pieces of code here into components
        - general UI refactor for more responsiveness?
*/

ApplicationWindow {
    id: root
    visible: true
    minimumWidth: expandedWidth + 456
    minimumHeight: 400
    title: "winchroma"
    palette.accent: "#ff99cc"

    property bool sidebarOpen: true
    property int activeIndex: 0

    readonly property int expandedWidth: 220
    readonly property int collapsedWidth: 56

    Component { id: bordersPage; BordersPage {} }
    Component { id: titlebarsPage; TitlebarsPage {} }
    Component { id: highlightsPage; HighlightsPage {} }

    property var navModel: [
        { icon: "qrc:/icons/border_none_24_regular.svg", label: "Borders", page: bordersPage },
        { icon: "qrc:/icons/window_header_horizontal_24_regular.svg", label: "Titlebars", page: titlebarsPage },
        { icon: "qrc:/icons/highlight_24_regular.svg", label: "Highlights", page: highlightsPage },
        //{ icon: "qrc:/icons/border_none_24_regular.svg", label: "Accents" },
    ]

    Component.onCompleted: {
        WindowWatcher.startWatching()
        RegistryManager.setKey("Control Panel\\Colors")
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            id: sidebar
            Layout.preferredWidth: root.sidebarOpen ? root.expandedWidth : root.collapsedWidth
            Layout.fillHeight: true
            color: "#191919"
            clip: true

            Behavior on Layout.preferredWidth {
                NumberAnimation { duration: 200; easing.type: Easing.OutExpo }
            }

            ColumnLayout {
                width: root.expandedWidth
                height: parent.height
                spacing: 2

                // top padding
                Item { Layout.preferredHeight: 8 }

                ItemDelegate {
                    Layout.preferredWidth: root.expandedWidth
                    Layout.preferredHeight: 40
                    leftPadding: (root.collapsedWidth - 18) / 2
                    rightPadding: 0
                    topPadding: 0
                    bottomPadding: 0

                    onClicked: root.sidebarOpen = !root.sidebarOpen

                    contentItem: RowLayout {
                        spacing: 10

                        // cross-fade between open/close icons (text bcuz they look kinda nice actually)
                        Item {
                            Layout.preferredWidth: 20
                            Layout.preferredHeight: 15

                            Label {
                                text: "☰"
                                font.pixelSize: 15
                                font.weight: Font.Light
                                color: palette.windowText
                                anchors.centerIn: parent

                                opacity: 1 - toggleMenuText.opacity
                            }

                            Label {
                                text: "〈"
                                font.pixelSize: 15
                                font.weight: Font.DemiBold
                                color: palette.windowText
                                anchors.centerIn: parent

                                opacity: toggleMenuText.opacity
                            }
                        }

                        Label {
                            id: toggleMenuText
                            text: "winchroma"
                            font.pixelSize: 15
                            font.weight: Font.Bold
                            color: "#f5f0f2"
                            Layout.fillWidth: true
                            opacity: root.sidebarOpen ? 1 : 0
                            Behavior on opacity { NumberAnimation { duration: 150 } }
                        }
                    }
                }

                // sidebar items
                Repeater {
                    model: navModel
                    delegate: ItemDelegate {
                        Layout.preferredWidth: root.expandedWidth
                        Layout.preferredHeight: 40
                        leftPadding: (root.collapsedWidth - 18) / 2
                        rightPadding: 0
                        topPadding: 0
                        bottomPadding: 0
                        highlighted: root.activeIndex === index

                        contentItem: RowLayout {
                            spacing: 10
                            Image {
                                source: modelData.icon
                                sourceSize: Qt.size(20, 20)
                                Layout.alignment: Qt.AlignVCenter

                                // https://forum.qt.io/topic/144070/qt6-color-svg-using-multieffect/7
                                layer.enabled: true
                                layer.effect: MultiEffect {
                                    brightness: 1.0
                                    colorization: 1.0
                                    colorizationColor: palette.windowText
                                }
                            }
                            Label {
                                text: modelData.label
                                color: root.activeIndex === index ? palette.accent : palette.windowText
                                Layout.fillWidth: true
                                opacity: root.sidebarOpen ? 1 : 0
                                Behavior on opacity { NumberAnimation { duration: 150 } }
                                Behavior on color { ColorAnimation { duration: 150 } }
                            }
                        }

                        onClicked: {
                            root.activeIndex = index

                            let activePage = navModel[activeIndex].page
                            contentStack.replace(activePage)
                        }
                    }
                }

                Item { Layout.fillHeight: true }

                // settings (or info? idk, i'll keep it as placeholder)
                ItemDelegate {
                    Layout.preferredWidth: root.expandedWidth
                    Layout.preferredHeight: 40
                    leftPadding: (root.collapsedWidth - 18) / 2
                    rightPadding: 0
                    topPadding: 0
                    bottomPadding: 0

                    contentItem: RowLayout {
                        spacing: 10
                        Image {
                            source: "qrc:/icons/settings_24_regular.svg"
                            sourceSize: Qt.size(20, 20)
                            Layout.alignment: Qt.AlignVCenter

                            layer.enabled: true
                            layer.effect: MultiEffect {
                                brightness: 1.0
                                colorization: 1.0
                                colorizationColor: palette.windowText
                            }
                        }
                        Label {
                            text: "Settings"
                            color: palette.windowText
                            Layout.fillWidth: true
                            opacity: root.sidebarOpen ? 1 : 0
                            Behavior on opacity { NumberAnimation { duration: 150 } }
                        }
                    }
                }

                // bottom padding
                Item { Layout.preferredHeight: 8 }
            }
        }

        // sidebar divider
        Rectangle {
            Layout.preferredWidth: 1
            Layout.fillHeight: true
            color: "#2a2a2e"
        }

        // content
        StackView {
            id: contentStack
            Layout.fillWidth: true
            Layout.fillHeight: true

            initialItem: navModel[root.activeIndex].page

            replaceEnter: Transition {
                NumberAnimation {
                    property: "opacity"
                    from: 0
                    to: 1
                    duration: 150
                    easing.type: Easing.InOutQuad
                }
            }

            replaceExit: Transition {
                NumberAnimation {
                    property: "opacity"
                    from: 1
                    to: 0
                    duration: 150
                    easing.type: Easing.InOutQuad
                }
            }
        }
    }
}
