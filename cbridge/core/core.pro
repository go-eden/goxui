QT          += widgets qml quick concurrent core-private

TEMPLATE    = lib
CONFIG      += staticlib
TARGET      = goxui
DEFINES     += GOXUI

HEADERS += \
        $$PWD/goxui.h \
        $$PWD/goxui_p.h \
        $$PWD/core/ui_property.h \
        $$PWD/core/ui_api.h \
        $$PWD/item/item_hotkey.h \
        $$PWD/item/item_window.h \
        $$PWD/item/item_event.h

SOURCES += \
        $$PWD/goxui.cpp \
        $$PWD/core/ui_property.cpp \
        $$PWD/core/ui_api.cpp \
        $$PWD/item/item_hotkey.cpp \
        $$PWD/item/item_event.cpp

mac: {
    SOURCES += $$PWD/item/item_window_mac.mm
} else:win32: {
    SOURCES += $$PWD/item/item_window_win.cpp
} else:unix {

}

INCLUDEPATH += $$PWD

# dependency qhotkey
include(../lib/qhotkey/qhotkey.pri)

# dependency singleapplication
include(../lib/singleapplication/singleapplication.pri)
DEFINES += QAPPLICATION_CLASS=QApplication

# dependency os library
mac: {
    LIBS += -framework Carbon
    LIBS += -framework Cocoa
} else:win32: {
    LIBS += -luser32
} else:unix {
    QT += x11extras
    LIBS += -lX11
}

# export shared library
header_files.files = $$PWD/goxui.h
unix {
    header_files.path = /usr/local/include
    target.path = /usr/local/lib
    INSTALLS += target header_files
}
