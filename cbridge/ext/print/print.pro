QT          += widgets qml quick concurrent core-private printsupport

TEMPLATE    = lib
CONFIG      += staticlib

TARGET = goxui-print

HEADERS += \
        $$PWD/goxui_print.h

SOURCES += \
        $$PWD/goxui_print.cpp

# export static library
header_files.files = $$PWD/goxui_print.h
unix {
    header_files.path = /usr/local/include
    target.path = /usr/local/lib
    INSTALLS += target header_files
}

# Dependent on goxui
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../core/release/ -lgoxui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../core/debug/ -lgoxui
else:unix: LIBS += -L$$OUT_PWD/../../core/ -lgoxui

INCLUDEPATH += $$PWD/../../core
DEPENDPATH += $$PWD/../../core

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../core/release/libgoxui.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../core/debug/libgoxui.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../core/release/goxui.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../core/debug/goxui.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../core/libgoxui.a
