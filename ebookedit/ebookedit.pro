#-------------------------------------------------
#
# Project created by QtCreator 2018-10-23T10:31:19
#
#-------------------------------------------------

QT       += core gui xml svg sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
TARGET = biblios

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

VERSION_MAJOR = 0
VERSION_MINOR = 1
VERSION_BUILD = 0

DEFINES += \
       "VERSION_MAJOR=$$VERSION_MAJOR" \
       "VERSION_MINOR=$$VERSION_MINOR" \
       "VERSION_BUILD=$$VERSION_BUILD"

DESTDIR = $$OUT_PWD/..
#DESTDIR = $$PWD/../../build/biblios
#OBJECTS_DIR = $$DESTDIR/build/.objd
#MOC_DIR = $$DESTDIR/build/.mocd
#RCC_DIR = $$DESTDIR/build/.qrcd
#UI_DIR = $$DESTDIR/build/.uid

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    optionsdialog.cpp \
    xhtmlhighlighter.cpp \
    ebookcodeeditor.cpp \
    ebookwrapper.cpp \
    ebookeditor.cpp \
    deletefiledialog.cpp \
    authordialog.cpp \
    metadataeditor.cpp \
    ebookwordreader.cpp \
    plugindialog.cpp \
    libraryframe.cpp \
    libraryshelf.cpp \
    aboutdialog.cpp \
    ebooktocwidget.cpp \
    ebooktoceditor.cpp \
    focuslineedit.cpp

HEADERS += \
    mainwindow.h \
    optionsdialog.h \
    xhtmlhighlighter.h \
    ebookcodeeditor.h \
    ebookwrapper.h \
    ebookeditor.h \
    deletefiledialog.h \
    authordialog.h \
    metadataeditor.h \
    ebookwordreader.h \
    plugindialog.h \
    libraryframe.h \
    libraryshelf.h \
    aboutdialog.h \
    ebooktocwidget.h \
    ebooktoceditor.h \
    focuslineedit.h

FORMS += \
        mainwindow.ui

RESOURCES += \
    icons.qrc

# CVSSplitter library
unix|win32: LIBS += -lcsvsplitter
# QYAML-CPP library
unix|win32: LIBS += -lqyaml-cpp
# YAML-CPP library
unix|win32: LIBS += -lyaml-cpp
# QUAZIP
unix|win32: LIBS += -lquazip5
# QLOGGER library
unix|win32: LIBS += -lqloggerlib

DISTFILES += \
    attributions.txt

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../interface/release/ -linterface
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../interface/debug/ -linterface
else:unix: LIBS += -L$$OUT_PWD/../interface/ -linterface

INCLUDEPATH += $$PWD/../interface
DEPENDPATH += $$PWD/../interface

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../interface/release/libinterface.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../interface/debug/libinterface.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../interface/release/interface.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../interface/debug/interface.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../interface/libinterface.a
