#! [0]
TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets
CONFIG += c++14

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
    hunspellplugin.h \
    hunspellchecker.h

SOURCES         = \
    hunspellplugin.cpp \
    hunspellchecker.cpp
#! [0]

# install
#target.path = $$DESTDIR/../../ebookedit/plugins
#INSTALLS += target

DISTFILES += \
    hunspell.json

unix|win32: LIBS += -L$$DESTDIR/../../interface/ -linterface
INCLUDEPATH += $$PWD/../../interface

unix|win32: LIBS += -lhunspell-1.7
