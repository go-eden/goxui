TEMPLATE = subdirs

#CONFIG += ordered

SUBDIRS += \
    goxui-gui \
    goxui-web \
#    test/webengine \
    test/fulltest

test/fulltest.depends = goxui-web
#RESOURCES += test/*
