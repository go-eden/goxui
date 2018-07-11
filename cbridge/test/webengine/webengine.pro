QT       += webengine widgets qml quick concurrent core-private 

TARGET = webengine-test
TEMPLATE = app 

SOURCES += main.cpp

#LIBS += -framework Cocoa
#LIBS += -framework Carbon

LIBS += -L$$OUT_PWD/../../goxui-web/ -lgoxui-web

INCLUDEPATH += $$PWD/../../goxui-web
DEPENDPATH += $$PWD/../../goxui-web

INCLUDEPATH += $$PWD/../../src
