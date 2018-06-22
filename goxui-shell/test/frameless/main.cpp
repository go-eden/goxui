#include <QQuickView>
#include <QGuiApplication>
#include <qtwebengineglobal.h>
#include <QQmlApplicationEngine>
#include "frameless.h"
#include "filter.h"
#include "MyWindow.h"
#include "item_title.h"


int main(int argc, char *argv[]) {
    QGuiApplication a(argc, argv);
    qSetMessagePattern("%{time yyyy-MM-dd hh:mm:ss} [%{type}] : %{message}");
    QtWebEngine::initialize();
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
//    qputenv("QML_DISABLE_DISK_CACHE", "true");
    qputenv("QSG_RENDER_LOOP", "basic"); // 在frameless的窗口中, 必须设置这个环境变量, 才能避免webengine假死

//    QQuickWindow::setSceneGraphBackend(QSGRendererInterface::Software);
//    auto w = new QQuickView();
//    w->setSource(QUrl("/Users/sulin/workspace/shareit/client-shell/test/qml/main1.qml"));
//    w->setWidth(900);
//    w->setHeight(720);
//    w->show();
//    w->installEventFilter(new Filter(w));
//    init_no_title(*w);

    qmlRegisterType<MyWindow>("UILib", 1, 0, "Window");
    qmlRegisterType<TitleItem>("UILib", 1, 0, "TitleBar");
    auto engine = new QQmlApplicationEngine();
    engine->load("/Users/sulin/workspace/shareit/client-shell/test/qml/Window.qml");

    return a.exec();
}
