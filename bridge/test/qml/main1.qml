import QtQuick 2.3
import QtQuick.Window 2.2
import QtWebEngine 1.5

// main UIWindow
Rectangle {
    color: "blue"
    anchors.fill: parent

    WebEngineView {
	    anchors.fill: parent
	    url : "https://www.baidu.com"
	}

}