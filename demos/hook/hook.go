package main

import (
	"fmt"

	"github.com/robotn/gohook"
)

func main() {
	EvChan := hook.Start()
	defer hook.End()

	for ev := range EvChan {
		fmt.Println(ev)
	}
}
