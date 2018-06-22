//import related modules
import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0

Window {
    id : mainWindow
    visible: true
    //color: "black"
    width: 560
    height: 460
    color: "transparent"
    flags: Qt.FramelessWindowHint
    Rectangle {
        id: main
        anchors.fill: parent
        color: "#F1F4F9"
        Rect {}

        Rectangle {
            anchors {
                right:parent.right
                bottom:parent.bottom
            }
            width: 80
            height: 80
            color: "red"
            z: 1
        }

        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: Item {
                width: main.width
                height: main.height
                Rectangle {
                    anchors.fill: parent
                    radius: 3
                }
            }
        }
    }
}