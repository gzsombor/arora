INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    adblockrule.h \
    adblocksubscription.h \
    blockednetworkreply.h \
    networkaccesspolicy.h \
    ruleeditor.h \
    ruletablemodel.h \
    subscriptioneditor.h \
    subscriptiontablemodel.h \
    subscriptiondownload.h \
    filterselector.h

SOURCES += \
    adblockrule.cpp \
    adblocksubscription.cpp \
    blockednetworkreply.cpp \
    networkaccesspolicy.cpp \
    ruleeditor.cpp \
    ruletablemodel.cpp \
    subscriptioneditor.cpp \
    subscriptiontablemodel.cpp \
    subscriptiondownload.cpp \
    filterselector.cpp

FORMS += \
    ruleeditor.ui \
    subscriptioneditor.ui
