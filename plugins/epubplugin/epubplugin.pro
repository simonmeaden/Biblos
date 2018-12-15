#-------------------------------------------------
#
# Project created by QtCreator 2018-12-15T07:02:29
#
#-------------------------------------------------

TEMPLATE       = lib
CONFIG         += plugin
QT             += xml svg
CONFIG         += c++11

TARGET = epubplugin
DESTDIR = $$PWD/../../../build/ebookedit/ebookedit/plugins


VERSION_MAJOR = 0
VERSION_MINOR = 1
VERSION_BUILD = 0

DEFINES += \
       "EPUB_VERSION_MAJOR=$$VERSION_MAJOR" \
       "EPUB_VERSION_MINOR=$$VERSION_MINOR" \
       "EPUB_VERSION_BUILD=$$VERSION_BUILD"

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
    hunspell.json

unix|win32: LIBS += -L$$DESTDIR/../../interface/ -linterface
INCLUDEPATH += $$PWD/../../interface

## EBOOK library
#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ebook/ -lebook
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ebook/ -lebookd
#else:unix: LIBS += -L$$OUT_PWD/../ebook/ -lebook

#INCLUDEPATH += $$PWD/../ebook
#DEPENDPATH += $$PWD/../ebook
