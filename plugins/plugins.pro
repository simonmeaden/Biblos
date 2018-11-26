#! [0]
TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets

TARGET          = $$qtLibraryTarget(nuspellplugin)
DESTDIR         = ../ebookedit/plugins

HEADERS         = \
    interface.h \
    googlespellplugin.h \
    hunspellplugin.h

SOURCES         = \
    googlespellplugin.cpp \
    interface.cpp \
    hunspellplugin.cpp
#! [0]

# install
target.path = ../build/ebookedit/ebookedit/plugins
INSTALLS += target

CONFIG += c++14

DISTFILES += \
    nuspellplugin.json

## QNUSPELL
#unix|win32: LIBS += -L$$OUT_PWD/../qnuspell/ -lqnuspell
#INCLUDEPATH += $$PWD/../qnuspell
#INCLUDEPATH += $$PWD/../ebook

unix|win32: LIBS += -lhunspell-1.7
