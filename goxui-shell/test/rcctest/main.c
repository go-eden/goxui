//
// Created by sulin on 2017/9/22.
//
#include <stdio.h>
#include <stdlib.h>
#include "goxui.h"

int main(int argc, char *argv[]) {
    ui_init(argc, argv);

    FILE *f = fopen("/Users/sulin/workspace/go/src/shareit/asserts/uilib.rcc", "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  //same as rewind(f);

    char *rccData = malloc((size_t) (fsize + 1));
    fread(rccData, (size_t) fsize, 1, f);
    fclose(f);

    rccData[fsize] = 0;

    ui_add_resource("/", rccData);
    ui_map_resource("img", "/Users/sulin/workspace/go/src/shareit/asserts/static");

    return ui_start(("/Users/sulin/workspace/shareit/client-shell/test/fulltest/qml/main.qml"));
}