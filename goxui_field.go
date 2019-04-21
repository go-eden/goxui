package goxui

import (
	"encoding/json"
	"github.com/sisyphsu/goxui/core"
	"github.com/sisyphsu/goxui/util"
	"reflect"
)

// Goxui field's metadata
type field struct {
	name     string      // field's name, like 'IsLogin'
	fullname string      // field's fullname, like 'User.IsLogin'
	root     interface{} // field's root object, relative to 'fullname'
	cache    *string     // field's value cache, used to judge whether it changes
	qtype    core.Q_TYPE // field's Q_TYPE
}

// getter used to get the field's value, and update cache.
func (f *field) getter() (v string) {
	defer func() {
		if r := recover(); r != nil {
			log.WarnF("get field[%v] failed, panic occured: %v", f.fullname, r)
		}
	}()
	// find owner
	owner := util.FindOwner(reflect.ValueOf(f.root), f.fullname)
	if owner.Kind() != reflect.Struct {
		log.WarnF("get field[%v] failed, can't find owner Struct", f.fullname)
		return
	}
	// check it's real Get method
	m := owner.Addr().MethodByName("Get" + f.name)
	if m.Kind() == reflect.Func && m.Type().NumIn() == 0 && m.Type().NumOut() == 1 && core.ParseQType(m.Type().Out(0)) == f.qtype {
		results := m.Call([]reflect.Value{})
		v = util.ToString(results[0].Interface())
	} else {
		fieldV := owner.FieldByName(f.name)
		v = util.ToString(fieldV.Interface())
	}
	log.DebugF("get field[%v] done: %v", f.fullname, v)
	// update cache
	f.cache = &v

	return
}

// setter used to set the field's value
func (f *field) setter(v string) {
	defer func() {
		if r := recover(); r != nil {
			log.WarnF("set field[%v] with param[%v] failed, panic occured: %v", f.fullname, v, r)
		}
	}()
	// find owner
	owner := util.FindOwner(reflect.ValueOf(f.root), f.fullname)
	for owner.Kind() != reflect.Struct {
		log.WarnF("set field[%v] failed, can't find owner Struct", f.fullname)
		return
	}
	// convert input argument
	var tmp interface{}
	if err := json.Unmarshal([]byte(v), &tmp); err != nil {
		tmp = v
	}
	// check it's real Set method.
	m := owner.Addr().MethodByName("Set" + f.name)
	if m.Kind() == reflect.Func && m.Type().NumOut() == 0 && m.Type().NumIn() == 1 && core.ParseQType(m.Type().In(0)) == f.qtype {
		argType := m.Type().In(0)
		if arg, err := util.ConvertToValue(argType, tmp); err == nil {
			m.Call([]reflect.Value{owner.Addr(), arg})
		} else {
			log.WarnF("set field[%v] failed, can't resolve [%v] as [%v]: %v", f.fullname, v, argType, err)
			return
		}
	} else {
		fieldV := owner.FieldByName(f.name)
		if result, err := util.ConvertToValue(fieldV.Type(), tmp); err == nil {
			fieldV.Set(result)
		} else {
			log.WarnF("set field[%v] failed, can't resolve [%v] as [%v]: %v", f.fullname, v, fieldV.Type(), err)
			return
		}
	}
	log.DebugF("set field[%v] done: %v", f.fullname, v)
	// update cache
	f.cache = nil
}

// Check this field's value was changed or not
func (f *field) checkChanged() bool {
	if f.cache == nil {
		return true
	}
	oldVal := *f.cache

	return f.getter() != oldVal
}
