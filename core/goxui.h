//
// Created by sulin on 2017/9/23.
//

#ifndef GOXUI_H
#define GOXUI_H

#ifdef Q_DECL_EXPORT
    #define API Q_DECL_EXPORT
#else
    #define API
#endif

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
 * Initilize ui context, and QApplication
 * @param argc Arguments count
 * @param argv Arguments value
 */
API void ui_init(int argc, char **argv);

/**
 * setup qt's logger
 * @param logger Outter logger
 */
API void ui_set_logger(void (*logger)(int type, char* file, int line, char* msg));

/**
 * Add specified object into QML's context, MUST BE QObject
 *
 * @param name The name of QML's context property
 * @param ptr The reference
 */
API void ui_add_object(char *name, void *ptr);

/**
 * Add an named field into QML's context, with specified reader and writer callback.
 *
 * @param name Field's name
 * @param reader Field's reader callback
 * @param writer Field's writer callback
 * @return success or not
 */
API int ui_add_field(char *name, int type, char *(*reader)(char *), void (*writer)(char *, char *));

/**
 * Add an named method into QML's context, need specify return type.
 *
 * @param name Method's name
 * @param retType Method's return type
 * @param argNum Method's arguments number
 * @param callback Method's callback, result will be free by C
 * @return success or not
 */
API int ui_add_method(char *name, int retType, int argNum, char *(*callback)(char *, char *));

/**
 * Notify the named field was updated, try trigger QML's binding property.
 * @param name Field's name
 * @return success or not
 */
API int ui_notify_field(char *name);

/**
 * Trigger the named event, with specified data.
 *
 * @param name Event's name
 * @param dataType Event's data type
 * @param data Event's data
 */
API void ui_trigger_event(char *name, int dataType, char *data);

/**
 * Add the specified RCC into Qt resources.
 *
 * @param prefix Resource's prefix
 * @param data Resources's RCC data
 */
API void ui_add_resource(char *prefix, char *data);

/**
 * Add the specified path into Qt resource path.
 *
 * @param path Could be filepath
 */
API void ui_add_resource_path(char *path);

/**
 * Add the specified path into Qt's import path, could be used for identified modules.
 *
 * @param path New import path
 */
API void ui_add_import_path(char *path);

/**
 * Add new resource's mapping role.
 * <d>Unsupported： In QML, could use `${prefix}:` to locate resource directly.</d>
 *
 * @param prefix Resource search prefix, e.g: img
 * @param path Resource search path, e.g.1: /a/b/c; e.g.2: qrc:/img/
 */
API void ui_map_resource(char *prefix, char *path);

/**
 * UI's entry-point, will block until fail or exit.
 *
 * @param root Should be an QML path
 * @return code
 */
API int ui_start(char *root);

/**
 * TOOL: setup application's http proxy
 * @param host Proxy's host name
 * @param port Proxy's port
 */
API void ui_tool_set_http_proxy(char *host, int port);

#ifdef __cplusplus
}
#endif

#endif //GOXUI_H
