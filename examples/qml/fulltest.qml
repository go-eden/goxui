import QtQuick 2.9
import QtQuick.Controls 2.2
import Goxui 1.0

Window {
    id : mainWindow
    visible: true
    width: 560
    height: 460
//    color: "transparent"
    Rectangle {
        id: main
        anchors.fill: parent
        color: "#F1F4F9"
        Rectangle {
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            width: 80
            height: 80
            color: "red"
            z: 1
        }
        TitleBar {
            width: 100
            height: 30
            x:10
            y:10
            Rectangle {
                anchors.fill: parent
                color: "#72F"
            }
        }

        property bool myFlag : Flag
        property int myNum : Root.Number
        property double myReal : Root.Body.Real
        property string myStr : Root.Str
        onMyFlagChanged: {
            console.log("onMyFlagChanged", myFlag)
        }
        onMyNumChanged: {
            console.log("onMyNumChanged", myNum)
        }
        onMyRealChanged: {
            console.log("onMyRealChanged", myReal)
        }
        onMyStrChanged : {
            console.log("onMyStrChanged", myStr)
        }
        Component.onCompleted: {
            console.log("===================== before");
            console.log("Flag: ", Flag);
            console.log("Root.Number: ", Root.Number);
            console.log("Root.Number2: ", Root.Number2);
            console.log("Root.Body.Real: ", Root.Body.Real);
            console.log("Root.Str: ", Root.Str);

            Flag = false;
            Flag = true;
            Flag = false;
            Flag = true;
            Root.Number = 1024;
            Root.Number2 = 2345654345676543456;
            Root.Body.Real = 1111.2344;
            Root.Str = "hahah哈哈";
//
            console.log("===================== after");
            console.log("Flag: ", Flag);
            console.log("Root.Number: ", Root.Number);
            console.log("Root.Number2: ", Root.Number2);
            console.log("Root.Body.Real: ", Root.Body.Real);
            console.log("Root.Str: ", Root.Str);

            console.log("===================== test method");
            console.log(Test(1990, 34.9));
            Test(2000, 44.2324, function(data) {
                console.log("Test异步回调：", JSON.stringify(data));
            });
            Root.Test(undefined, "fdsfds", {test:true}, function(data){
                console.log("Root.Test异步回调：", JSON.stringify(data));
            });
            var result = Root.Body.Test(0.22, null, {param:'ff'})
            console.log("Root.Body.Test同步结果：", result, JSON.stringify(result));
            Root.Body.Test(0.22, null, {param:'ff'}, function(data){
                console.log("Root.Body.Test异步回调：", data, JSON.stringify(data));
            });
            Root.Test0(null, null, function(data){
                console.log("Root.Test0", data);
            });
            Root.Test1(null, null, function(data){
                console.log("Root.Test1", data);
            });
            Root.Test2(null, null, function(data){
                console.log("Root.Test2", data);
            });
            Root.Body.Test1(0.22, null, {param:'ff'}, function(data){
                console.log("Root.Body.Test1异步回调：", data, JSON.stringify(data));
            });
            Root.Test3("哈哈", {name:"你好", age: 9999});
        }
        Event {
            key: "event_bool"
            onActive: function(data){
                console.log("event_bool", data);
            }
        }
        Event {
            key: "event_int"
            onActive: function(data){
                console.log("event_int", data);
            }
        }
        Event {
            key: "event_long"
            onActive: function(data){
                console.log("event_long", data);
            }
        }
        Event {
            key: "event_double"
            onActive: function(data){
                console.log("event_double", data);
            }
        }
        Event {
            key: "event_string"
            onActive: function(data){
                console.log("event_string", data);
            }
        }
        Event {
            key: "event_object"
            onActive: function(data){
                console.log("event_object", data, JSON.stringify(data));
            }
        }
        Event {
            key: "event_array"
            onActive: function(data) {
                console.log("event_array", data, JSON.stringify(data));
            }
        }
    }
}