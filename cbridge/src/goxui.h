//
// Created by sulin on 2017/9/23.
//

#ifndef UILIB_UILIB_H
#define UILIB_UILIB_H

#ifdef __cplusplus
extern "C" {  // only need to export C interface if used by C++ source code
#endif

#define UI_TYPE_VOID 0
#define UI_TYPE_BOOL 1
#define UI_TYPE_INT 2
#define UI_TYPE_LONG 3
#define UI_TYPE_DOUBLE 4
#define UI_TYPE_OBJECT 5
#define UI_TYPE_STRING 6

/**
 * 初始化UI容器, 及QApplication
 * @param argc 启动参数C
 * @param argv 启动参数V
 */
void ui_init(int argc, char **argv);

/**
 * 向UI中新增一个变量, 此变量支持属性绑定
 *
 * @param name 变量名
 * @param reader 读回调, 返回值由uilib负责free
 * @param writer 写回调
 * @return 是否成功
 */
int ui_add_field(char *name, int type, char *(*reader)(char *), void (*writer)(char *, char *));

/**
 * 向UI中新增函数, 需要指定函数返回值类型
 *
 * @param name 函数名
 * @param retType 函数返回值类型, 默认为string
 * @param argNum 函数的参数数量
 * @param callback 回调函数, 返回值必须为支持free的malloc字符串
 * @return 是否成功
 */
int ui_add_method(char *name, int retType, int argNum, char *(*callback)(char *, char *));

/**
 * 通知UI中指定属性已更新, 触发QML中的属性绑定
 * @param name 属性名
 * @return 是否成功
 */
int ui_notify_field(char *name);

/**
 * 激活UI中指定名称的事件, 并携带指定类型的数据
 *
 * @param name 事件名称
 * @param dataType 数据类型
 * @param data 数据体
 */
void ui_trigger_event(char *name, int dataType, char *data);

/**
 * 添加资源文件, UI会将此资源作为RCC加载.
 *
 * @param prefix 资源前缀
 * @param data 必须为RCC格式
 */
void ui_add_resource(char *prefix, char *data);

/**
 * 添加资源文件搜索路径
 * @param path 搜索路径
 */
void ui_add_resource_path(char *path);

/**
 * 增加Import路径, 可用于identified modules.
 * @param path import路径
 */
void ui_add_import_path(char *path);

/**
 * 添加资源文件映射规则, 可用于灵活定制资源文件分布.
 * <d>暂不支持：在QML中即可直接以`${prefix}:`命名path中的资源文件.</d>
 *
 * @param prefix 搜索前缀, e.g: img
 * @param path 搜索路径, e.g.1: /a/b/c; e.g.2: qrc:/img/
 */
void ui_map_resource(char *prefix, char *path);

/**
 * Run模式: UI启动入口, 启动成功后将阻塞直至UI退出。
 *
 * @param root UI入口
 * @return 退出码
 */
int ui_start(char *root);

/**
 * 工具接口：设置HTTP代理
 * @param host 代理主机
 * @param port 代理端口
 */
void ui_tool_set_http_proxy(char *host, int port);

#ifdef __cplusplus
}
#endif

#endif //UILIB_UILIB_H
