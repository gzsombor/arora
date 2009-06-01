TEMPLATE = lib
VERSION = 0.7

CONFIG += qt

INCLUDEPATH += ../src ../src/utils ../src/cookiejar ../src/.ui

TARGET = $$qtLibraryTarget(arora_api)
DESTDIR = ../plugins
HEADERS += pluginapi.h aroraapi.h
SOURCES += pluginapi.cpp

DEFINES += PLUGINAPI_LIBRARY
