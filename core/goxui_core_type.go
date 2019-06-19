package core

import "reflect"

type QType int

const (
	QUnknownType       = -1
	QVoidType    QType = iota
	QBoolType
	QIntType
	QLongType
	QDoubleType
	QObjectType
	QStringType
)

func (t QType) String() string {
	switch t {
	case QVoidType:
		return "void"
	case QBoolType:
		return "bool"
	case QIntType:
		return "int"
	case QLongType:
		return "long"
	case QDoubleType:
		return "double"
	case QObjectType:
		return "object"
	case QStringType:
		return "string"
	default:
		return "unknown"
	}
}

func ParseQType(t reflect.Type) QType {
	kind := t.Kind()
	if kind >= reflect.Int && kind <= reflect.Uintptr {
		return QLongType
	} else if kind >= reflect.Float32 && kind <= reflect.Float64 {
		return QDoubleType
	} else if kind == reflect.Bool {
		return QBoolType
	} else if kind == reflect.Array || kind == reflect.Slice || kind == reflect.Struct {
		return QObjectType
	} else if kind == reflect.String {
		return QStringType
	} else {
		return QUnknownType
	}
}
