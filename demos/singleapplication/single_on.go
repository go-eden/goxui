package main

import (
	"github.com/go-eden/goxui"
	"runtime"
)

func main() {
	runtime.LockOSThread()

	goxui.Init()

	goxui.EnableSingleApplication()

	goxui.StartRelative("ui", "singleapp.qml")
}
