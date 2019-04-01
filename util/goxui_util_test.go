package util

import "testing"

func TestToJSON(t *testing.T) {
	var num uint8 = 1
	t.Log(ToString(num))
	t.Log(ToString(&num))

	var b = false
	t.Log(ToString(b))
	t.Log(ToString(&b))

	var f float32 = 2.445453566544
	t.Log(ToString(f))
	t.Log(ToString(&f))

	var str = "helloworld"
	t.Log(ToString(str))
	t.Log(ToString(&str))

	var model = Model{"lilly", 18}
	t.Log(ToString(model))
	t.Log(ToString(&model))
}

type Model struct {
	Name string `json:"name"`
	Age  uint16 `json:"age"`
}
