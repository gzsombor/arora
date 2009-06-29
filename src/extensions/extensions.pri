INCLUDEPATH += $$PWD \
    $$PWD/../../pluginapi
DEPENDPATH += $$PWD
HEADERS += aroraextension.h \
    extensionmanager.h \
    aroraapi.h \
    extensiondialog.h \
    extensioninfo.h \
    windowextension.h \
    networkextension.h \
    arorawindow.h \
    arorawindowimpl.h
SOURCES += extensionmanager.cpp \
    extensiondialog.cpp \
    extensioninfo.cpp \
    arorawindowimpl.cpp
FORMS += extensiondialog.ui
