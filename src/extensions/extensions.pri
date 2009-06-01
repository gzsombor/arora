INCLUDEPATH += $$PWD \
    $$PWD/../../pluginapi
DEPENDPATH += $$PWD
HEADERS += aroraextension.h \
    extensionmanager.h \
    aroraapi.h \
    extensiondialog.h \
    extensioninfo.h \
    windowextension.h
SOURCES += aroraextension.cpp \
    extensionmanager.cpp \
    extensiondialog.cpp \
    extensioninfo.cpp
FORMS += extensiondialog.ui
