package storage

import "testing"

func TestStorage(t *testing.T) {
	_ = Init("ShareBit", "testtt")
	Set("test", "key", "ddddddd")
	t.Log(Get("test", "key"))
}
