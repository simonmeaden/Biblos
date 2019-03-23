#-------------------------------------------------
#
# Project created by QtCreator 2018-12-22T21:30:46
#
#-------------------------------------------------

QT       += core gui svg xml webenginewidgets

TEMPLATE = lib
CONFIG         += plugin
QT             += xml svg
CONFIG += c++14

VERSION_MAJOR = 0
VERSION_MINOR = 1
VERSION_BUILD = 0

DEFINES += \
       "MOBI_VERSION_MAJOR=$$VERSION_MAJOR" \
       "MOBI_VERSION_MINOR=$$VERSION_MINOR" \
       "MOBI_VERSION_BUILD=$$VERSION_BUILD"

TARGET = mobiplugin
DESTDIR = $$OUT_PWD/..
#DESTDIR = $$PWD/../../../build/biblos/plugins
#OBJECTS_DIR = $$DESTDIR/../build/.objd
#MOC_DIR = $$DESTDIR/../build/.mocd
#RCC_DIR = $$DESTDIR/../build/.qrcd
#UI_DIR = $$DESTDIR/../build/.uid

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

INCLUDEPATH += /usr/local/include

# CVSSplitter library
unix|win32: LIBS += -lcsvsplitter
# LibMobi library
unix|win32: LIBS += -L/usr/local/lib/ -lmobi
unix|win32: LIBS += -lqyaml-cpp
unix|win32: LIBS += -lyaml-cpp


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../interface/release/ -linterface
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../interface/debug/ -linterface
else:unix: LIBS += -L$$OUT_PWD/../../interface/ -linterface

INCLUDEPATH += $$PWD/../../interface
DEPENDPATH += $$PWD/../../interface

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../interface/release/libinterface.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../interface/debug/libinterface.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../interface/release/interface.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../interface/debug/interface.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../interface/libinterface.a

#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../qyaml-cpp/ -lqyaml-cpp
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../qyaml-cpp/ -lqyaml-cppd
#else:unix: LIBS += -L$$OUT_PWD/../../qyaml-cpp/ -lqyaml-cpp

#INCLUDEPATH += $$PWD/../../qyaml-cpp
#DEPENDPATH += $$PWD/../../qyaml-cpp
