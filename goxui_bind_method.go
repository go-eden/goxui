package goxui

import (
    "reflect"
    "encoding/json"
    "shareit/common/lang"
)

// 函数元数据
type method struct {
    name     string      // 函数名称
    fullname string      // 函数全名
    otype    ui_type     // 函数出参类型
    inum     int         // 函数入参数量
}

// 函数注入，封装（参数反序列化、结果序列化）
func (m *method) invoke(param string) (result string) {
    defer func() {
        if r := recover(); r != nil {
            logger.InfoF("invoke [%v] failed, panic occured: %v", m.fullname, r)
        }
    }()
    owner := findOwner(reflect.ValueOf(root), m.fullname)
    if owner.Kind() != reflect.Struct {
        logger.InfoF("invoke [%v] failed, can't find owner Struct", m.fullname)
        return
    }
    methodV := owner.Addr().MethodByName(m.name)
    if methodV.Kind() != reflect.Func {
        logger.InfoF("invoke [%v] failed, invalid func", m.fullname)
        return
    }
    var args []interface{}
    if err := json.Unmarshal([]byte(param), &args); err != nil {
        logger.InfoF("invoke [%v] failed, parse args[%v] failed: %v", m.fullname, param, err)
        return
    }
    if len(args) != methodV.Type().NumIn() {
        logger.InfoF("invoke [%v] failed, number of args error: %v", m.fullname, param)
        return
    }
    argValues := make([]reflect.Value, methodV.Type().NumIn())
    for i := 0; i < methodV.Type().NumIn(); i++ {
        argType := methodV.Type().In(i)
        arg := args[i]
        if argVal, err := convertToValue(argType, arg); err != nil {
            logger.InfoF("invoke [%v] failed, can't resolve argument[%v] as [%v]: %v", m.fullname, arg, argType, err)
            return
        } else {
            argValues[i] = argVal
        }
    }
    if vals := methodV.Call(argValues); len(vals) == 1 {
        result = lang.ToString(vals[0].Interface())
    }
    logger.DebugF("invoke [%v] success with args %v, result: %v", m.fullname, param, result)
    return
}
