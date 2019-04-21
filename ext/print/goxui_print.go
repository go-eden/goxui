package print

/*
#cgo LDFLAGS: -L./darwin -lgoxui-print

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
#cgo darwin LDFLAGS: -framework QtPrintSupport

#include "goxui_print.h"
*/
import "C"
import "github.com/sisyphsu/goxui/core"

func init() {
	core.AddInitCallback(doInit)
}

func doInit() {
	C.ui_init_print()
}
