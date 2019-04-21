package main

import (
	"github.com/sisyphsu/goxui"
	_ "github.com/sisyphsu/goxui/ext/web"
	"runtime"
)

// test goxui web
func main() {
	runtime.LockOSThread()

	goxui.Init()

	goxui.StartRelative("ui", "BrowserWindow.qml")
}
