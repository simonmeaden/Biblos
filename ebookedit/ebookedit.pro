#-------------------------------------------------
#
# Project created by QtCreator 2018-10-23T10:31:19
#
#-------------------------------------------------

QT       += core gui xml svg sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
TARGET = ebookedit

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
#CONFIG += debug_and_release # prepared to build in debug and release forms
#CONFIG -= debug_and_release_target # but NOT in debug and release directories
#CONFIG += build_all # build BOTH debug and realese (if debug_and_release is set)
#CONFIG += shared_and_static # Build both static and shared libraries


SOURCES += \
    main.cpp \
    mainwindow.cpp \
    optionsdialog.cpp \
    xhtmlhighlighter.cpp \
    ebookcodeeditor.cpp \
    ebookwrapper.cpp \
    ebookeditor.cpp \
    deletefiledialog.cpp \
    library.cpp \
    authordialog.cpp \
    metadataeditor.cpp \
    dbmanager.cpp

HEADERS += \
    mainwindow.h \
    optionsdialog.h \
    xhtmlhighlighter.h \
    ebookcodeeditor.h \
    ebookwrapper.h \
    ebookeditor.h \
    deletefiledialog.h \
    library.h \
    authordialog.h \
    metadataeditor.h \
    dbmanager.h

FORMS += \
        mainwindow.ui


# EBOOK library
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ebook/ -lebook
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ebook/ -lebookd
else:unix: LIBS += -L$$OUT_PWD/../ebook/ -lebook

INCLUDEPATH += $$PWD/../ebook
DEPENDPATH += $$PWD/../ebook
# SpellInterface
INCLUDEPATH += ../plugins


# CVSSplitter library
unix|win32: LIBS += -lcsvsplitter
# QYAML-CPP library
unix|win32: LIBS += -lqyaml-cpp
# YAML-CPP library
unix|win32: LIBS += -lyaml-cpp
# QUAZIP
unix|win32: LIBS += -lquazip
# QLOGGER library
unix|win32: LIBS += -lqloggerlib
# STRINGUTIL
unix|win32: LIBS += -lstringutil

RESOURCES += \
    icons.qrc
