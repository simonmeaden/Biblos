TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets
CONFIG         += c++11

TARGET          = ocrplugin
DESTDIR = $$PWD/../../../build/ebookedit/ebookedit/plugins

VERSION_MAJOR = 0
VERSION_MINOR = 1
VERSION_BUILD = 0

DEFINES += \
       "HUNSPELL_VERSION_MAJOR=$$VERSION_MAJOR" \
       "HUNSPELL_VERSION_MINOR=$$VERSION_MINOR" \
       "HUNSPELL_VERSION_BUILD=$$VERSION_BUILD"


SOURCES += \
        ocrplugin.cpp \
    iocrinterface.cpp

HEADERS += \
        ocrplugin.h \
        ocrplugin_global.h \ 
    iocrinterface.h

DISTFILES += \
    ocrplugin.json

unix|win32: LIBS += -L$$DESTDIR/../../interface/ -linterface
INCLUDEPATH += $$PWD/../../interface

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../build/scan/release/ -lscan
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../build/scan/debug/ -lscan
else:unix: LIBS += -L$$PWD/../../../build/scan/ -lscan

INCLUDEPATH += $$PWD/../../../build/scan
DEPENDPATH += $$PWD/../../../build/scan
