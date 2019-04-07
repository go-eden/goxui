package web

/*
#include "goxui_web.h"
*/
import "C"
import "github.com/sisyphsu/goxui/core"

func init() {
	core.AddInitCallback(doInit)
}

func doInit() {
	C.ui_init_web()
}
