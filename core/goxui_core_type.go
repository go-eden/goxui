package core

import "reflect"

type Q_TYPE int

const (
	Q_TYPE_UNKNOWN        = -1
	Q_TYPE_VOID    Q_TYPE = iota
	Q_TYPE_BOOL
	Q_TYPE_INT
	Q_TYPE_LONG
	Q_TYPE_DOUBLE
	Q_TYPE_OBJECT
	Q_TYPE_STRING
)

func (t Q_TYPE) String() string {
	switch t {
	case Q_TYPE_VOID:
		return "void"
	case Q_TYPE_BOOL:
		return "bool"
	case Q_TYPE_INT:
		return "int"
	case Q_TYPE_LONG:
		return "long"
	case Q_TYPE_DOUBLE:
		return "double"
	case Q_TYPE_OBJECT:
		return "object"
	case Q_TYPE_STRING:
		return "string"
	default:
		return "unknown"
	}
}

func ParseQType(t reflect.Type) Q_TYPE {
	kind := t.Kind()
	if kind >= reflect.Int && kind <= reflect.Uintptr {
		return Q_TYPE_LONG
	} else if kind >= reflect.Float32 && kind <= reflect.Float64 {
		return Q_TYPE_DOUBLE
	} else if kind == reflect.Bool {
		return Q_TYPE_BOOL
	} else if kind == reflect.Array || kind == reflect.Slice || kind == reflect.Struct {
		return Q_TYPE_OBJECT
	} else if kind == reflect.String {
		return Q_TYPE_STRING
	} else {
		return Q_TYPE_UNKNOWN
	}
}
