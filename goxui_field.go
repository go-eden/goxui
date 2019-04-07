package goxui

import (
	"encoding/json"
	"github.com/sisyphsu/goxui/core"
	"github.com/sisyphsu/goxui/util"
	"reflect"
)

// 属性元数据
type field struct {
	name     string      // 属性名称
	fullname string      // 属性全名
	cache    string      // 属性值缓存, 用于判断是否变化
	ftype    core.Q_TYPE // 属性类型
}

// 查询属性值, 同时更新缓存
func (f *field) getter() (v string) {
	defer func() {
		if r := recover(); r != nil {
			logger.WarnF("get field[%v] failed, panic occured: %v", f.fullname, r)
		}
	}()
	owner := util.FindOwner(reflect.ValueOf(root), f.fullname)
	if owner.Kind() != reflect.Struct {
		logger.WarnF("get field[%v] failed, can't find owner Struct", f.fullname)
		return
	}
	m := owner.Addr().MethodByName("Get" + f.name)
	if m.Kind() == reflect.Func && m.Type().NumIn() == 0 && m.Type().NumOut() == 1 && core.ParseQType(m.Type().Out(0)) == f.ftype {
		results := m.Call([]reflect.Value{})
		v = util.ToString(results[0].Interface())
	} else {
		fieldV := owner.FieldByName(f.name)
		v = util.ToString(fieldV.Interface())
	}
	logger.DebugF("get field[%v] done: %v", f.fullname, v)
	f.cache = v
	return
}

// 设置属性值
func (f *field) setter(v string) {
	defer func() {
		if r := recover(); r != nil {
			logger.WarnF("set field[%v] with param[%v] failed, panic occured: %v", f.fullname, v, r)
		}
	}()
	owner := util.FindOwner(reflect.ValueOf(root), f.fullname)
	for owner.Kind() != reflect.Struct {
		logger.WarnF("set field[%v] failed, can't find owner Struct", f.fullname)
		return
	}
	var tmp interface{}
	if err := json.Unmarshal([]byte(v), &tmp); err != nil {
		tmp = v
	}
	m := owner.Addr().MethodByName("Set" + f.name)
	if m.Kind() == reflect.Func && m.Type().NumOut() == 0 && m.Type().NumIn() == 1 && core.ParseQType(m.Type().In(0)) == f.ftype {
		argType := m.Type().In(0)
		if arg, err := util.ConvertToValue(argType, tmp); err == nil {
			m.Call([]reflect.Value{owner.Addr(), arg})
		} else {
			logger.WarnF("set field[%v] failed, can't resolve [%v] as [%v]: %v", f.fullname, v, argType, err)
			return
		}
	} else {
		fieldV := owner.FieldByName(f.name)
		if result, err := util.ConvertToValue(fieldV.Type(), tmp); err == nil {
			fieldV.Set(result)
		} else {
			logger.WarnF("set field[%v] failed, can't resolve [%v] as [%v]: %v", f.fullname, v, fieldV.Type(), err)
		}
	}
	logger.DebugF("set field[%v] done: %v", f.fullname, v)
}

// 检查当前属性是否更新
func (f *field) checkChanged() bool {
	oldVal := f.cache
	return f.getter() != oldVal
}
