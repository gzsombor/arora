TEMPLATE = lib
CONFIG += plugin qt
# we have to include utils, cookiejar and .ui manually

INCLUDEPATH += ../../src ../../src/utils ../../src/cookiejar ../../src/.ui
TARGET = $$qtLibraryTarget(arora_demoextension)
DESTDIR = ../../plugins
HEADERS += demoextension.h
SOURCES += demoextension.cpp
