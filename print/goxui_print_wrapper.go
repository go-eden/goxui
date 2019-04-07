package print

/*
#include "goxui_print.h"
*/
import "C"
import "github.com/sisyphsu/goxui/core"

func init() {
	core.AddInitCallback(doInit)
}

func doInit() {
	C.ui_init_print()
}
