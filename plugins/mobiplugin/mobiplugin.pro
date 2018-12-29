#-------------------------------------------------
#
# Project created by QtCreator 2018-12-22T21:30:46
#
#-------------------------------------------------

QT       += svg xml

TEMPLATE = lib
CONFIG         += plugin
QT             += xml svg
CONFIG         += c++11

VERSION_MAJOR = 0
VERSION_MINOR = 1
VERSION_BUILD = 0

DEFINES += \
       "MOBI_VERSION_MAJOR=$$VERSION_MAJOR" \
       "MOBI_VERSION_MINOR=$$VERSION_MINOR" \
       "MOBI_VERSION_BUILD=$$VERSION_BUILD"

TARGET = mobiplugin
DESTDIR = $$PWD/../../../build/ebookedit/ebookedit/plugins

SOURCES += \
    mobiplugin.cpp \
    mobidocument.cpp \
    private/mobidocument_p.cpp

HEADERS += \
    mobiplugin.h \
    mobiplugin_global.h \
    mobidocument.h \
    private/mobidocument_p.h

DISTFILES += \
    mobiplugin.json

unix|win32: LIBS += -L$$DESTDIR/../../interface/ -linterface
INCLUDEPATH += $$PWD/../../interface

# CVSSplitter library
unix|win32: LIBS += -lcsvsplitter
unix|win32: LIBS += -L/usr/local/lib/ -lmobi

