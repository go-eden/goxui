package util

import (
	"encoding/json"
)

func ToString(v interface{}) string {
	return ToJSON(v)
}

// Convert any object in to json string
func ToJSON(i interface{}) string {
	if i == nil {
		return ""
	}
	bs, err := json.Marshal(i)
	if err != nil { // will not error normally
		panic(err)
	}
	return string(bs)
}
