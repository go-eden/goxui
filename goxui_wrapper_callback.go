package goxui

/*
#include <stdlib.h>
#include <stdio.h>
#include "goxui.h"

extern char *getField(char *name);
extern void setField(char *name, char *data);
extern char *invoke(char *name, char *params);

static inline int _ui_add_field(char *name, int type) {
    return ui_add_field(name, type, getField, setField);
}

static inline int _ui_add_method(char *name, int retType, int argNum) {
    return ui_add_method(name, retType, argNum, invoke);
}
*/
import "C"
import (
	"encoding/base64"
	"unsafe"
)

type ui_type int

const (
	UI_TYPE_VOID ui_type = iota
	UI_TYPE_BOOL
	UI_TYPE_INT
	UI_TYPE_LONG
	UI_TYPE_DOUBLE
	UI_TYPE_OBJECT
	UI_TYPE_STRING
)

func (t ui_type) String() string {
	switch t {
	case UI_TYPE_VOID:
		return "void"
	case UI_TYPE_BOOL:
		return "bool"
	case UI_TYPE_INT:
		return "int"
	case UI_TYPE_LONG:
		return "long"
	case UI_TYPE_DOUBLE:
		return "double"
	case UI_TYPE_OBJECT:
		return "object"
	case UI_TYPE_STRING:
		return "string"
	default:
		return "unknown"
	}
}

var (
	fieldReaderMap    = make(map[string]func() string)
	fieldWriterMap    = make(map[string]func(string))
	methodCallbackMap = make(map[string]func(string) string)
)

//export getField
func getField(cName *C.char) *C.char {
	name := C.GoString(cName)
	defer func() {
		defer func() {
			if r := recover(); r != nil {
				logger.ErrorF("getfield[%v] failed, panic occured: %v", name, r)
			}
		}()
	}()
	if reader, ok := fieldReaderMap[name]; ok {
		return C.CString(reader()) // free in c
	} else {
		logger.WarnF("invalid field, no reader: %v", name)
	}
	return nil
}

//export setField
func setField(cName *C.char, cVal *C.char) {
	name := C.GoString(cName)
	val := C.GoString(cVal)
	if bs, err := base64.StdEncoding.DecodeString(val); err == nil {
		val = string(bs)
	} else {
		logger.WarnF("setField %v failed, parse data [%v] failed", name, val, err)
		return
	}
	defer func() {
		defer func() {
			if r := recover(); r != nil {
				logger.ErrorF("setField[%v] failed with param[%v], panic occured: %v", name, val, r)
			}
		}()
	}()
	if writer, ok := fieldWriterMap[name]; ok {
		writer(val)
	} else {
		logger.WarnF("invalid field, no writer: %v", name)
	}
}

//export invoke
func invoke(cName *C.char, cData *C.char) *C.char {
	name := C.GoString(cName)
	data := C.GoString(cData)
	if bs, err := base64.StdEncoding.DecodeString(data); err == nil {
		data = string(bs)
	} else {
		logger.WarnF("invoke %v failed, parse data [%v] failed", name, data, err)
		return nil
	}
	defer func() {
		defer func() {
			if r := recover(); r != nil {
				logger.ErrorF("invoke [%v] failed with param[%v], panic occured: %v", name, data, r)
			}
		}()
	}()
	if callback, ok := methodCallbackMap[name]; ok {
		return C.CString(callback(data)) // free in c
	} else {
		logger.WarnF("invalid method: %v", name)
	}
	return nil
}

// 封装C接口中的ui_add_field函数, 向UI中新增一个变量
func addField(name string, fieldType ui_type, reader func() string, writer func(string)) bool {
	cName := C.CString(name)
	cType := C.int(fieldType)
	cResult := C._ui_add_field(cName, cType)
	success := cResult != 0
	if success {
		fieldReaderMap[name] = reader
		fieldWriterMap[name] = writer
	} else {
		logger.WarnF("addField failed: %s", name)
	}
	return success
}

// 封装C接口中的ui_add_method函数, 向UI中新增一个函数
func addMethod(name string, retType ui_type, argNum int, callback func(string) string) bool {
	cName := C.CString(name)
	cType := C.int(retType)
	cArgNum := C.int(argNum)
	cResult := C._ui_add_method(cName, cType, cArgNum)
	success := cResult != 0
	if success {
		methodCallbackMap[name] = callback
	} else {
		logger.WarnF("addMethod failed: %s", name)
	}
	return success
}

// 封装C中的ui_notify_field函数, 通知UI中某个属性已更新
func notifyField(name string) bool {
	cName := C.CString(name)
	defer C.free(unsafe.Pointer(cName))
	cResult := C.ui_notify_field(cName)
	success := cResult != 0
	if !success {
		logger.WarnF("notifyField failed: %s", name)
	}
	return success
}
