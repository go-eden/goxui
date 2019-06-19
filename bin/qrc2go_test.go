package main

import (
	log "github.com/go-eden/slf4go"
	"testing"
)

func TestFormatBinary(t *testing.T) {
	bs := []byte{1, 10, 100, 200}

	log.Info(formatData(bs))
}
