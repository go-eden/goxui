package core

/*
#include <stdlib.h>
#include <stdio.h>
#include "goxui.h"

static void* allocArgv(int argc) {
    return malloc(sizeof(char *) * argc);
}
*/
import "C"
import (
	"fmt"
	"github.com/go-eden/goxui/util"
	"os"
	"reflect"
	"unsafe"
)

var initCallbacks []func()

func AddInitCallback(f func()) {
	initCallbacks = append(initCallbacks, f)
}

// Forward Goxui's ui_init method, it will init Goxui and invoke ext's initCallback.
func Init() {
	argv := os.Args
	argc := C.int(len(argv))
	cArgv := (*[0xfff]*C.char)(C.allocArgv(argc))
	for i, arg := range argv {
		cArgv[i] = C.CString(arg)
	}
	C.ui_init(argc, (**C.char)(unsafe.Pointer(cArgv)))
	// invoke callback
	if len(initCallbacks) > 0 {
		for _, initCallback := range initCallbacks {
			initCallback()
		}
	}
}

// Forward Goxui's ui_add_resource method, it will add RCC data into Qt resources system.
func AddResourceData(prefix string, data []byte) {
	cPrefix := C.CString(prefix)
	C.ui_add_resource(cPrefix, (*C.char)(unsafe.Pointer(&data[0])))
}

// Forward Goxui's ui_add_resource_path method, it will add the specified path into resource path.
func AddResourcePath(path string) {
	cPath := C.CString(path)
	defer C.free(unsafe.Pointer(cPath))
	C.ui_add_resource_path(cPath)
}

// Forward Goxui's ui_add_import_path method, it could be used in identified modules
func AddImportPath(path string) {
	cPath := C.CString(path)
	defer C.free(unsafe.Pointer(cPath))
	C.ui_add_import_path(cPath)
}

// Forward Goxui's ui_map_resource method, it will register the specified resource into QML.
func MapResource(prefix string, path string) {
	cPrefix := C.CString(prefix)
	cPath := C.CString(path)
	defer C.free(unsafe.Pointer(cPrefix))
	defer C.free(unsafe.Pointer(cPath))
	C.ui_map_resource(cPrefix, cPath)
}

// Forward Goxui's ui_tool_set_http_proxy method, it will setup the Qt's network proxy configuration.
func ToolSetHttpProxy(host string, port int) {
	cHost := C.CString(host)
	defer C.free(unsafe.Pointer(cHost))
	C.ui_tool_set_http_proxy(cHost, C.int(port))
}

// Forward Goxui's ui_tool_set_debug_enabled method, it will enable some debug features or not.
func ToolSetDebugEnabled(enable bool) {
	if enable {
		C.ui_tool_set_debug_enabled(C.int(1))
	} else {
		C.ui_tool_set_debug_enabled(C.int(0))
	}
}

// Forward Goxui's ui_trigger_event method, it will trigger the specified event.
func TriggerEvent(name string, data interface{}) {
	dtype := ParseQType(reflect.TypeOf(data))
	_data := util.ToString(data)
	cName := C.CString(name)
	cType := C.int(dtype)
	cData := C.CString(_data)
	defer C.free(unsafe.Pointer(cName))
	defer C.free(unsafe.Pointer(cData))
	C.ui_trigger_event(cName, cType, cData)
}

// Forward Goxui's ui_notify_field method, notify the specified property is changed.
func NotifyField(name string) bool {
	cName := C.CString(name)
	defer C.free(unsafe.Pointer(cName))
	cResult := C.ui_notify_field(cName)
	success := cResult != 0
	if !success {
		fmt.Printf("notifyField failed: %s", name)
	}
	return success
}

// Forward Goxui's ui_start method, will block until fail or exit.
func Start(root string) int {
	cRoot := C.CString(root)
	defer C.free(unsafe.Pointer(cRoot))
	cCode := C.ui_start(cRoot)
	return int(cCode)
}
