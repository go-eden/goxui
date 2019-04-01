TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += goxui-web
SUBDIRS += goxui-gui

SUBDIRS += test/fulltest
SUBDIRS += test/webengine
SUBDIRS += test/fulltest

# test/fulltest.depends = goxui-web
# RESOURCES += test/*
