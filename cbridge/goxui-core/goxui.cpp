//
// Created by sulin on 2017/9/23.
//

#include <QtGlobal>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>
#include <QNetworkProxy>
#include <QFileDialog>

#include "item/item_hotkey.h"
#include "item/item_window.h"
#include "item/item_event.h"
#include "core/ui_api.h"
#include "core/ui_property.h"
#include "goxui_p.h"

// init QT context
API void ui_init(int argc, char **argv) {
    qSetMessagePattern("%{time yyyy-MM-dd hh:mm:ss} [%{type}] : %{message}");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // qputenv("QML_DISABLE_DISK_CACHE", "1"); // disable cache
    // qputenv("QSG_RENDER_LOOP", "basic"); // for Qt5.9
    // QQuickWindow::setSceneGraphBackend(QSGRendererInterface::Software); // for windows vm

    // start
    static QString NULL_Str;
    static int argNum = argc;
    app = new SingleApplication(argNum, argv);
    if(app->isSecondary() ) {
        qDebug() << "app repeated";
        app->exit(0);
    }

    // init ui
    root = new PropertyNode(NULL_Str, nullptr);
#ifdef WEB
    qDebug() << "initialize WebEngine";
    QtWebEngine::initialize();
#endif
    qmlRegisterType<WindowItem>("UILib", 1, 0, "Window");
    qmlRegisterType<WindowTitleItem>("UILib", 1, 0, "TitleBar");
    qmlRegisterType<EventItem>("UILib", 1, 0, "Event");
    qmlRegisterType<HotKeyItem>("UILib", 1, 0, "HotKey");
    engine = new QQmlApplicationEngine();
}

// 向QML暴露string属性
API int ui_add_field(char *name, int type, char *(*reader)(char *), void (*writer)(char *, char *)) {
    QString nameStr(name);
    Reader r = [=](void *ret) {
        qDebug() << "invoke c getter of property" << name;
        char *data = reader(name);
        qDebug() << "invoke c getter of property" << name << "done, result is:" << data;
        convertStrToPtr(data, type, ret);
        qDebug() << "convert to ptr success";
        // free(data); // 主动释放此内存
    };
    Writer w = [=](void *arg) {
        QByteArray tmp = convertPtrToStr(arg, type);
        writer(name, tmp.toBase64().data());
    };
    switch (type) {
        case UI_TYPE_BOOL:
            return root->addField(nameStr, QVariant::Bool, r, w);
        case UI_TYPE_INT:
            return root->addField(nameStr, QVariant::Int, r, w);
        case UI_TYPE_LONG:
            return root->addField(nameStr, QVariant::LongLong, r, w);
        case UI_TYPE_DOUBLE:
            return root->addField(nameStr, QVariant::Double, r, w);
        case UI_TYPE_OBJECT:
            return root->addField(nameStr, QMetaType::QVariant, r, w);
        default:
            return root->addField(nameStr, QVariant::String, r, w);
    }
}

// 向QML暴露指定名称的函数
API int ui_add_method(char *name, int retType, int argNum, char *(*callback)(char *, char *)) {
    QString nameStr(name);
    Callback call = [=](QVariant &ret, QVariantList &args) {
        auto param = QJsonDocument::fromVariant(args).toJson(QJsonDocument::Compact);
        qDebug() << "invoke method" << name << "with param: " << param;
        auto str = callback(name, param.toBase64().data());
        qDebug() << "invoke method" << name << "finish with result: "<< str;
        convertStrToVar(str, retType, ret);
        // free(str); // 主动释放此内存!!!
    };
    return root->addMethod(nameStr, argNum, call);
}

// 通知QML指定bool参数已更新
API int ui_notify_field(char *name) {
    QString nameStr(name);
    QVariant var;
    qDebug() << "field notify: " << name;
    return root->notifyProperty(nameStr, var);
}

// 激活QML中名称为${name}的事件
API void ui_trigger_event(char *name, int dataType, char *data) {
    QString str(name);
    QVariant var;
    convertStrToVar(data, dataType, var);
    for (auto item : EventItem::ReceverMap.values(str)) {
        if (item == nullptr) {
            continue;
        }
        item->notify(var);
    }
}

// 新增资源文件
API void ui_add_resource(char *prefix, char *data) {
    QString rccPrefix(prefix);
    auto rccData = reinterpret_cast<uchar *>(data);
    QResource::registerResource(rccData, rccPrefix);
}

// 新增资源搜索路径
API void ui_add_resource_path(char *path) {
    QString resPath(path);
    QDir::addResourceSearchPath(resPath);
}

// 新增import路径
API void ui_add_import_path(char *path) {
    QString importPath(path);
    engine->addImportPath(importPath);
}

// 新增资源路径
API void ui_map_resource(char *prefix, char *path) {
    QString resPrefix(prefix);
    QString resPath(path);
    QDir::addSearchPath(resPrefix, resPath);
}

// 启动UI: Run模式
API int ui_start(char *qml) {
    // 监听active消息
    QObject::connect(app, &SingleApplication::instanceStarted, [=]() {
        ui_trigger_event(const_cast<char*>("app_active"), UI_TYPE_VOID, nullptr);
    });
    QObject::connect(app, &QApplication::applicationStateChanged, [=](Qt::ApplicationState state){
        if (state == Qt::ApplicationActive) {
            ui_trigger_event(const_cast<char*>("app_active"), UI_TYPE_VOID, nullptr);
        }
    });
    
    // 启动UI
    QString rootQML(qml);
    root->buildMetaData();
    engine->rootContext()->setContextObject(root);
    engine->rootContext()->setContextProperty("System", new UIApi(engine));
    engine->load(rootQML);
    return app->exec();
}

// 工具接口: 设置HTTP代理
API void ui_tool_set_http_proxy(char *host, int port) {
    QNetworkProxy proxy;
    proxy.setType(QNetworkProxy::HttpProxy);
    proxy.setHostName(host);
    proxy.setPort(static_cast<quint16>(port));
    QNetworkProxy::setApplicationProxy(proxy);
}

// 工具接口: 设置是否启用Debug日志
API void ui_tool_set_debug_enabled(int enable) {
    QLoggingCategory::defaultCategory()->setEnabled(QtMsgType::QtDebugMsg, enable!=0);
}
