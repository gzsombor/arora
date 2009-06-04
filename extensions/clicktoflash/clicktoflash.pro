# common part
TEMPLATE = lib
CONFIG += plugin qt

QT += webkit network

INCLUDEPATH += ../../src ../../pluginapi ../../src/utils

DESTDIR = ../../plugins

LIBS += -L../../plugins -larora_api

!win32 {
    exists($$PWD/../../.git/HEAD) {
        # Share object files for faster compiling
        RCC_DIR     = $$PWD/.rcc
        UI_DIR      = $$PWD/.ui
        MOC_DIR     = $$PWD/.moc
        OBJECTS_DIR = $$PWD/.obj
    }
}

# plugin specific
TARGET = $$qtLibraryTarget(arora_clicktoflash)

HEADERS += clicktoflash.h \
  clicktoflashplugin.h
SOURCES += clicktoflash.cpp \
  clicktoflashplugin.cpp

FORMS += clicktoflash.ui clicktoflashsettings.ui

RESOURCES += clicktoflash.qrc

