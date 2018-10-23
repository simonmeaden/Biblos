#-------------------------------------------------
#
# Project created by QtCreator 2018-10-23T10:31:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

-TEMPLATE = app

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
#CONFIG += staticlib

CONFIG(debug, debug|release) {
    TARGET = ebookedit
    DESTDIR = ../build/ebookedit
    OBJECTS_DIR = $$DESTDIR/.objd
    MOC_DIR = $$DESTDIR/.mocd
    RCC_DIR = $$DESTDIR/.qrcd
    UI_DIR = $$DESTDIR/.uid
} else {
    TARGET = ebookedit
    DESTDIR = ../build/ebookedit
    OBJECTS_DIR = $$DESTDIR/.obj
    MOC_DIR = $$DESTDIR/.moc
    RCC_DIR = $$DESTDIR/.qrc
    UI_DIR = $$DESTDIR/.ui
}

SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
