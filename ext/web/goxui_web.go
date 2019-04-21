package web

/*
#cgo darwin LDFLAGS: -L./darwin
#cgo darwin LDFLAGS: -F/usr/local/opt/qt/lib
#cgo darwin LDFLAGS: -framework Carbon
#cgo darwin LDFLAGS: -framework Cocoa
#cgo darwin LDFLAGS: -lc++
#cgo darwin LDFLAGS: -framework QtCore
#cgo darwin LDFLAGS: -framework QtWidgets
#cgo darwin LDFLAGS: -framework QtQuick
#cgo darwin LDFLAGS: -framework QtGui
#cgo darwin LDFLAGS: -framework QtQml
#cgo darwin LDFLAGS: -framework QtNetwork
#cgo darwin LDFLAGS: -framework QtConcurrent
#cgo darwin LDFLAGS: -framework QtWebEngine

#cgo LDFLAGS: -lgoxui-web

#include "goxui_web.h"
*/
import "C"
import "github.com/sisyphsu/goxui/core"

func init() {
	core.AddInitCallback(doInit)
}

func doInit() {
	C.ui_init_web()
}
