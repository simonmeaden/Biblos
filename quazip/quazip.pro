#-------------------------------------------------
#
# Project created by QtCreator 2018-10-15T11:06:52
#
#-------------------------------------------------

QT       -= gui

TEMPLATE = lib

DEFINES += QUAZIP_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++14
CONFIG += staticlib
#CONFIG += shared_and_static # Build both static and shared libraries
#CONFIG += staticlib

TARGET = quazip
#DESTDIR = ../build/quazip
#OBJECTS_DIR = $$DESTDIR/.obj
#MOC_DIR = $$DESTDIR/.moc
#RCC_DIR = $$DESTDIR/.qrc
#UI_DIR = $$DESTDIR/.ui

SOURCES += \
    quazip.cpp \
    JlCompress.cpp \
    qioapi.cpp \
    quaadler32.cpp \
    quacrc32.cpp \
    quagzipfile.cpp \
    quaziodevice.cpp \
    quazipdir.cpp \
    quazipfile.cpp \
    quazipfileinfo.cpp \
    quazipnewinfo.cpp \
    unzip.c \
    zip.c

HEADERS += \
    quazip.h \
    quazip_global.h \
    ioapi.h \
    JlCompress.h \
    minizip_crypt.h \
    quaadler32.h \
    quachecksum32.h \
    quacrc32.h \
    quagzipfile.h \
    quaziodevice.h \
    quazipdir.h \
    quazipfile.h \
    quazipfileinfo.h \
    quazipnewinfo.h \
    unzip.h \
    zip.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

SUBDIRS += \
    quazip.pro

unix|win32: LIBS += -lz
