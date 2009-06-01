TEMPLATE = lib
CONFIG += plugin qt

INCLUDEPATH += ../../src ../../pluginapi
TARGET = $$qtLibraryTarget(arora_demoextension)
DESTDIR = ../../plugins
HEADERS += demoextension.h
SOURCES += demoextension.cpp

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
