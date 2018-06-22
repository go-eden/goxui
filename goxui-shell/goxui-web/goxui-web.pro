#-------------------------------------------------
#
# Project created by QtCreator 2018-01-15T16:14:21
#
#-------------------------------------------------

QT       += webengine

TEMPLATE = lib
#CONFIG += staticlib

win32:{
  CONFIG += dll
}

DEFINES += WEB

include(../src/src.pri)
