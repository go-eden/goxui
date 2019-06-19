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
)

var (
	readerCallbackMap = make(map[string]func() string)
	writerCallbackMap = make(map[string]func(string))
	methodCallbackMap = make(map[string]func(string) string)
)

//export getField
func getField(cName *C.char) *C.char {
	name := C.GoString(cName)
	log.Debugf("get field[%v]", name)
	defer func() {
		if r := recover(); r != nil {
			log.Panicf("get field[%v] error: %v", name, r)
		}
	}()
	if reader, ok := readerCallbackMap[name]; ok && reader != nil {
		val := reader()
		log.Debugf("get field[%v] done: %v", name, val)
		return C.CString(val) // free in c
	} else {
		log.Debugf("get field[%v] failed, no reader", name)
	}
	return nil
}

//export setField
func setField(cName *C.char, cVal *C.char) {
	name := C.GoString(cName)
	val := C.GoString(cVal)
	log.Debugf("set field[%v]: %v", name, val)
	defer func() {
		if r := recover(); r != nil {
			log.Panicf("set field[%v] error: %v, %v", name, val, r)
		}
	}()
	if bs, err := base64.StdEncoding.DecodeString(val); err != nil {
		log.Errorf("setField %v failed, parse data [%v] failed: %v", name, val, err)
		return
	} else {
		val = string(bs)
	}
	if writer, ok := writerCallbackMap[name]; ok && writer != nil {
		writer(val)
		log.Debugf("set field[%v] done: %v", name, val)
	} else {
		log.Warnf("set field[%v] failed, no writer: %v", name, val)
	}
}

//export invoke
func invoke(cName *C.char, cData *C.char) *C.char {
	name := C.GoString(cName)
	data := C.GoString(cData)
	log.Debugf("invoke [%v] with args %v", name, data)
	defer func() {
		if r := recover(); r != nil {
			log.Panicf("invoke [%v] error: %v", name, r)
		}
	}()
	if bs, err := base64.StdEncoding.DecodeString(data); err == nil {
		data = string(bs)
	} else {
		log.Errorf("invoke [%v] failed, parse data [%v] failed: %v", name, data, err)
		return nil
	}
	if callback, ok := methodCallbackMap[name]; ok || callback != nil {
		result := callback(data)
		log.Debugf("invoke [%v] success with args %v, result: %v", name, data, result)
		return C.CString(result) // free in c
	} else {
		log.Warnf("invoke [%v] failed, invalid method.", name)
	}
	return nil
}

// Add a golang filed into Goxui's environment, must provide reader and writer callback.
func AddField(name string, fieldType QType, reader func() string, writer func(string)) bool {
	log.Infof("AddField: %v, %v", name, fieldType)
	cName := C.CString(name)
	cType := C.int(fieldType)
	cResult := C._ui_add_field(cName, cType)
	success := cResult != 0
	if success {
		readerCallbackMap[name] = reader
		writerCallbackMap[name] = writer
	} else {
		log.Warnf("addField failed: %s", name)
	}
	return success
}

// Add a golang method into Goxui's environment
func AddMethod(name string, retType QType, argNum int, callback func(string) string) bool {
	log.Infof("AddMethod: %v(%d)=>%v", name, argNum, retType)
	cName := C.CString(name)
	cType := C.int(retType)
	cArgNum := C.int(argNum)
	cResult := C._ui_add_method(cName, cType, cArgNum)
	success := cResult != 0
	if success {
		methodCallbackMap[name] = callback
	} else {
		log.Warnf("addMethod failed: %s", name)
	}
	return success
}
