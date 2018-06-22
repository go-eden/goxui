//
// Created by sulin on 2017/9/22.
//
//#include <printf.h>
#include <cstring>
#include <cstdlib>
#include <QString>
#include <QTimer>
#include <QJsonDocument>
#include <QVariant>
#include "goxui.h"

int flag = 1;

int getFlag(char *name) {
    printf("getFlag..... \n");
    return flag;
}

void setFlag(char *name, int v) {
    printf("setFlag..... \n");
    flag = v;
}

int getNumber(char *name) {
    printf("getNumber..... \n");
    return 111;
}

void setNumber(char *name, int v) {
    printf("setNumber..... \n");
}

double getF(char *name) {
    printf("getF..... \n");
    return 0.222;
}

void setF(char *name, double f) {
    printf("setF..... \n");
}

char *getString(char *name) {
    auto *result = (char *) malloc(sizeof(char) * 10);
    strcpy(result, "!!!!!!!!!!");
    printf("getString..... \n");
    return result;
}

void setString(char *name, char *v) {
    printf("setString..... \n");
}

char *callback(char *name, char *data) {
    auto *result = (char *) malloc(sizeof(char) * 10);
    strcpy(result, "1234567890");
    printf("callback: %s\n", data);
    return result;
}

int main(int argc, char *argv[]) {
    ui_init(argc, argv);
//    ui_setup_dump(const_cast<char *>("/Users/sulin/"));

    ui_map_resource(const_cast<char *>("img"),
                    const_cast<char *>("/Users/sulin/workspace/go/src/shareit/asserts/images"));
    printf("start...\n");
    printf("%s \n", QJsonDocument::fromVariant(QVariant::fromValue(12233)).toJson(QJsonDocument::Compact).data());
    printf("%s \n", QJsonDocument::fromVariant(QVariant::fromValue(333.22)).toJson(QJsonDocument::Compact).data());
    printf("%s \n",
           QJsonDocument::fromVariant(QVariant::fromValue(QString("fsdfsd"))).toJson(QJsonDocument::Compact).data());
//    ui_add_field_bool(const_cast<char *>("flag"), getFlag, setFlag);
//    ui_add_field_int(const_cast<char *>("number"), getNumber, setNumber);
//    ui_add_field_double(const_cast<char *>("real"), getF, setF);
//    ui_add_field_string(const_cast<char *>("str"), getString, setString);
//    ui_add_method(const_cast<char *>("RequestApi"), callback);
//    ui_add_field_string(const_cast<char *>("agentPort"), getString, setString);
//    ui_add_field_string(const_cast<char *>("agentRoles"), getString, setString);
    auto timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout, []() {
        auto name = const_cast<char *>("consoleUpdate");
        auto body = const_cast<char *>("{\"role\":\"test\",\"host\":\"www.baidu.com\",\"agent\":\"nil\",\"time\":\"00:00:00\"}");
        ui_trigger_event(name, UI_TYPE_OBJECT, body);
    });
    timer->start(3000);

//    ui_sys_tool_set_proxy(0, nullptr);

    return ui_start(const_cast<char *>("/Users/sulin/workspace/go/src/shareit/asserts/main.qml"));
}