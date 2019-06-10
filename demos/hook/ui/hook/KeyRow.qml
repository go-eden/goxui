import QtQuick 2.12

Row {
    id: row
    property double size: 0

    height: 50
    width: parent.width
    visible: false

    Component.onCompleted: {
        for (var i=0; i<row.children.length; i++) {
            if (row.children[i].size) {
                row.size += row.children[i].size;
            }
        }
        row.visible = true
    }

}
