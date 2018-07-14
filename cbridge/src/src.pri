QT       += widgets qml quick concurrent core-private printsupport

DEFINES += GOXUI

HEADERS += \
        $$PWD/goxui.h \
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

include(qsingle/qsingle.pri)

mac: {
    SOURCES += $$PWD/item/item_window_mac.mm
} else:win32: {
    SOURCES += $$PWD/item/item_window_win.cpp
} else:unix {

}

INCLUDEPATH += $$PWD

# QHotKey模块
HEADERS += $$PWD/qhotkey/qhotkey.h
HEADERS += $$PWD/qhotkey/qhotkey_p.h

SOURCES += $$PWD/qhotkey/qhotkey.cpp
mac: {
    SOURCES += $$PWD/qhotkey/qhotkey_mac.cpp
} else:win32: {
    SOURCES += $$PWD/qhotkey/qhotkey_win.cpp
} else:unix: {
    SOURCES += $$PWD/qhotkey/qhotkey_x11.cpp
}

# PRC
mac: {
    LIBS += -framework Carbon
    LIBS += -framework Cocoa
} else:win32: {
    LIBS += -luser32
} else:unix {
	QT += x11extras
	LIBS += -lX11
}

# 导出接口
header_files.files = $$PWD/goxui.h
unix {
    header_files.path = /usr/local/include
    target.path = /usr/local/lib
    INSTALLS += target header_files
}
