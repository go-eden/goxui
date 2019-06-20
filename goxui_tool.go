package goxui

import (
	"path/filepath"
	"runtime"
)

// Start UI from the specified relative path
func StartRelative(rpath ...string) int {
	_, filename, _, _ := runtime.Caller(1)

	path := filepath.Dir(filename)

	return Start(filepath.Join(path, filepath.Join(rpath...)))
}
