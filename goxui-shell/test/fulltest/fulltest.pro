TARGET = fulltest
TEMPLATE = app 

SOURCES += main.c

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../goxui-web/release/ -lgoxui-web
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../goxui-web/debug/ -lgoxui-web
else:unix: LIBS += -L$$OUT_PWD/../../goxui-web/ -lgoxui-web

INCLUDEPATH += $$PWD/../../goxui-web
DEPENDPATH += $$PWD/../../goxui-web

INCLUDEPATH += $$PWD/../../src
