#-------------------------------------------------
#
# Project created by QtCreator 2018-10-27T09:48:12
#
#-------------------------------------------------

TARGET = ebook
TEMPLATE = lib

QT += xml svg

DEFINES += EBOOK_LIBRARY

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
DESTDIR = $$PWD/../../build/ebookedit/ebook

SOURCES += \
    epub.cpp \
#    buffer.c \
#    compression.c \
#    debug.c \
#    encryption.c \
#    index.c \
#    memory.c \
#    meta.c \
#    miniz.c \
#    opf.c \
#    parse_rawml.c \
#    read.c \
#    sha1.c \
#    structure.c \
#    util.c \
#    write.c \
#    xmlwriter.c \
    ebookcommon.cpp \
    epubcontainer.cpp \
    epubdocument.cpp \
    private/epubdocument_p.cpp \
    private/ebookdocument_p.cpp \
    ebookdocument.cpp \
    mobidocument.cpp

HEADERS += \
    ebook_global.h \
#    buffer.h \
#    compression.h \
#    config.h \
#    debug.h \
#    encryption.h \
#    epub.h \
#    index.h \
#    memory.h \
#    meta.h \
#    miniz.h \
#    mobi.h \
#    opf.h \
#    parse_rawml.h \
#    read.h \
#    sha1.h \
#    structure.h \
#    util.h \
#    write.h \
#    xmlwriter.h \
    ebookcommon.h \
    epubcontainer.h \
    epubdocument.h \
    private/epubdocument_p.h \
    private/ebookdocument_p.h \
    ebookdocument.h \
    mobidocument.h

#unix {
#    target.path = /usr/lib
#    INSTALLS += target
#}

DISTFILES += #\
#    qpm.json

SUBDIRS += #\
#    mobi.pro

# QYAML-CPP library
unix|win32: LIBS += -lqyaml-cpp
# QUAZIP
unix|win32: LIBS += -lquazip
# YAML-CPP library
unix|win32: LIBS += -lyaml-cpp
# QLOGGER library
unix|win32: LIBS += -lqloggerlib
# CVSSplitter library
unix|win32: LIBS += -lcsvsplitter

#INCLUDEPATH += $$PWD/../../common/csvsplitter
#DEPENDPATH += $$PWD/../../common/csvsplitter
