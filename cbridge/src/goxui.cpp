//
// Created by sulin on 2017/9/23.
//
#ifndef QT_NO_WIDGETS
    #include <QtWidgets/QApplication>
    typedef QApplication Application;
#else
    #include <QtGui/QGuiApplication>
    typedef QGuiApplication Application;
#endif
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
#include "goxui.h"

#ifdef WEB
    #include <QtWebEngine/qtwebengineglobal.h>
#endif

PropertyNode *root = nullptr;
Application *app = nullptr;
QQmlApplicationEngine *engine = nullptr;

// 将字符串转换为指定类型的Var变量
inline void convertStrToVar(char *data, int type, QVariant &ptr) {
    QByteArray array(data);
    switch (type) {
        case UI_TYPE_VOID:
            break;
        case UI_TYPE_BOOL:
            ptr.setValue(array == "0" || array.toLower() == "true");
            break;
        case UI_TYPE_INT:
            ptr.setValue(array.toInt());
            break;
        case UI_TYPE_LONG:
            ptr.setValue(array.toLongLong());
            break;
        case UI_TYPE_DOUBLE:
            ptr.setValue(array.toDouble());
            break;
        case UI_TYPE_OBJECT:
            ptr.setValue(QJsonDocument::fromJson(array).toVariant());
            break;
        default:
            ptr.setValue(QString(array));
    }
}

// 将字符串转换为指定类型的变量, 并赋值给指定指针
inline void convertStrToPtr(char *data, int type, void *ptr) {
    QByteArray array(data);
    switch (type) {
        case UI_TYPE_BOOL:
            *reinterpret_cast< bool *>(ptr) = array == "0" || array.toLower() == "true";
            break;
        case UI_TYPE_INT:
            *reinterpret_cast< qint32 *>(ptr) = array.toInt();
            break;
        case UI_TYPE_LONG:
            *reinterpret_cast< qint64 *>(ptr) = array.toLongLong();
            break;
        case UI_TYPE_DOUBLE:
            *reinterpret_cast< double *>(ptr) = array.toDouble();
            break;
        case UI_TYPE_OBJECT:
            *reinterpret_cast< QVariant *>(ptr) = QJsonDocument::fromJson(array).toVariant();
            break;
        default:
            *reinterpret_cast< QString *>(ptr) = QString(data);
    }
}

// 将指针按照指定类型格式化为字符串
inline QByteArray convertPtrToStr(void *arg, int type) {
    QByteArray result;
    switch (type) {
        case UI_TYPE_BOOL:
            result.setNum(*reinterpret_cast< bool *>(arg));
            break;
        case UI_TYPE_INT:
            result.setNum(*reinterpret_cast< qint32 *>(arg));
            break;
        case UI_TYPE_LONG:
            result.setNum(*reinterpret_cast< qint64 *>(arg));
            break;
        case UI_TYPE_DOUBLE:
            result.setNum(*reinterpret_cast< double *>(arg));
            break;
        case UI_TYPE_OBJECT:
            result.append(QJsonDocument::fromVariant(*reinterpret_cast< QVariant *>(arg)).toJson(QJsonDocument::Compact).data());
            break;
        default:
            result.append(*reinterpret_cast< QString *>(arg));
    }
    return result;
}

// 初始化函数, 必须最先调用
API void ui_init(int argc, char **argv) {
    qSetMessagePattern("%{time yyyy-MM-dd hh:mm:ss} [%{type}] : %{message}");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);    
    
    qputenv("QSG_RENDER_LOOP", "basic"); // for Qt5.9
    QQuickWindow::setSceneGraphBackend(QSGRendererInterface::Software); // for windows vm
    
    static QString NULL_Str;
    static int argNum = argc;
    app = new Application(argNum, argv);
    root = new PropertyNode(NULL_Str, nullptr);
    app->setQuitOnLastWindowClosed(false);
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
        writer(name, tmp.data());
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
        auto param = QJsonDocument::fromVariant(args).toJson(QJsonDocument::Compact).data();
        auto str = callback(name, param);
        convertStrToVar(str, retType, ret);
        // free(str); // 主动释放此内存!!!
    };
    return root->addMethod(nameStr, argNum, call);
}

// 通知QML指定bool参数已更新
API int ui_notify_field(char *name) {
    QString nameStr(name);
    QVariant var;
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
