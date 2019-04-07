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
#include "goxui_p.h"

static PropertyNode *root = nullptr;
static SingleApplication *app = nullptr;
static QQmlApplicationEngine *engine = nullptr;
static QMap<QString, QObject*> contextProperties;

// init QT context
API void ui_init(int argc, char **argv) {
    qSetMessagePattern("%{time yyyy-MM-dd hh:mm:ss} [%{type}] : %{message}");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // qputenv("QML_DISABLE_DISK_CACHE", "1"); // disable cache
    // qputenv("QSG_RENDER_LOOP", "basic"); // for Qt5.9
    // QQuickWindow::setSceneGraphBackend(QSGRendererInterface::Software); // for windows vm

    static QString NULL_Str;
    static int argNum = argc;
    app = new SingleApplication(argNum, argv);
    if(app->isSecondary() ) {
        qDebug() << "app repeated";
        app->exit(0);
    }

    // init ui
    root = new PropertyNode(NULL_Str, nullptr);
    qmlRegisterType<WindowItem>("Goxui", 1, 0, "Window");
    qmlRegisterType<WindowTitleItem>("Goxui", 1, 0, "TitleBar");
    qmlRegisterType<EventItem>("Goxui", 1, 0, "Event");
    qmlRegisterType<HotKeyItem>("Goxui", 1, 0, "HotKey");

    engine = new QQmlApplicationEngine();
}

// Add an QObject into QML's context
API void ui_add_object(char *name, void *ptr) {
    QString nameStr(name);
    QObject *obj = static_cast<QObject *>(ptr);
    contextProperties.insert(nameStr, obj);
}

//  Add specified c field into QML
API int ui_add_field(char *name, int type, char *(*reader)(char *), void (*writer)(char *, char *)) {
    QString nameStr(name);
    Reader r = [=](void *ret) {
        qDebug() << "invoke c getter of property" << name;
        char *data = reader(name);
        qDebug() << "invoke c getter of property" << name << "done, result is:" << data;
        convertStrToPtr(data, type, ret);
        qDebug() << "convert to ptr success";
        // free(data); // memory leak???
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

// Add specified c method into QML
API int ui_add_method(char *name, int retType, int argNum, char *(*callback)(char *, char *)) {
    QString nameStr(name);
    Callback call = [=](QVariant &ret, QVariantList &args) {
        auto param = QJsonDocument::fromVariant(args).toJson(QJsonDocument::Compact);
        qDebug() << "invoke method" << name << "with param: " << param;
        auto str = callback(name, param.toBase64().data());
        qDebug() << "invoke method" << name << "finish with result: "<< str;
        convertStrToVar(str, retType, ret);
        // free(str); // memory leak???
    };
    return root->addMethod(nameStr, argNum, call);
}

// Notify QML that specified data has changed
API int ui_notify_field(char *name) {
    QString nameStr(name);
    QVariant var;
    qDebug() << "field notify: " << name;
    return root->notifyProperty(nameStr, var);
}

// Trige specified QML event by name
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

// Add RCC data to QResource as specified prefix
API void ui_add_resource(char *prefix, char *data) {
    QString rccPrefix(prefix);
    auto rccData = reinterpret_cast<uchar *>(data);
    QResource::registerResource(rccData, rccPrefix);
}

// Add file system path to QDir's resource search path
API void ui_add_resource_path(char *path) {
    QString resPath(path);
    QDir::addResourceSearchPath(resPath);
}

// Add file system path to QML import
API void ui_add_import_path(char *path) {
    QString importPath(path);
    engine->addImportPath(importPath);
}

// Map file system resource to specify QML prefix
API void ui_map_resource(char *prefix, char *path) {
    QString resPrefix(prefix);
    QString resPath(path);
    QDir::addSearchPath(resPrefix, resPath);
}

// start Application
API int ui_start(char *qml) {
    // 监听active消息
    qDebug() << (app == nullptr);
    QObject::connect(app, &SingleApplication::instanceStarted, [=]() {
        ui_trigger_event(const_cast<char*>("app_active"), UI_TYPE_VOID, nullptr);
    });
    QObject::connect(app, &QApplication::applicationStateChanged, [=](Qt::ApplicationState state){
        if (state == Qt::ApplicationActive) {
            ui_trigger_event(const_cast<char*>("app_active"), UI_TYPE_VOID, nullptr);
        }
    });

    // setup root object
    root->buildMetaData();
    engine->rootContext()->setContextObject(root);

    // setup context properties
    contextProperties.insert("System", new UISystem(engine));
    for(auto name : contextProperties.keys()) {
        engine->rootContext()->setContextProperty(name, contextProperties.value(name));
    }

    // start~
    QString rootQML(qml);
    engine->load(rootQML);

    return app->exec();
}

// TOOL: setup the http proxy of Application
API void ui_tool_set_http_proxy(char *host, int port) {
    QNetworkProxy proxy;
    proxy.setType(QNetworkProxy::HttpProxy);
    proxy.setHostName(host);
    proxy.setPort(static_cast<quint16>(port));
    QNetworkProxy::setApplicationProxy(proxy);
}

// TOOL: setup debug level's enable status
API void ui_tool_set_debug_enabled(int enable) {
    QLoggingCategory::defaultCategory()->setEnabled(QtMsgType::QtDebugMsg, enable!=0);
}
