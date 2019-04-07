import QtQuick 2.9
import QtQuick.Controls 2.2
import QtWebEngine 1.6
import Goxui 1.0

Window {
    id: mainWindow
    visible: true
    width: 900
    height: 720
    WebEngineView {
        anchors.fill: parent
        url: "https://www.bing.com"
    }
}
