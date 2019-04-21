package goxui

import (
	"github.com/sisyphsu/goxui/core"
	"github.com/sisyphsu/slf4go"
	"reflect"
)

var log = slf4go.GetLogger("goxui")
var fields []field
var methods []method

func init() {
	log.SetLevel(slf4go.LEVEL_WARN)
}

// 联动C接口中的ui_init, 初始化uilib并绑定root
func Init() {
	core.Init()
}

// 封装C接口中的ui_add_resource函数. 添加资源文件, UI会将此资源作为RCC加载
func AddResourceData(prefix string, data []byte) {
	core.AddResourceData(prefix, data)
}

// 封装C接口中的ui_add_resource_path函数. 将指定目录作为资源目录, 可用性存疑?
func AddResourcePath(path string) {
	core.AddResourcePath(path)
}

// 封装C接口中的ui_add_import_path函数. 利用identified modules
func AddImportPath(path string) {
	core.AddImportPath(path)
}

// 封装C接口中的ui_map_resource函数. 映射资源文件的搜索规则, 可用于灵活定制资源文件分布.
func MapResource(prefix string, path string) {
	core.MapResource(prefix, path)
}

// 封装C接口中的ui_tool_set_http_proxy函数. 设置UI环境所采用的网络代理
func SetHttpProxy(host string, port int) {
	core.ToolSetHttpProxy(host, port)
}

// 封装C接口中的ui_tool_set_debug_enabled函数. 设置是否启用debug日志
func SetDebugEnabled(enable bool) {
	core.ToolSetDebugEnabled(enable)
}

// 封装C接口中的ui_start函数. Run模式启动入口, 启动成功后将阻塞直至UI退出。
func Start(root string) int {
	return core.Start(root)
}

// 封装C中的ui_trigger_event函数, 触发UI中某个指定名称的事件
func TriggerEvent(name string, data interface{}) {
	core.TriggerEvent(name, data)
}

// 刷新UI, 可用于通知属性更新
func Flush() {
	for _, f := range fields {
		if !f.checkChanged() {
			continue
		}
		core.NotifyField(f.fullname) // 通知字段更新
	}
}

// 将指定对象绑定入UI层, 对象中的属性、函数均会以相同名称暴露在UI中
func BindObject(obj interface{}) {
	var fields []field
	var methods []method
	var success bool
	if fields, methods, success = scanMetaData(reflect.TypeOf(obj)); !success {
		log.WarnF("scan metadata of object[%v] failed.", obj)
		return
	}
	for i := range fields {
		fields[i].root = obj
		core.AddField(fields[i].fullname, fields[i].qtype, fields[i].getter, fields[i].setter)
		log.DebugF("bind field: [%v], [%v]", fields[i].fullname, fields[i].qtype)
	}
	for i := range methods {
		methods[i].root = obj
		core.AddMethod(methods[i].fullname, methods[i].otype, methods[i].inum, methods[i].invoke)
		log.DebugF("bind method: %v(%v), %v", methods[i].fullname, methods[i].inum, methods[i].otype)
	}
}
