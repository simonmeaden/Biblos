#-------------------------------------------------
#
# Project created by QtCreator 2018-10-23T10:27:43
#
#-------------------------------------------------

QT       -= gui

TEMPLATE = lib

DEFINES += LIBMOBI_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11
CONFIG += debug_and_release # prepared to build in debug and release forms
CONFIG -= debug_and_release_target # but NOT in debug and release directories
CONFIG += build_all # build BOTH debug and realese (if debug_and_release is set)
#CONFIG += shared_and_static # Build both static and shared libraries
CONFIG += staticlib

CONFIG(debug, debug|release) {
    TARGET = libmobid
    DESTDIR = ../build/libmobi
    OBJECTS_DIR = $$DESTDIR/.objd
    MOC_DIR = $$DESTDIR/.mocd
    RCC_DIR = $$DESTDIR/.qrcd
    UI_DIR = $$DESTDIR/.uid
} else {
    TARGET = libmobi
    DESTDIR = ../build/libmobi
    OBJECTS_DIR = $$DESTDIR/.obj
    MOC_DIR = $$DESTDIR/.moc
    RCC_DIR = $$DESTDIR/.qrc
    UI_DIR = $$DESTDIR/.ui
}

SOURCES += \
    libmobi.cpp \
    buffer.c \
    compression.c \
    debug.c \
    encryption.c \
    index.c \
    memory.c \
    meta.c \
    miniz.c \
    opf.c \
    parse_rawml.c \
    read.c \
    sha1.c \
    structure.c \
    util.c \
    write.c \
    xmlwriter.c

HEADERS += \
    libmobi.h \
    libmobi_global.h \
    buffer.h \
    compression.h \
    config.h \
    debug.h \
    encryption.h \
    index.h \
    memory.h \
    meta.h \
    miniz.h \
    mobi.h \
    opf.h \
    parse_rawml.h \
    read.h \
    sha1.h \
    structure.h \
    util.h \
    write.h \
    xmlwriter.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
