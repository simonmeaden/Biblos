#-------------------------------------------------
#
# Project created by QtCreator 2018-12-15T07:02:29
#
#-------------------------------------------------

TEMPLATE       = lib
CONFIG         += plugin
QT             += core gui xml svg
CONFIG += c++14

VERSION_MAJOR = 0
VERSION_MINOR = 1
VERSION_BUILD = 0

DEFINES += \
       "EPUB_VERSION_MAJOR=$$VERSION_MAJOR" \
       "EPUB_VERSION_MINOR=$$VERSION_MINOR" \
       "EPUB_VERSION_BUILD=$$VERSION_BUILD"

TARGET = epubplugin
DESTDIR = $$OUT_PWD/..
#DESTDIR = $$PWD/../../../build/biblos/plugins
#OBJECTS_DIR = $$DESTDIR/../build/.objd
#MOC_DIR = $$DESTDIR/../build/.mocd
#RCC_DIR = $$DESTDIR/../build/.qrcd
#UI_DIR = $$DESTDIR/../build/.uid

SOURCES += \
    epubplugin.cpp \
    epubcontainer.cpp \
    epubdocument.cpp \
    private/epubdocument_p.cpp \
    htmlparser.cpp

HEADERS += \
    epubplugin.h \
    epubplugin_global.h \
    epubcontainer.h \
    epubdocument.h \
    private/epubdocument_p.h \
    htmlparser.h

DISTFILES += \
    epubplugin.json

INCLUDEPATH += /usr/local/include

#
# CVSSplitter library
unix|win32: LIBS += -lcsvsplitter
# QLogger library
unix|win32: LIBS += -lqloggerlib
# QYaml YAML Extensions
unix|win32: LIBS += -lqyaml-cpp
# YAML library
unix|win32: LIBS += -lyaml-cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../interface/ -linterface
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../interface/ -linterface
else:unix: LIBS += -L$$OUT_PWD/../../interface/ -linterface

INCLUDEPATH += $$PWD/../../interface
DEPENDPATH += $$PWD/../../interface

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../interface/libinterface.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../interface/libinterface.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../interface/interface.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../interface/interfaced.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../interface/libinterface.a

#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../qyaml-cpp/ -lqyaml-cpp
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../qyaml-cpp/ -lqyaml-cppd
#else:unix: LIBS += -L$$OUT_PWD/../../qyaml-cpp/ -lqyaml-cpp

#INCLUDEPATH += $$PWD/../../qyaml-cpp
#DEPENDPATH += $$PWD/../../qyaml-cpp
