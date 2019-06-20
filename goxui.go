package goxui

import (
	"github.com/go-eden/goxui/core"
	"github.com/go-eden/goxui/util"
	"github.com/go-eden/slf4go"
	"os"
	"reflect"
)

var fields []field
var methods []method

var log = slog.GetLogger()

// Initilize ui context, and QApplication.
func Init() {
	core.Init()
}

// Enable single application configuration
func EnableSingleApplication() {
	_ = os.Setenv("GOXUI_SINGLE_APPLICATION", "1")
}

// Add the specified RCC into Qt resources system.
func AddResourceData(prefix string, data []byte) {
	core.AddResourceData(prefix, data)
}

// Add the specified path into Qt resource path.
func AddResourcePath(path string) {
	core.AddResourcePath(path)
}

// Add the specified path into Qt's import path, could be used for identified modules.
func AddImportPath(path string) {
	core.AddImportPath(path)
}

// Add new resource's mapping role.
// <d>Unsupported： In QML, could use `${prefix}:` to locate resource directly.</d>
func MapResource(prefix string, path string) {
	core.MapResource(prefix, path)
}

// TOOL: setup application's http proxy, only for Qt side.
func SetHttpProxy(host string, port int) {
	core.ToolSetHttpProxy(host, port)
}

// TOOL: setup whether enable debug level log or not.
func SetDebugEnabled(enable bool) {
	core.ToolSetDebugEnabled(enable)
}

// UI's entry-point, will block until fail or exit.
func Start(root string) int {
	log.Debugf("Start: %v", root)
	return core.Start(root)
}

// Trigger the named event, with specified data.
func TriggerEvent(name string, data interface{}) {
	dtype := core.ParseQType(reflect.TypeOf(data))
	_data := util.ToString(data)
	log.Debugf("TriggerEvent: %v, %v", name, _data)
	core.TriggerEvent(name, dtype, _data)
}

// Flush add fields, notify Qt if value changed.
func Flush() {
	for _, f := range fields {
		if !f.checkChanged() {
			continue
		}
		core.NotifyField(f.fullname) // notify value changed
	}
}

// Bind the specified object into QML side, them will be exposed in QML context.
func BindObject(obj interface{}) {
	var fields []field
	var methods []method
	var success bool
	if fields, methods, success = scanMetaData(reflect.TypeOf(obj)); !success {
		log.Warnf("scan metadata of object[%v] failed.", obj)
		return
	}
	for i := range fields {
		fields[i].root = obj
		core.AddField(fields[i].fullname, fields[i].qtype, fields[i].getter, fields[i].setter)
		log.Debugf("bind field: [%v], [%v]", fields[i].fullname, fields[i].qtype)
	}
	for i := range methods {
		methods[i].root = obj
		core.AddMethod(methods[i].fullname, methods[i].otype, methods[i].inum, methods[i].invoke)
		log.Debugf("bind method: %v(%v) => %v", methods[i].fullname, methods[i].inum, methods[i].otype)
	}
}
