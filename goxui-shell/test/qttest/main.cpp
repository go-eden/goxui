#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[]) {
    QGuiApplication a(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl("/Users/sulin/workspace/shareit/client-shell/test/qml/Window.qml"));

    return a.exec();
}
