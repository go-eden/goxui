//
// Created by sulin on 2017/9/22.
//
#include "goxui.h"

int main(int argc, char *argv[]) {
    ui_init(argc, argv);

    ui_add_import_path("/Users/sulin/workspace/go/src/shareit/desktop/browser/ui");
    return ui_start("/Users/sulin/workspace/go/src/shareit/desktop/browser/ui/StartDev.qml");
    
//    return ui_start(const_cast<char *>("/Users/sulin/Development/Qt5/Examples/Qt-5.9.4/webengine/quicknanobrowser/BrowserWindow.qml"));
}
