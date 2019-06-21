package core

/*
#cgo darwin LDFLAGS: -L./darwin
#cgo darwin LDFLAGS: -F/usr/local/opt/qt/lib
#cgo darwin LDFLAGS: -framework Carbon
#cgo darwin LDFLAGS: -framework Cocoa
#cgo darwin LDFLAGS: -framework QtWidgets
#cgo darwin LDFLAGS: -framework QtQuick
#cgo darwin LDFLAGS: -framework QtGui
#cgo darwin LDFLAGS: -framework QtQml
#cgo darwin LDFLAGS: -framework QtNetwork
#cgo darwin LDFLAGS: -framework QtConcurrent
#cgo darwin LDFLAGS: -framework QtCore
#cgo darwin LDFLAGS: -lc++

#cgo LDFLAGS: -lgoxui
#include "goxui.h"

extern void uiLogger(int type, char *catagory, char* file, int line, char* msg);

static inline void _ui_bind_logger() {
	ui_set_logger(uiLogger);
}
*/
import "C"
import (
	"github.com/go-eden/slf4go"
)

var log = slog.GetLogger()

func init() {
	C._ui_bind_logger()
}

//export uiLogger
func uiLogger(cLevel C.int, cCategory *C.char, cFile *C.char, cLine C.int, cMsg *C.char) {
	l := int(cLevel)
	category := C.GoString(cCategory)
	file := C.GoString(cFile)
	line := int(cLine)
	msg := C.GoString(cMsg)
	switch l {
	case 0:
		log.Debugf("[%v] %v:%v %v", category, file, line, msg)
	case 1:
		log.Warnf("[%v] %v:%v %v", category, file, line, msg)
	case 2:
		log.Errorf("[%v] %v:%v %v", category, file, line, msg)
	case 3:
		log.Fatalf("[%v] %v:%v %v", category, file, line, msg)
	case 4:
		log.Infof("[%v] %v:%v %v", category, file, line, msg)
	}
}
