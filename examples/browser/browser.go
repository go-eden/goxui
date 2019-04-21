package main

import (
	"github.com/sisyphsu/goxui"
	_ "github.com/sisyphsu/goxui/ext/web"
	"path/filepath"
	"runtime"
)

// test goxui web
func main() {
	runtime.LockOSThread()

	_, filename, _, _ := runtime.Caller(0)
	path := filepath.Dir(filename)

	goxui.Init()

	goxui.Start(filepath.Join(path, "ui", "BrowserWindow.qml"))
}
