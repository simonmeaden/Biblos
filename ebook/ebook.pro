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

SOURCES += \
    ebookdocument.cpp \
    epub.cpp \
    epubcontainer.cpp \
    epubdocument.cpp \
    mobidocument.cpp \
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
    xmlwriter.c \
    ebookeditor.cpp

HEADERS += \
    ebookdocument.h \
    ebook_global.h \
    buffer.h \
    compression.h \
    config.h \
    debug.h \
    encryption.h \
    epub.h \
    epubcontainer.h \
    epubdocument.h \
    index.h \
    memory.h \
    meta.h \
    miniz.h \
    mobi.h \
    mobidocument.h \
    opf.h \
    parse_rawml.h \
    read.h \
    sha1.h \
    structure.h \
    util.h \
    write.h \
    xmlwriter.h \
    ebookeditor.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    qpm.json

SUBDIRS += \
    mobi.pro

# QUAZIP library
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../quazip/release/ -lquazip
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../quazip/debug/ -lquazip
else:unix: LIBS += -L$$OUT_PWD/../quazip/ -lquazip

INCLUDEPATH += $$PWD/../quazip
DEPENDPATH += $$PWD/../quazip

# QLOGGER library

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../qlogger/release/ -lqlogger
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../qlogger/debug/ -lqlogger
else:unix: LIBS += -L$$OUT_PWD/../qlogger/ -lqlogger

INCLUDEPATH += $$PWD/../qlogger
DEPENDPATH += $$PWD/../qlogger

# CVSSplitter library
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../csvsplitter/ -lcsvsplitter
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../csvsplitter/ -lcsvsplitterd
else:unix: LIBS += -L$$OUT_PWD/../csvsplitter/ -lcsvsplitter

INCLUDEPATH += $$PWD/../csvsplitter
DEPENDPATH += $$PWD/../csvsplitter
