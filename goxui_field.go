package goxui

import (
	"encoding/json"
	"github.com/go-eden/goxui/core"
	"github.com/go-eden/goxui/util"
	"reflect"
)

// Goxui field's metadata
type field struct {
	name     string      // field's name, like 'IsLogin'
	fullname string      // field's fullname, like 'User.IsLogin'
	root     interface{} // field's root object, relative to 'fullname'
	cache    *string     // field's value cache, used to judge whether it changes
	qtype    core.QType  // field's QType
}

// getter used to get the field's value, and update cache.
func (f *field) getter() (v string) {
	// find owner
	owner := util.FindOwner(reflect.ValueOf(f.root), f.fullname)
	if owner.Kind() != reflect.Struct {
		log.Warnf("get field[%v] failed, can't find owner Struct", f.fullname)
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
	// update cache
	f.cache = &v
	return
}

// setter used to set the field's value
func (f *field) setter(v string) {
	// find owner
	owner := util.FindOwner(reflect.ValueOf(f.root), f.fullname)
	for owner.Kind() != reflect.Struct {
		log.Warnf("set field[%v] failed, can't find owner Struct", f.fullname)
		return
	}
	// convert input argument
	var tmp interface{}
	if err := json.Unmarshal([]byte(v), &tmp); err != nil {
		log.Warnf("can't unmarshal field[%v]'s argument: %v", f.fullname, v)
		tmp = v // fallback
	}
	// check it's real Set method.
	m := owner.Addr().MethodByName("Set" + f.name)
	if m.Kind() == reflect.Func && m.Type().NumOut() == 0 && m.Type().NumIn() == 1 && core.ParseQType(m.Type().In(0)) == f.qtype {
		argType := m.Type().In(0)
		if arg, err := util.ConvertToValue(argType, tmp); err == nil {
			m.Call([]reflect.Value{owner.Addr(), arg})
		} else {
			log.Warnf("set field[%v] failed, can't resolve [%v] as [%v]: %v", f.fullname, v, argType, err)
			return
		}
	} else {
		fieldV := owner.FieldByName(f.name)
		if result, err := util.ConvertToValue(fieldV.Type(), tmp); err == nil {
			fieldV.Set(result)
		} else {
			log.Warnf("set field[%v] failed, can't resolve [%v] as [%v]: %v", f.fullname, v, fieldV.Type(), err)
			return
		}
	}
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
