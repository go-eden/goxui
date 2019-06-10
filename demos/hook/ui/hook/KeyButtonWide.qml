import QtQuick 2.12

Item {
    id: root

    property double size: 1
    property string label: ""
    property int code: 0

    property alias fontSize: content.fontSize
    property alias align: content.align

    width: parent.width * (root.size / parent.size)
    height: parent.height

    KeyContent {
        id: content
        label: root.label
        fontSize: root.fontSize
    }

}
