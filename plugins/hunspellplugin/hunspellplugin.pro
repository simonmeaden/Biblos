TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets
CONFIG         += c++11

TARGET          = hunspellplugin
DESTDIR = $$PWD/../../../build/manuscript/manuscript/plugins

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

DISTFILES += \
    hunspell.json

unix|win32: LIBS += -L$$DESTDIR/../../interface/ -linterface
INCLUDEPATH += $$PWD/../../interface

unix|win32: LIBS += -lhunspell-1.7

CONFIG += install_ok
