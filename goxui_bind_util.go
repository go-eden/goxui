package goxui

import (
    "reflect"
    "encoding/json"
    "errors"
    "shareit/common/lang"
    "strings"
)

// 寻找指定对象所属的指针类型, 如果数据类型不是struct则返回失败
func findStructPtrType(t reflect.Type) (reflect.Type, bool) {
    if t.Kind() == reflect.Struct {
        t = reflect.PtrTo(t)
    }
    for t.Kind() == reflect.Ptr && t.Elem().Kind() == reflect.Ptr {
        t = t.Elem()
    }
    return t, t.Kind() == reflect.Ptr && t.Elem().Kind() == reflect.Struct
}

// 解析数据类型
func parseType(t reflect.Type) ui_type {
    kind := t.Kind()
    if kind >= reflect.Int && kind <= reflect.Uintptr {
        return UI_TYPE_LONG
    } else if kind >= reflect.Float32 && kind <= reflect.Float64 {
        return UI_TYPE_DOUBLE
    } else if kind == reflect.Bool {
        return UI_TYPE_BOOL
    } else if kind == reflect.Array || kind == reflect.Slice || kind == reflect.Struct {
        return UI_TYPE_OBJECT
    } else {
        return UI_TYPE_STRING
    }
}

// 将string转换为指定类型的value
func convertToValue(t reflect.Type, val interface{}) (result reflect.Value, err error) {
    if val == nil {
        result = reflect.Zero(t)
        return
    }
    if t.Kind() == reflect.Bool {
        if b, ok := val.(bool); ok {
            result = reflect.ValueOf(b)
        } else if i, ok := val.(float64); ok {
            result = reflect.ValueOf(int(i) != 0)
        } else {
            err = errors.New("invalid bool")
        }
    } else if t.Kind() >= reflect.Int && t.Kind() <= reflect.Float64 {
        if tmp, ok := val.(float64); ok {
            switch t.Kind() {
            case reflect.Int:
                result = reflect.ValueOf(int(tmp))
            case reflect.Int8:
                result = reflect.ValueOf(int8(tmp))
            case reflect.Int16:
                result = reflect.ValueOf(int16(tmp))
            case reflect.Int32:
                result = reflect.ValueOf(int32(tmp))
            case reflect.Int64:
                result = reflect.ValueOf(int64(tmp))
            case reflect.Uint:
                result = reflect.ValueOf(uint(tmp))
            case reflect.Uint8:
                result = reflect.ValueOf(uint8(tmp))
            case reflect.Uint16:
                result = reflect.ValueOf(uint16(tmp))
            case reflect.Uint32:
                result = reflect.ValueOf(uint32(tmp))
            case reflect.Uint64:
                result = reflect.ValueOf(uint64(tmp))
            case reflect.Uintptr:
                result = reflect.ValueOf(uintptr(tmp))
            case reflect.Float32:
                result = reflect.ValueOf(float32(tmp))
            case reflect.Float64:
                result = reflect.ValueOf(float64(tmp))
            }
        } else {
            err = errors.New("invalid number")
        }
    } else if t.Kind() == reflect.Slice || t.Kind() == reflect.Array {
        if tmp, ok := val.([]interface{}); !ok {
            err = errors.New("invalid array")
        } else {
            tmpSlice := reflect.New(reflect.SliceOf(t.Elem()))
            for _, item := range tmp {
                var tmpValue reflect.Value
                tmpValue, err = convertToValue(t.Elem(), item)
                if err != nil {
                    return
                }
                tmpSlice = reflect.Append(tmpSlice, tmpValue)
            }
            if t.Kind() == reflect.Slice {
                result = reflect.AppendSlice(result, tmpSlice)
            } else {
                reflect.Copy(result, tmpSlice)
            }
        }
    } else if t.Kind() == reflect.Map {
        if tmp, ok := val.(map[string]interface{}); !ok {
            err = errors.New("invalid map")
        } else {
            result = reflect.MakeMap(t)
            keyType := t.Key()
            valType := t.Elem()
            var _key, _val reflect.Value
            for k, v := range tmp {
                if _key, err = convertToValue(keyType, k); err != nil {
                    return
                } else if _val, err = convertToValue(valType, v); err != nil {
                    return
                } else {
                    result.SetMapIndex(_key, _val)
                }
            }
        }
    } else if t.Kind() == reflect.String {
        result = reflect.ValueOf(lang.ToString(val))
    } else if t.Kind() == reflect.Struct {
        tmp := reflect.New(t)
        if err = json.Unmarshal([]byte(lang.ToJSON(val)), tmp.Interface()); err == nil {
            result = tmp.Elem()
        }
    } else if t.Kind() == reflect.Interface {
        result = reflect.ValueOf(val)
    } else {
        err = errors.New("unsupported type: " + t.Kind().String())
    }
    return
}

// 根据属性名称，寻找定位owner节点, 返回Struct
func findOwner(val reflect.Value, name string) (result reflect.Value) {
    if !val.IsValid() {
        return // 无效
    }
    for val.Kind() == reflect.Ptr {
        val = val.Elem()
    }
    if val.Kind() != reflect.Struct {
        return // 无效
    }
    if !strings.Contains(name, ".") {
        return val
    }
    offset := strings.Index(name, ".")
    leftName := name[:offset]
    rightName := name[offset+1:]
    subVal := val.FieldByName(leftName)
    return findOwner(subVal, rightName)
}
