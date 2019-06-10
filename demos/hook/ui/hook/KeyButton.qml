import QtQuick 2.12

Item {
    id: root

    property double size: 1
    property string label: ""
    property alias fontSize: content.fontSize
    property int code: 0

    width: parent.width * (root.size / parent.size)
    height: parent.height

    KeyContent {
        id: content
        label: root.label
        code: root.code
    }

}
