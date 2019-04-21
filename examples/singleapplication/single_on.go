package main

import (
	"github.com/sisyphsu/goxui"
	"runtime"
)

func main() {
	runtime.LockOSThread()

	goxui.Init()

	goxui.StartRelative("ui", "singleapp.qml")
}
