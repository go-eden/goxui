import QtQuick 2.9
import QtQuick.Controls 2.2
import UILib 1.0

Window {
    id: mainWindow
    visible: true
    width: 560
    height: 460
    color: "transparent"
    Rectangle {
        id: main
        anchors.fill: parent
        color: "#F1F4F9"
        property bool myFlag: Flag
        property int myNum: Root.number
        property double myReal: Root.body.real
        property string myStr: Root.str
        Rectangle {
            anchors {
                right: parent.right
                bottom: parent.bottom
            }
            width: 80
            height: 80
            color: "red"
            z: 1
        }
        onMyFlagChanged: {
            console.log("onMyFlagChanged", myFlag)
        }
        onMyNumChanged: {
            console.log("onMyNumChanged", myNum)
        }
        onMyRealChanged: {
            console.log("onMyRealChanged", myReal)
        }
        onMyStrChanged: {
            console.log("onMyStrChanged", myStr)
        }
        Component.onCompleted: {
            console.log("Root", Root)
            console.log("System", System)
            console.log("===================== before")
            console.log("Flag: ", Flag)
            console.log("Root.number: ", Root.number)
            console.log("Root.number2: ", Root.number2)
            console.log("Root.body.real: ", Root.body.real)
            console.log("Root.str: ", Root.str)

            Flag = false
            Flag = true
            Flag = false
            Flag = true
            Root.number = 1024
            Root.number2 = 2345654345676543456
            Root.body.real = 1111.2344
            Root.str = "hahah哈哈"

            console.log("===================== after")
            console.log("Flag: ", Flag)
            console.log("Root.number: ", Root.number)
            console.log("Root.number2: ", Root.number2)
            console.log("Root.body.real: ", Root.body.real)
            console.log("Root.str: ", Root.str)

            console.log("===================== test method")
            console.log(Test(1990, 34.9))
            Test(2000, 44.2324, function (data) {
                console.log("Test异步回调：", data)
            })
            Root.Test(undefined, "fdsfds", {
                          test: true
                      }, function (data) {
                          console.log("Root.Test异步回调：", data)
                      })
            var result = Root.Body.Test(0.22, null, {
                                            param: 'ff'
                                        })
            console.log("Root.Body.Test同步结果：", result, JSON.stringify(result))
            Root.Body.Test(0.22, null, {
                               param: 'ff'
                           }, function (data) {
                               console.log("Root.Body.Test异步回调：", data,
                                           JSON.stringify(data))
                           })
            Root.Test0(null, null, function (data) {
                console.log("Root.Test0", data)
            })
            Root.Test1(null, null, function (data) {
                console.log("Root.Test1", data)
            })
            Root.Test2(null, null, function (data) {
                console.log("Root.Test2", data)
            })
            Root.Body.Test1(0.22, null, {
                                param: 'ff'
                            }, function (data) {
                                console.log("Root.Body.Test1异步回调：", data,
                                            JSON.stringify(data))
                            })
            
            console.log("Start waitFor")
            System.waitFor("testttttttt")
            console.log("Done waitFor")
        }
        Event {
            key: "event_bool"
            onActive: function (data) {
                console.log("event_bool", data)
            }
        }
        Event {
            key: "event_int"
            onActive: function (data) {
                console.log("event_int", data)
            }
        }
        Event {
            key: "event_long"
            onActive: function (data) {
                console.log("event_long", data)
            }
        }
        Event {
            key: "event_double"
            onActive: function (data) {
                console.log("event_double", data)
            }
        }
        Event {
            key: "event_string"
            onActive: function (data) {
                console.log("event_string", data)
            }
        }
        Event {
            key: "event_object"
            onActive: function (data) {
                console.log("event_object", data, JSON.stringify(data))
            }
        }
        Event {
            key: "event_array"
            onActive: function (data) {
                console.log("event_array", data, JSON.stringify(data))
            }
        }
    }
}
