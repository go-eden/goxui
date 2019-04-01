# QHotKey模块
HEADERS += $$PWD/qhotkey.h
HEADERS += $$PWD/qhotkey_p.h

SOURCES += $$PWD/qhotkey.cpp
mac: {
    SOURCES += $$PWD/qhotkey_mac.cpp
} else:win32: {
    SOURCES += $$PWD/qhotkey_win.cpp
} else:unix: {
    SOURCES += $$PWD/qhotkey_x11.cpp
}

INCLUDEPATH += $$PWD
