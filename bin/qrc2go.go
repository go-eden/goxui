package main

import (
	"fmt"
	log "github.com/go-eden/slf4go"
	"io/ioutil"
	"os"
	"os/exec"
	"path/filepath"
	"strings"
)

var name = "ui"
var qrcFile = name + ".qrc"
var rccFile = name + ".rcc"
var goFile = name + ".go"

var tmpl = "package %s\n\nvar DATA = []byte%s"

// 将指定目录打包为rcc文件
func main() {
	if len(os.Args) < 2 {
		log.Info("need directory as qrc path.")
		os.Exit(1)
	}
	dir := os.Args[1]
	if _, err := os.Stat(dir); err != nil {
		log.Info("directory is invalid: ", dir)
		os.Exit(2)
	}
	if dir[len(dir)-1] != os.PathSeparator {
		dir += string(os.PathSeparator)
	}

	qrcFile = dir + qrcFile
	rccFile = dir + rccFile
	goFile = dir + goFile

	log.Info("generate qrc for: ", dir)
	generateRcc(dir)

	// clean
	_ = os.Remove(qrcFile)
	_ = os.Remove(rccFile)
}

func generateRcc(dir string) {
	var lines []string
	lines = append(lines, `<!DOCTYPE RCC>`)
	lines = append(lines, `<RCC version="1.0">`)
	lines = append(lines, `<qresource>`)
	_ = filepath.Walk(dir, func(path string, info os.FileInfo, err error) error {
		path = strings.TrimPrefix(path, dir)
		if filepath.Base(path) == "" || filepath.Base(path)[0] == '.' {
			return nil
		}
		if info.IsDir() {
			return nil
		}
		if strings.HasSuffix(path, ".qmlproject") || strings.HasSuffix(path, ".qmlproject.user") {
			return nil
		}
		lines = append(lines, fmt.Sprintf("  <file>%s</file>", path))
		return nil
	})
	lines = append(lines, `</qresource>`)
	lines = append(lines, `</RCC>`)

	log.Info("write qrc file: ", qrcFile)
	if err := ioutil.WriteFile(qrcFile, []byte(strings.Join(lines, "\n")), os.ModePerm); err != nil {
		log.Error("write qrc error:", qrcFile, err)
		return
	}

	cmd := exec.Command("rcc", "-binary", "-name", name, qrcFile, "-o", rccFile)
	log.Info("generate rcc file:", qrcFile, "->", rccFile)
	if err := cmd.Run(); err != nil {
		log.Error("exec rcc failed: ", err)
		return
	}

	// bind golang
	rccData, err := ioutil.ReadFile(rccFile)
	if err != nil {
		log.Error("read rcc failed: ", err)
		return
	}
	goSrc := fmt.Sprintf(tmpl, filepath.Base(dir), formatData(rccData))
	if err = ioutil.WriteFile(goFile, []byte(goSrc), os.ModePerm); err != nil {
		log.Error("write go source error:", goFile, err)
		return
	}

	// format
	cmd = exec.Command("gofmt", dir)
	log.Info("format go source: gofmt ", dir)
	if err := cmd.Run(); err != nil {
		log.Error("format error:", err)
	}
}

// format []byte to {0x00, 0x00...}
func formatData(bs []byte) (s string) {
	for _, b := range bs {
		if len(s) > 0 {
			s += ", "
		}
		s += fmt.Sprintf("0x%02X", uint8(b))
	}
	return "{" + s + "}"
}
