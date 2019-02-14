TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets
CONFIG         += c++11

TARGET          = hunspellplugin
DESTDIR = $$OUT_PWD/..
#DESTDIR = $$PWD/../../../build/biblios/plugins
#OBJECTS_DIR = $$DESTDIR/../build/.objd
#MOC_DIR = $$DESTDIR/../build/.mocd
#RCC_DIR = $$DESTDIR/../build/.qrcd
#UI_DIR = $$DESTDIR/../build/.uid

VERSION_MAJOR = 0
VERSION_MINOR = 1
VERSION_BUILD = 0

DEFINES += \
       "HUNSPELL_VERSION_MAJOR=$$VERSION_MAJOR" \
       "HUNSPELL_VERSION_MINOR=$$VERSION_MINOR" \
       "HUNSPELL_VERSION_BUILD=$$VERSION_BUILD"

HEADERS         = \
    hunspellplugin.h \
    hunspellchecker.h

SOURCES         = \
    hunspellplugin.cpp \
    hunspellchecker.cpp

DISTFILES += \
    hunspell.json

# HUNSpell library
unix|win32: LIBS += -lhunspell-1.7


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../interface/release/ -linterface
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../interface/debug/ -linterface
else:unix: LIBS += -L$$OUT_PWD/../../interface/ -linterface

INCLUDEPATH += $$PWD/../../interface
DEPENDPATH += $$PWD/../../interface

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../interface/release/libinterface.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../interface/debug/libinterface.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../interface/release/interface.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../interface/debug/interface.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../interface/libinterface.a
