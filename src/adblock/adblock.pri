INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    adblocksubscription.h \
    adblockfilter.h \
    blockednetworkreply.h \
    networkaccesspolicy.h \
    urlaccessrule.h \
    ruleeditor.h \
    ruletablemodel.h \
    subscriptioneditor.h \
    subscriptiontablemodel.h \
    subscriptiondownload.h \
    filterselector.h

SOURCES += \
    adblocksubscription.cpp \
#    adblockfilter.cpp \
    blockednetworkreply.cpp \
    networkaccesspolicy.cpp \
    urlaccessrule.cpp \
    ruleeditor.cpp \
    ruletablemodel.cpp \
    subscriptioneditor.cpp \
    subscriptiontablemodel.cpp \
    subscriptiondownload.cpp \
    filterselector.cpp

FORMS += \
    ruleeditor.ui \
    subscriptioneditor.ui
