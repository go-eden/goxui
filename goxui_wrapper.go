package goxui

/*
#include <stdlib.h>
#include <stdio.h>
#include "goxui.h"

static void* allocArgv(int argc) {
    return malloc(sizeof(char *) * argc);
}
*/
import "C"
import (
    "unsafe"
    "os"
    "reflect"
    "shareit/common/lang"
)

// 联动C接口中的ui_init, 初始化uilib并绑定root
func Init() {
    argv := os.Args
    argc := C.int(len(argv))
    cArgv := (*[0xfff]*C.char)(C.allocArgv(argc))
    for i, arg := range argv {
        cArgv[i] = C.CString(arg)
    }
    C.ui_init(argc, (**C.char)(unsafe.Pointer(cArgv)))
}

// 封装C接口中的ui_add_resource函数. 添加资源文件, UI会将此资源作为RCC加载
func AddResourceData(prefix string, data []byte) {
    cPrefix := C.CString(prefix)
    C.ui_add_resource(cPrefix, (*C.char)(unsafe.Pointer(&data[0])))
}

// 封装C接口中的ui_add_resource_path函数. 将指定目录作为资源目录, 可用性存疑?
func AddResourcePath(path string) {
    cPath := C.CString(path)
    defer C.free(unsafe.Pointer(cPath))
    C.ui_add_resource_path(cPath)
}

// 封装C接口中的ui_add_import_path函数. 利用identified modules
func AddImportPath(path string) {
    cPath := C.CString(path)
    defer C.free(unsafe.Pointer(cPath))
    C.ui_add_import_path(cPath)
}

// 封装C接口中的ui_map_resource函数. 映射资源文件的搜索规则, 可用于灵活定制资源文件分布.
func MapResource(prefix string, path string) {
    cPrefix := C.CString(prefix)
    cPath := C.CString(path)
    defer C.free(unsafe.Pointer(cPrefix))
    defer C.free(unsafe.Pointer(cPath))
    C.ui_map_resource(cPrefix, cPath)
}

// 封装C接口中的ui_tool_set_http_proxy函数. 设置UI环境所采用的网络代理
func ToolSetHttpProxy(host string, port int) {
    cHost := C.CString(host)
    defer C.free(unsafe.Pointer(cHost))
    C.ui_tool_set_http_proxy(cHost, C.int(port))
}

// 封装C接口中的ui_tool_set_debug_enabled函数. 设置是否启用debug日志
func ToolSetDebugEnabled(enable bool) {
    if enable {
        C.ui_tool_set_debug_enabled(C.int(1))
    } else {
        C.ui_tool_set_debug_enabled(C.int(0))
    }
}

// 封装C接口中的ui_start函数. Run模式启动入口, 启动成功后将阻塞直至UI退出。
func Start(root string) int {
    cRoot := C.CString(root)
    defer C.free(unsafe.Pointer(cRoot))
    cCode := C.ui_start(cRoot)
    return int(cCode)
}

// 封装C中的ui_trigger_event函数, 触发UI中某个指定名称的事件
func TriggerEvent(name string, data interface{}) {
    dtype := parseType(reflect.TypeOf(data))
    _data := lang.ToString(data)
    cName := C.CString(name)
    cType := C.int(dtype)
    cData := C.CString(_data)
    defer C.free(unsafe.Pointer(cName))
    defer C.free(unsafe.Pointer(cData))
    C.ui_trigger_event(cName, cType, cData)
}

// 刷新UI, 可用于通知属性更新
func Flush() {
    for _, f := range fields {
        if !f.checkChanged() {
            continue
        }
        notifyField(f.fullname) // 通知字段更新
    }
}
