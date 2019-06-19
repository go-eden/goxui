package main

import (
	"github.com/go-eden/goxui"
	_ "github.com/go-eden/goxui/ext/web"
	"runtime"
)

// test goxui web
func main() {
	runtime.LockOSThread()

	goxui.Init()

	goxui.StartRelative("ui", "App.qml")
}
