
TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets

TARGET          = googlespellplugin
DESTDIR = $$PWD/../../../build/ebookedit/ebookedit/plugins

VERSION_MAJOR = 0
VERSION_MINOR = 1
VERSION_BUILD = 0

DEFINES += \
       "GOOGLE_VERSION_MAJOR=$$VERSION_MAJOR" \
       "GOOGLE_VERSION_MINOR=$$VERSION_MINOR" \
       "GOOGLE_VERSION_BUILD=$$VERSION_BUILD"

HEADERS         = \
    googlespellplugin.h

SOURCES         = \
    googlespellplugin.cpp


CONFIG += c++14

DISTFILES += \
    google.json

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ebook/ -lebook
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ebook/ -lebookd
else:unix: LIBS += -L$$OUT_PWD/../../ebook/ -lebook

INCLUDEPATH += $$PWD/../../ebook
DEPENDPATH += $$PWD/../../ebook

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../interface/ -linterface
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../interface/ -linterfaced
else:unix: LIBS += -L$$OUT_PWD/../../interface/ -linterface

INCLUDEPATH += $$PWD/../../interface
