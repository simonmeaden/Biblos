TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets
CONFIG         += c++11

TARGET          = hunspellplugin
DESTDIR = $$PWD/../../../build/ebookedit/ebookedit/plugins

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

unix|win32: LIBS += -L$$DESTDIR/../../interface/ -linterface
INCLUDEPATH += $$PWD/../../interface

unix|win32: LIBS += -lhunspell-1.7

CONFIG += install_ok

#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ebook/release/ -lebook
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ebook/debug/ -lebook
#else:unix: LIBS += -L$$OUT_PWD/../../ebook/ -lebook

#INCLUDEPATH += $$PWD/../../ebook
#DEPENDPATH += $$PWD/../../ebook

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../ebook/release/libebook.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../ebook/debug/libebook.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../ebook/release/ebook.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../ebook/debug/ebook.lib
#else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../ebook/libebook.a
