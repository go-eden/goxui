package storage

import "testing"

func TestStorage(t *testing.T) {
    Init("ShareBit", "testtt")
    Set("test", "key", "ddddddd")
    t.Log(Get("test", "key"))
}
