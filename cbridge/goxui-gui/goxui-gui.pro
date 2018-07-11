TARGET = goxui-gui

TEMPLATE = lib

CONFIG += staticlib
win32: CONFIG += dll

include(../src/src.pri)
