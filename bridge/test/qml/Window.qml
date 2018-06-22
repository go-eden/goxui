import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import UILib 1.0
import QtWebEngine 1.5

//ApplicationWindow {
Window {
    visible: true
    x: 100
    y: 100
    width: 880
    height: 610
    color: "#EEE"
    TitleBar {
//    Rectangle{
        anchors.top: parent.top
        width: parent.width
        height: 40
//        Rectangle {
//            anchors.fill: parent
//            color: "#FFF"
//        }
        Row {
            anchors.centerIn: parent
            Text {
                text: "Title"
                enabled: false
            }
            TextField{
                height: 25
                width: 60
                text: "text"
            }
        }
    }
    WebEngineView {
        anchors.fill: parent
        anchors.topMargin: 40
        url: "https://www.baidu.com"
    }
}
