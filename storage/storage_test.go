package storage

import "testing"

func TestStorage(t *testing.T) {
	_ = Init("goxui", "HelloWorld")
	Set("test", "key", "ddddddd")
	t.Log(Get("test", "key"))
}
