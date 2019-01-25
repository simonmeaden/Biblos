#-------------------------------------------------
#
# Project created by QtCreator 2018-12-15T07:02:29
#
#-------------------------------------------------

TEMPLATE       = lib
CONFIG         += plugin
QT             += xml svg
CONFIG         += c++11

VERSION_MAJOR = 0
VERSION_MINOR = 1
VERSION_BUILD = 0

DEFINES += \
       "EPUB_VERSION_MAJOR=$$VERSION_MAJOR" \
       "EPUB_VERSION_MINOR=$$VERSION_MINOR" \
       "EPUB_VERSION_BUILD=$$VERSION_BUILD"

TARGET = epubplugin
DESTDIR = $$PWD/../../../build/manuscript/manuscript/plugins

SOURCES += \
    epubplugin.cpp \
    epubcontainer.cpp \
    epubdocument.cpp \
    private/epubdocument_p.cpp

HEADERS += \
    epubplugin.h \
    epubplugin_global.h \
    epubcontainer.h \
    epubdocument.h \
    private/epubdocument_p.h

DISTFILES += \
    epubplugin.json

unix|win32: LIBS += -L$$DESTDIR/../../interface/ -linterface
INCLUDEPATH += $$PWD/../../interface

# CVSSplitter library
unix|win32: LIBS += -lcsvsplitter

unix|win32: LIBS += -lqloggerlib
