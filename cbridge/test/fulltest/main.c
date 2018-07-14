//
// Created by sulin on 2017/9/22.
//
//#include <printf.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "goxui.h"

char *bflag = "true";
char *rootInt = "123";
char *rootLong = "2333";
char *rootBodyReal = "0.114";
char *rootStr = "hello";

char *getFlag(char *name) {
    char *result = (char *) malloc(strlen(bflag));
    strcpy(result, bflag);
    return result;
}

void setFlag(char *name, char *v) {
    bflag = (char *) malloc(strlen(v)); // 会有内存泄漏
    strcpy(bflag, v);
    printf("setFlag: %s, %s\n", v, bflag);
    ui_notify_field(name);
    ui_trigger_event("event_bool", UI_TYPE_BOOL, v);
}

char *getNumber(char *name) {
    char *result = (char *) malloc(strlen(rootInt));
    strcpy(result, rootInt);
    return result;
}

void setNumber(char *name, char *v) {
    rootInt = (char *) malloc(strlen(v)); // 会有内存泄漏
    strcpy(rootInt, v);
    printf("setNumber: %s, %s\n", v, rootInt);
    ui_notify_field(name);
    ui_trigger_event("event_int", UI_TYPE_INT, v);
}

char *getLong(char *name) {
    char *result = (char *) malloc(strlen(rootLong));
    strcpy(result, rootLong);
    return result;
}

void setLong(char *name, char *v) {
    rootLong = (char *) malloc(strlen(v)); // 会有内存泄漏
    strcpy(rootLong, v);
    printf("setLong: %s, %s\n", v, rootLong);
    ui_notify_field(name);
    ui_trigger_event("event_long", UI_TYPE_LONG, v);
}

char *getDouble(char *name) {
    char *result = (char *) malloc(strlen(rootBodyReal));
    strcpy(result, rootBodyReal);
    return result;
}

void setDouble(char *name, char *v) {
    rootBodyReal = (char *) malloc(strlen(v)); // 会有内存泄漏
    strcpy(rootBodyReal, v);
    printf("setDouble: %s, %s\n", v, rootBodyReal);
    ui_notify_field(name);
    ui_trigger_event("event_double", UI_TYPE_DOUBLE, v);
}

char *getString(char *name) {
    char *result = (char *) malloc(strlen(rootStr));
    strcpy(result, rootStr);
    return result;
}

void setString(char *name, char *v) {
    rootStr = (char *) malloc(sizeof(v)); // 会有内存泄漏
    strcpy(rootStr, v);
    printf("setString: %s, %s\n", v, rootStr);
    ui_notify_field(name);
    ui_trigger_event("event_string", UI_TYPE_STRING, v);
}

char *callback(char *name, char *param) {
    char *result = (char *) malloc(sizeof(char) * 10);
    strcpy(result, "1234567890");
    printf("callback: %s\n", param);
    return result;
}

char *callback2(char *name, char *param) {
    char *result = (char *) malloc(sizeof(char) * 4);
    strcpy(result, "true");
    printf("callback2: %s\n", param);
    return result;
}

char *callback3(char *name, char *param) {
    char *tmp = "{\"name\":\"test\",\"sub\":{\"age\":1.88}}";
    char *result = (char *) malloc(strlen(tmp));
    strcpy(result, tmp);
    printf("callback3: %s, %s\n", param, result);
    ui_trigger_event("event_object", UI_TYPE_OBJECT, tmp);
    return result;
}

char *callback4(char *name, char *param) {
    char *tmp = "[{\"name\":\"test\",\"sub\":{\"age\":1.88}}]";
    char *result = (char *) malloc(strlen(tmp));
    strcpy(result, tmp);
    printf("callback4: %s, %s\n", param, result);
    ui_trigger_event("event_array", UI_TYPE_OBJECT, tmp);
    return result;
}

int main(int argc, char *argv[]) {
    ui_init(argc, argv);
    ui_add_field("Flag", UI_TYPE_BOOL, getFlag, setFlag);
    ui_add_field("Root.number", UI_TYPE_INT, getNumber, setNumber);
    ui_add_field("Root.number2", UI_TYPE_LONG, getLong, setLong);
    ui_add_field("Root.body.real", UI_TYPE_DOUBLE, getDouble, setDouble);
    ui_add_field("Root.str", UI_TYPE_STRING, getString, setString);
    ui_add_method(("Test"), UI_TYPE_DOUBLE, 2, callback);
    ui_add_method(("Root.Test0"), UI_TYPE_INT, 2, callback);
    ui_add_method(("Root.Test1"), UI_TYPE_LONG, 2, callback);
    ui_add_method(("Root.Test2"), UI_TYPE_VOID, 2, callback);
    ui_add_method(("Root.Test"), UI_TYPE_BOOL, 3, callback2);
    ui_add_method(("Root.Body.Test"), UI_TYPE_OBJECT, 3, callback3);
    ui_add_method(("Root.Body.Test1"), UI_TYPE_OBJECT, 3, callback4);

//    return ui_start(("/home/sulin/workspace/client-shell/test/fulltest/qml/main.qml"));
//    return ui_start(("/Users/sulin/workspace/shareit/client-shell/test/fulltest/qml/main.qml"));
//    return ui_start("Z:\\sulin\\workspace\\go\\src\\github.com\\sisyphsu\\goxui\\cbridge\\test\\fulltest\\qml\\main.qml");
    return ui_start("/Users/sulin/workspace/go/src/github.com/sisyphsu/goxui/cbridge/test/fulltest/qml/main.qml");
}
