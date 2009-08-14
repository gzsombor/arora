TEMPLATE = lib
VERSION = 0.7
CONFIG += qt
QT += network
INCLUDEPATH += ../src \
    ../src/utils \
    ../src/network \
    ../src/network/cookiejar \
    ../src/.ui
TARGET = $$qtLibraryTarget(arora_api)
DESTDIR = ../plugins
HEADERS += pluginapi.h \
    aroraapi.h \
    aroraeditlistview.h
SOURCES += pluginapi.cpp \
    aroraeditlistview.cpp
DEFINES += PLUGINAPI_LIBRARY
!win32:exists($$PWD/../.git/HEAD) { 
    # Share object files for faster compiling
    RCC_DIR = $$PWD/.rcc
    UI_DIR = $$PWD/.ui
    MOC_DIR = $$PWD/.moc
    OBJECTS_DIR = $$PWD/.obj
}
