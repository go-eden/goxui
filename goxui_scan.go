package goxui

import (
	"github.com/go-eden/goxui/core"
	"github.com/go-eden/goxui/util"
	"reflect"
)

// Scan the specified type, return it owns fields and methods.
func scanMetaData(otype reflect.Type) (fields []field, methods []method, success bool) {
	if otype, success = util.FindStructPtrType(otype); !success {
		return
	}
	// scan methods
	for i := 0; i < otype.NumMethod(); i++ {
		mtype := otype.Method(i)
		item := method{}
		item.name = mtype.Name
		item.fullname = mtype.Name
		item.inum = mtype.Type.NumIn() - 1
		if mtype.Type.NumOut() == 0 {
			item.otype = core.Q_TYPE_VOID
		} else if mtype.Type.NumOut() > 1 {
			item.otype = core.Q_TYPE_OBJECT
		} else {
			item.otype = core.ParseQType(mtype.Type.Out(0))
		}
		methods = append(methods, item)
	}
	// scan fields
	for i := 0; i < otype.Elem().NumField(); i++ {
		ftype := otype.Elem().Field(i)
		if ftype.Anonymous {
			log.Infof("unsupported anonymous field with type[%v]", ftype.Type)
			continue
		}
		// not public
		if ftype.Name[0] < 'A' || ftype.Name[0] > 'Z' {
			log.Infof("ignore no-public field[%v]", ftype.Name)
			continue
		}
		// normal qtype
		qtype := core.ParseQType(ftype.Type)
		if qtype != core.Q_TYPE_UNKNOWN {
			item := field{}
			item.name = ftype.Name
			item.fullname = ftype.Name
			item.qtype = qtype
			fields = append(fields, item)
			continue
		}
		// subtype
		if ftype.Type.Kind() == reflect.Struct || ftype.Type.Kind() == reflect.Ptr {
			subFields, subMethods, success := scanMetaData(ftype.Type)
			if !success {
				log.Infof("unsupported field[%v] with ptr", ftype.Name)
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
			continue
		}
		log.Infof("unsupported field[%v] with type[%v]", ftype.Name, ftype.Type)
	}

	return
}
