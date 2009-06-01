TEMPLATE = lib
CONFIG += plugin qt

INCLUDEPATH += ../../src ../../pluginapi
TARGET = $$qtLibraryTarget(arora_demoextension)
DESTDIR = ../../plugins
HEADERS += demoextension.h
SOURCES += demoextension.cpp

LIBS += -L../../plugins -larora_api
