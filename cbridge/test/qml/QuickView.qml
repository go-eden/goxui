import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import Qt.labs.platform 1.0

Rectangle {
    id: main
   height:300;
   width:500;
   color:"green";

   border.width:4;
   border.color:"#008080";
//   radius:10;
   opacity:1;       //区分颜色透明度和窗口透明度
   SystemTrayIcon {
        id: tray
       visible: true
       iconSource: "/Users/sulin/workspace/go/src/91byte/labor/desktop/asserts/qml/images/icon.svg"
       onMessageClicked: console.log("Message clicked")
       onActivated: {
            tray.showMessage("hhh", "mmmmmmmmmmmmmmmmm", null, 3)
       }
   }
    MouseArea {
       anchors.fill: parent
       onClicked: {
        main.width = main.width + 10
        main.height = main.height + 10
       }
   }
   Window { x: 100; y: 100; width: 300; height: 300; visible:true; color:"red";}
}