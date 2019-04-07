//
// Created by sulin on 2019/4/7.
//
#include <QDebug>
#include <QtWebEngine/qtwebengineglobal.h>
#include "goxui_web.h"

// exec init
API void ui_init_web() {
    qDebug() << "initialize WebEngine";
    QtWebEngine::initialize();
}
