package goxui

import (
    "github.com/sisyphsu/slf4go"
    "reflect"
)

var logger = slf4go.GetLogger("")
var root interface{}
var fields []field
var methods []method

func init() {
    logger.SetLevel(slf4go.LEVEL_WARN)
}

// 将制定对象绑定入UI层, 对象中的属性、函数均会以相同名称暴露在UI中
func BindObject(obj interface{}) {
    var success bool
    if fields, methods, success = scanMetaData(reflect.TypeOf(obj)); !success {
        logger.WarnF("scan metadata of object[%v] failed.", obj)
        return
    }
    for _, f := range fields {
        tmp := f
        addField(tmp.fullname, tmp.ftype, tmp.getter, tmp.setter)
        logger.InfoF("bind field: [%v], [%v]", tmp.fullname, tmp.ftype)
    }
    for _, m := range methods {
        tmp := m
        addMethod(tmp.fullname, tmp.otype, tmp.inum, tmp.invoke)
        logger.InfoF("bind method: %v(%v), %v", tmp.fullname, tmp.inum, tmp.otype)
    }
    root = obj
}

// 扫描指定对象, 返回属性和函数
func scanMetaData(otype reflect.Type) (fields []field, methods []method, success bool) {
    if otype, success = findStructPtrType(otype); !success {
        return
    }
    for i := 0; i < otype.NumMethod(); i++ {
        mtype := otype.Method(i)
        item := method{}
        item.name = mtype.Name
        item.fullname = mtype.Name
        item.inum = mtype.Type.NumIn() - 1
        if mtype.Type.NumOut() == 0 {
            item.otype = UI_TYPE_VOID
        } else if mtype.Type.NumOut() > 1 {
            item.otype = UI_TYPE_OBJECT
        } else {
            item.otype = parseType(mtype.Type.Out(0))
        }
        methods = append(methods, item)
    }
    for i := 0; i < otype.Elem().NumField(); i++ {
        ftype := otype.Elem().Field(i)
        if ftype.Anonymous {
            logger.InfoF("unsupported anonymous field with type[%v]", ftype.Type)
            continue
        }
        if ftype.Name[0] < 'A' || ftype.Name[0] > 'Z' {
            logger.InfoF("ignore private field[%v]", ftype.Name)
            continue
        }
        switch ftype.Type.Kind() {
        case reflect.Invalid, reflect.Complex64, reflect.Complex128, reflect.Chan, reflect.Func, reflect.Interface, reflect.UnsafePointer:
            logger.InfoF("unsupported field[%v] with type[%v]", ftype.Name, ftype.Type)
        case reflect.Struct, reflect.Ptr:
            subFields, subMethods, success := scanMetaData(ftype.Type)
            if !success {
                logger.InfoF("unsupported field[%v] with ptr", ftype.Name)
                continue
            }
            for _, subfield := range subFields {
                subfield.fullname = ftype.Name + "." + subfield.fullname
                fields = append(fields, subfield)
            }
            for _, submethod := range subMethods {
                submethod.fullname = ftype.Name + "." + submethod.fullname
                methods = append(methods, submethod)
            }
        default:
            item := field{}
            item.name = ftype.Name
            item.fullname = ftype.Name
            item.ftype = parseType(ftype.Type)
            fields = append(fields, item)
        }
    }
    
    return
}
