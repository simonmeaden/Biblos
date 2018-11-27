#-------------------------------------------------
#
# Project created by QtCreator 2018-10-23T10:45:22
#
#-------------------------------------------------

QT       += xml svg

TEMPLATE = lib
TARGET = epub

DEFINES += LIBEPUB_LIBRARY

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
#CONFIG += debug_and_release # prepared to build in debug and release forms
#CONFIG -= debug_and_release_target # but NOT in debug and release directories
#CONFIG += build_all # build BOTH debug and realese (if debug_and_release is set)
#CONFIG += shared_and_static # Build both static and shared libraries
#CONFIG += staticlib

#CONFIG(debug, debug|release) {
#    TARGET = epubd
#    DESTDIR = ../build/epub
#    OBJECTS_DIR = $$DESTDIR/.objd
#    MOC_DIR = $$DESTDIR/.mocd
#    RCC_DIR = $$DESTDIR/.qrcd
#    UI_DIR = $$DESTDIR/.uid
#} else {
#    TARGET = epub
#    DESTDIR = ../build/epub
#    OBJECTS_DIR = $$DESTDIR/.obj
#    MOC_DIR = $$DESTDIR/.moc
#    RCC_DIR = $$DESTDIR/.qrc
#    UI_DIR = $$DESTDIR/.ui
#}

SOURCES += \
    epub.cpp \
    epubcontainer.cpp \
    epubdocument.cpp \
    widget.cpp

HEADERS += \
    epub.h \
    epub_global.h \
    epubcontainer.h \
    epubdocument.h \
    widget.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    qpm.json


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
