package core

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
	"fmt"
)

var (
	readerCallbackMap = make(map[string]func() string)
	writerCallbackMap = make(map[string]func(string))
	methodCallbackMap = make(map[string]func(string) string)
)

//export getField
func getField(cName *C.char) *C.char {
	name := C.GoString(cName)
	defer func() {
		defer func() {
			if r := recover(); r != nil {
				fmt.Printf("getfield[%v] failed, panic occured: %v", name, r)
			}
		}()
	}()
	if reader, ok := readerCallbackMap[name]; ok {
		return C.CString(reader()) // free in c
	} else {
		fmt.Printf("invalid field, no reader: %v", name)
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
		fmt.Printf("setField %v failed, parse data [%v] failed: %v", name, val, err)
		return
	}
	defer func() {
		defer func() {
			if r := recover(); r != nil {
				fmt.Printf("setField[%v] failed with param[%v], panic occured: %v", name, val, r)
			}
		}()
	}()
	if writer, ok := writerCallbackMap[name]; ok {
		writer(val)
	} else {
		fmt.Printf("invalid field, no writer: %v", name)
	}
}

//export invoke
func invoke(cName *C.char, cData *C.char) *C.char {
	name := C.GoString(cName)
	data := C.GoString(cData)
	if bs, err := base64.StdEncoding.DecodeString(data); err == nil {
		data = string(bs)
	} else {
		fmt.Printf("invoke %v failed, parse data [%v] failed: %v", name, data, err)
		return nil
	}
	defer func() {
		defer func() {
			if r := recover(); r != nil {
				fmt.Printf("invoke [%v] failed with param[%v], panic occured: %v", name, data, r)
			}
		}()
	}()
	if callback, ok := methodCallbackMap[name]; ok {
		return C.CString(callback(data)) // free in c
	} else {
		fmt.Printf("invalid method: %v", name)
	}
	return nil
}

// 封装C接口中的ui_add_field函数, 向UI中新增一个变量
func AddField(name string, fieldType Q_TYPE, reader func() string, writer func(string)) bool {
	cName := C.CString(name)
	cType := C.int(fieldType)
	cResult := C._ui_add_field(cName, cType)
	success := cResult != 0
	if success {
		readerCallbackMap[name] = reader
		writerCallbackMap[name] = writer
	} else {
		fmt.Printf("addField failed: %s", name)
	}
	return success
}

// 封装C接口中的ui_add_method函数, 向UI中新增一个函数
func AddMethod(name string, retType Q_TYPE, argNum int, callback func(string) string) bool {
	cName := C.CString(name)
	cType := C.int(retType)
	cArgNum := C.int(argNum)
	cResult := C._ui_add_method(cName, cType, cArgNum)
	success := cResult != 0
	if success {
		methodCallbackMap[name] = callback
	} else {
		fmt.Printf("addMethod failed: %s", name)
	}
	return success
}