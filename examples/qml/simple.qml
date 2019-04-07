import QtQuick 2.9
import QtQuick.Controls 2.2
import Goxui 1.0

Window {
    id: mainWindow
    visible: true
    width: 560
    height: 460
//    color: "transparent"
    Rectangle {
        id: main
        anchors.fill: parent
        color: "#F1F4F9"
        Rectangle {
            anchors {
                right: parent.right
                bottom: parent.bottom
            }
            width: 80
            height: 80
            color: "red"
            z: 1
        }
        TitleBar {
            width: 100
            height: 30
            x:10
            y:10
            Rectangle{
                anchors.fill: parent
                color: "#72F"
            }
        }
    }
}
