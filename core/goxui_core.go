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

extern void uiLogger(int type, char* file, int line, char* msg);

static inline void _ui_bind_logger() {
	ui_set_logger(uiLogger);
}
*/
import "C"
import (
	"github.com/go-eden/slf4go"
	"path/filepath"
	"strconv"
	"strings"
)

var log = slog.GetLogger()
var nativeLog = slog.NewLogger("goxui/core")
var qmlLog = slog.NewLogger("goxui/qml")

func init() {
	C._ui_bind_logger()
}

//export uiLogger
func uiLogger(cLevel C.int, cFile *C.char, cLine C.int, cMsg *C.char) {
	file := C.GoString(cFile)
	line := int(cLine)
	msg := C.GoString(cMsg)
	// fix message
	if len(file) > 0 && strings.HasPrefix(msg, file) && len(msg) > len(file)+1 {
		msg = msg[len(file)+1:]
		if linePrefix := strconv.Itoa(line) + ":"; strings.HasPrefix(msg, linePrefix) {
			msg = msg[len(linePrefix):]
		}
	}
	// fix file
	if len(file) > 0 {
		file = filepath.Base(file)
	}
	// custom stack
	stack := &slog.Stack{
		Package:  "goxui-cbridge",
		Function: "unknown",
	}
	// choice logger
	var log *slog.Logger
	if file == "" && line == 0 {
		log = nativeLog
		stack.Line = 0
		stack.Filename = "unknown"
	} else {
		log = qmlLog
		stack.Line = line
		stack.Filename = file
	}
	switch int(cLevel) {
	case 0:
		log.Debug(stack, msg)
	case 1:
		log.Warn(stack, msg)
	case 2:
		log.Error(stack, msg)
	case 3:
		log.Error(stack, msg)
	case 4:
		log.Info(stack, msg)
	}
}
