import QtQuick 2.3
import QtQuick.Window 2.2

// main UIWindow
Rectangle {
    color: "blue"
    width: 700
    height: 560
    MouseArea {
        anchors.fill: parent
        onClicked: {
            console.log("js.result: ", Event.trigger("to_c", "hello, 我是 QML-JS"));
        }
    }

    function callback(data){
        console.log("js.onCallback: ", data);
        return "js.receive";
    }

    Component.onCompleted: {
        Event.bind("to_qml", 2000, callback);
    }

	Rectangle {
		anchors.fill: parent
		Text {
			anchors.centerIn: parent
			text: "Hello World"
		}
	}

}