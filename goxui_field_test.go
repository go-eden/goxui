package goxui

import (
	slog "github.com/go-eden/slf4go"
	"testing"
)

func Test(t *testing.T) {
	defer func() {
		if r := recover(); r != nil {
			slog.Panic("errrr", r)
		}
	}()
	var bs [1]byte
	_ = bs[:][2]
}
