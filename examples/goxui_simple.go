package main

import (
    "runtime"
    "github.com/sisyphsu/goxui"
    "path/filepath"
)

/**
  uilib测试程序
 */
func main() {
    runtime.LockOSThread()
    
    _, filename, _, _ := runtime.Caller(0)
    path := filepath.Dir(filename)
    
    status := &Status{}
    goxui.Init(status)
    //go func() {
    //    time.Sleep(time.Second * 10)
    //    goxui.TriggerEvent("event_bool", true)
    //    goxui.TriggerEvent("event_int", 10000)
    //    goxui.TriggerEvent("event_long", 10000000)
    //    goxui.TriggerEvent("event_double", 10000.4444)
    //    goxui.TriggerEvent("event_string", "fdasfadsfasdfdafdsafdsa")
    //    goxui.TriggerEvent("event_object", Param{"啦啦啦", 3333333})
    //    goxui.TriggerEvent("event_object", []Param{{"啦啦啦", 3333333}, {"啦啦啦444", 3333333}})
    //}()
    //path = "Z:\\sulin\\workspace\\go\\src\\github.com\\sisyphsu\\goxui\\examples"
    goxui.MapResource("img", path)
    goxui.Start(filepath.Join(path, "qml", "simple.qml"))
}