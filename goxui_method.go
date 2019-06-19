package goxui

import (
	"encoding/json"
	"github.com/go-eden/goxui/core"
	"github.com/go-eden/goxui/util"
	"reflect"
)

// Goxui method's metadata
type method struct {
	name     string      // method's name, like 'EnableNotice'
	fullname string      // method's fullname, like 'User.Setting.EnableNotice'
	root     interface{} // method's root instance relative to `fullname`
	otype    core.QTYPE  // method's return type, OBJECT for multi-out
	inum     int         // method's input number
}

// Wrap method's invocation, handle input & output parameters's serialization and deserialization.
func (m *method) invoke(param string) (result string) {
	defer func() {
		if r := recover(); r != nil {
			log.Warnf("invoke [%v] failed, panic occured: %v", m.fullname, r)
		}
	}()
	// find owner
	owner := util.FindOwner(reflect.ValueOf(m.root), m.fullname)
	if owner.Kind() != reflect.Struct {
		log.Warnf("invoke [%v] failed, can't find owner Struct", m.fullname)
		return
	}
	// find method
	methodVal := owner.Addr().MethodByName(m.name)
	if methodVal.Kind() != reflect.Func {
		log.Warnf("invoke [%v] failed, invalid func", m.fullname)
		return
	}
	// deserialization args array
	var args []interface{}
	if err := json.Unmarshal([]byte(param), &args); err != nil {
		log.Warnf("invoke [%v] failed, parse args[%v] failed: %v", m.fullname, param, err)
		return
	}
	if len(args) != methodVal.Type().NumIn() {
		log.Warnf("invoke [%v] failed, number of args error: %v", m.fullname, param)
		return
	}
	// prepare input parameters
	argValues := make([]reflect.Value, methodVal.Type().NumIn())
	for i := 0; i < methodVal.Type().NumIn(); i++ {
		argType := methodVal.Type().In(i)
		arg := args[i]
		if argVal, err := util.ConvertToValue(argType, arg); err != nil {
			log.Warnf("invoke [%v] failed, can't resolve argument[%v] as [%v]: %v", m.fullname, arg, argType, err)
			return
		} else {
			argValues[i] = argVal
		}
	}
	// invoke real method
	if vals := methodVal.Call(argValues); len(vals) == 1 {
		result = util.ToString(vals[0].Interface())
	}
	log.Debugf("invoke [%v] success with args %v, result: %v", m.fullname, param, result)
	return
}
