import QtQuick 2.12
import QtGraphicalEffects 1.12

Item {

    id: root

    property color front: "#2a2c2f"
    property color background: "#e1e1e1"
    property string label: ""
    property int code: 0
    property int fontSize: 14
    property double score: Math.random()
    property alias align: label.horizontalAlignment

    anchors.fill: parent
    anchors.margins: 3

    Rectangle {
        id: bg
        color: root.background
        anchors.fill: parent
        radius: 3
    }

    Rectangle {
        id: tm
        color: "#ec6762"
        anchors.fill: parent
        opacity: 0
        radius: 3
    }

    Text {
        id: label
        text: root.label
        color: root.front
        font.pixelSize: root.fontSize
        anchors.fill: parent
        anchors.margins: 18
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    Component.onCompleted: culBackground()

    function culBackground() {
        tm.opacity = root.score
//        if (root.score < 0.2) {
//            tm.opacity = root.score * 2
//            tm.color = "#656b7c"
//        } else if (root.score < 0.4) {
//            tm.opacity = (root.score - 0.2) * 2
//            tm.color = "#00f"
//        } else if (root.score < 0.6) {
//            tm.opacity = (root.score - 0.4) * 2
//            tm.color = "#0f0"
//        } else if (root.score < 0.8) {
//            tm.opacity = (root.score - 0.6) * 2
//            tm.color = "#ff0"
//        } else {
//            tm.opacity = (root.score - 0.8) * 2
//            tm.color = "red"
//        }
    }

}
