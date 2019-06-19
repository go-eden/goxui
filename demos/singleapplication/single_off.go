package main

import (
	"fmt"
	"github.com/go-eden/goxui"
	"os"
	"runtime"
)

func main() {
	runtime.LockOSThread()

	if err := os.Setenv("GOXUI_SINGLE_APPLICATION", "0"); err != nil {
		fmt.Println("setenv error: ", err)
		return
	}

	goxui.Init()

	goxui.StartRelative("ui", "singleapp.qml")
}
