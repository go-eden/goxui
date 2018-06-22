# 用于测试的辅助进程

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    include_directories(/usr/include/glib-2.0)
    include_directories(/usr/lib/x86_64-linux-gnu/glib-2.0/include/)
    add_executable(systest linux.c)
    target_link_libraries(systest gio-2.0 gobject-2.0 glib-2.0)
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")

IF (WIN32)
    add_executable(systest windows.c)
    target_link_libraries(systest wininet)
ENDIF (WIN32)

IF (APPLE)
    add_executable(systest mac.c)
    target_link_libraries(systest)
ENDIF (APPLE)
