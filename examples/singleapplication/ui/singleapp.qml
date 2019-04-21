import QtQuick 2.9
import QtQuick.Controls 2.2
import Goxui 1.0

Window {
    id: mainWindow
    visible: true
    width: 720
    height: 570

    TitleBar {
        id: title
        width: parent.width
        x: 0
        height: 50
        z: 100
        Rectangle {
            anchors.fill: parent
            color: "#E0E3E6"
        }
        Text {
            id: titleLabel
            text: qsTr("This is titleBar")
            anchors.centerIn: parent
            color: "#004499"
        }
    }

    Rectangle {
        id: main
        anchors.fill: parent
        color: "#F1F4F9"
        Rectangle {
            anchors.centerIn: parent
            width: 80
            height: 80
            color: "red"
            z: 1
        }
    }
}
