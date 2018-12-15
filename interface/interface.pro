#-------------------------------------------------
#
# Project created by QtCreator 2018-12-04T14:34:02
#
#-------------------------------------------------

QT       += gui

TARGET = interface
TEMPLATE = lib

DEFINES += INTERFACE_LIBRARY

CONFIG += staticlib
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

DESTDIR = $$PWD/../../build/ebookedit/interface

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES +=  \
    ispellinterfaceclass.cpp \
    iplugininterfaceclass.cpp \
    iebookinterfaceclass.cpp \
    ebookdocument.cpp \
    private/ebookdocument_p.cpp \
    ebookcommon.cpp

HEADERS += \
    interface_global.h \
    ispellinterface.h \
    ispellinterfaceclass.h \
    iplugininterface.h \
    ipluginterfaceclass.h \
    iebookinterface.h \
    iebookinterfaceclass.h \
    ebookdocument.h \
    private/ebookdocument_p.h \
    ebookcommon.h

DISTFILES += \
    spellinterface.json \
    ebookinterface.json \
    plugininterface.json


# EBOOK library
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ebook/ -lebook
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ebook/ -lebookd
else:unix: LIBS += -L$$OUT_PWD/../ebook/ -lebook

INCLUDEPATH += $$PWD/../ebook
DEPENDPATH += $$PWD/../ebook
