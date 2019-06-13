package main

import "testing"

func TestFormatBinary(t *testing.T) {
	bs := []byte{1, 10, 100, 200}

	t.Log(formatData(bs))
}
