package util

import (
	"encoding/json"
	"regexp"
)

var re = regexp.MustCompile(`("[A-Z][\w_]*":)`)

func ToString(v interface{}) string {
	//if v == nil {
	//    return ""
	//}
	//switch v.(type) {
	//case int8, uint8, int16, uint16, int32, uint32, int64, uint64:
	//case *int8, *uint8, *int16, *uint16, *int32, *uint32, *int64, *uint64:
	//    return fmt.Sprint(v)
	//}
	return ToJSON(v)
}

// 将i转换为json字符串，同时key中的大写首字母自动替换为小写字母
func ToJSON(i interface{}) string {
	if i == nil {
		return ""
	}
	bs, err := json.Marshal(i)
	if err != nil { // 一般不会出错，可以吞掉它
		panic(err)
	}
	//return string(re.ReplaceAllFunc(bs, lowerSecond))
	return string(bs)
}

// 将第二个大写字母转换为小写字母
func lowerSecond(bs []byte) []byte {
	if len(bs) < 2 {
		return bs
	}
	if bs[1] >= 'A' && bs[1] <= 'Z' {
		bs[1] = bs[1] + 32
	}
	return bs
}
