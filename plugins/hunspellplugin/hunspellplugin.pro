#! [0]
TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets

TARGET          = hunspellplugin
DESTDIR = $$PWD/../../../build/ebookedit/ebookedit/plugins
INCLUDEPATH += .. # for interface

VERSION_MAJOR = 0
VERSION_MINOR = 1
VERSION_BUILD = 0

DEFINES += \
       "HUNSPELL_VERSION_MAJOR=$$VERSION_MAJOR" \
       "HUNSPELL_VERSION_MINOR=$$VERSION_MINOR" \
       "HUNSPELL_VERSION_BUILD=$$VERSION_BUILD"

HEADERS         = \
    hunspellplugin.h

SOURCES         = \
    hunspellplugin.cpp
#! [0]

# install
#target.path = $$DESTDIR/../../ebookedit/plugins
#INSTALLS += target

CONFIG += c++14

DISTFILES += \
    hunspell.json

unix|win32: LIBS += -lhunspell-1.7


unix|win32: LIBS += -L$$OUT_PWD/../../interface/ -linterface

INCLUDEPATH += $$PWD/../../interface
