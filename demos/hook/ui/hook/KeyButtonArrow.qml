import QtQuick 2.12

Column {

    id: root

    property double size: 1

    width: parent.width * (root.size / parent.size)
    height: parent.height
    spacing: 3

    Item {
        width: parent.width
        height: (parent.height - parent.spacing) * 0.5
        KeyContent {
            label: "▲"
            fontSize: 9
            code: 38
            anchors.bottomMargin: 0
        }
    }

    Item {
        width: parent.width
        height: (parent.height - parent.spacing) * 0.5
        KeyContent {
            label: "▼"
            fontSize: 9
            code: 40
            anchors.topMargin: 0
        }
    }

}
