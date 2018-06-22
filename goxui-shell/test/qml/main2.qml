//import related modules
import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0

//UIWindow containing the application
ApplicationWindow {

    id: application
    // title: qsTr("Hello World")
    width: 640
    height: 480
    visible: true
    color: "transparent"
    flags: Qt.FramelessWindowHint
    // flags: Qt.SplashScreen

    //menu containing two menu items
    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("&Open")
                onTriggered: console.log("Open action triggered");
            }
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }

    Rectangle{
        anchors.fill: parent
        color: "blue"
    }

    RectangularGlow {
            id: effect
            anchors.fill: application
            glowRadius: 10
            spread: 0.0
            color: "gray"
            cornerRadius: 10
    }

    //a button in the middle of the content area
    Button {
        text: qsTr("Hello World")
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        onClicked: {
        }
    }
}