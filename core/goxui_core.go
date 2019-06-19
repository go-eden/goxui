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

extern void logger(int l, char *msg);

static inline void bindLogger() {
	ui_set_logger(logger);
}
*/
import "C"
import "github.com/go-eden/slf4go"

var log = slog.GetLogger()

func init() {
	C.bindLogger()
}

//export logger
func logger(cLevel C.int, cMsg *C.char) {
	l := int(cLevel)
	msg := C.GoString(cMsg)
	switch l {
	case 0:
		log.Debug(msg)
	case 1:
		log.Warn(msg)
	case 2:
		log.Error(msg)
	case 3:
		log.Fatal(msg)
	case 4:
		log.Infof(msg)
	}
}
