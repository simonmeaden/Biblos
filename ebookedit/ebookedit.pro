#-------------------------------------------------
#
# Project created by QtCreator 2018-10-23T10:31:19
#
#-------------------------------------------------

QT       += core gui xml svg

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

#CONFIG(debug, debug|release) {
#    TARGET = ebookedit
#    DESTDIR = ../build/ebookedit
#    OBJECTS_DIR = $$DESTDIR/.objd
#    MOC_DIR = $$DESTDIR/.mocd
#    RCC_DIR = $$DESTDIR/.qrcd
#    UI_DIR = $$DESTDIR/.uid
#} else {
#    TARGET = ebookedit
#    DESTDIR = ../build/ebookedit
#    OBJECTS_DIR = $$DESTDIR/.obj
#    MOC_DIR = $$DESTDIR/.moc
#    RCC_DIR = $$DESTDIR/.qrc
#    UI_DIR = $$DESTDIR/.ui
#}

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    hovertabwidget.cpp \
    hoverpopup.cpp \
    optionsdialog.cpp \
    xhtmlhighlighter.cpp \
    ebookcodeeditor.cpp \
    ebookwrapper.cpp \
    ebookeditor.cpp

HEADERS += \
        mainwindow.h \
    hovertabwidget.h \
    hoverpopup.h \
    optionsdialog.h \
    xhtmlhighlighter.h \
    ebookcodeeditor.h \
    ebookwrapper.h \
    ebookeditor.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# EBOOK library
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ebook/ -lebook
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ebook/ -lebookd
else:unix: LIBS += -L$$OUT_PWD/../ebook/ -lebook

INCLUDEPATH += $$PWD/../ebook
DEPENDPATH += $$PWD/../ebook

# QLOGGER library
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../qlogger/release/ -lqlogger
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../qlogger/debug/ -lqlogger
else:unix: LIBS += -L$$OUT_PWD/../qlogger/ -lqlogger

INCLUDEPATH += $$PWD/../qlogger
DEPENDPATH += $$PWD/../qlogger

# YAML-CPP library
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../yaml-cpp/ -lyaml-cpp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../yaml-cpp/ -lyaml-cppd
else:unix: LIBS += -L$$OUT_PWD/../yaml-cpp/ -lyaml-cpp

INCLUDEPATH += $$PWD/../yaml-cpp/include
DEPENDPATH += $$PWD/../yaml-cpp/include
DEPENDPATH += $$PWD/../yaml-cpp/src

# QYAML-CPP library
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../qyaml-cpp/ -lqyaml-cpp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../qyaml-cpp/ -lqyaml-cppd
else:unix: LIBS += -L$$OUT_PWD/../qyaml-cpp/ -lqyaml-cpp

INCLUDEPATH += $$PWD/../qyaml-cpp
DEPENDPATH += $$PWD/../qyaml-cpp

# CVSSplitter library
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../csvsplitter/ -lcsvsplitter
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../csvsplitter/ -lcsvsplitterd
else:unix: LIBS += -L$$OUT_PWD/../csvsplitter/ -lcsvsplitter

INCLUDEPATH += $$PWD/../csvsplitter
DEPENDPATH += $$PWD/../csvsplitter

# NUSPELL spellchecker library
unix|win32: LIBS += -lnuspell

