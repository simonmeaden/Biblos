QT += core gui widgets

HEADERS    = \
    qnuspell.h \
    hunspell/affentry.hxx \
    hunspell/affixmgr.hxx \
    hunspell/atypes.hxx \
    hunspell/baseaffix.hxx \
    hunspell/csutil.hxx \
    hunspell/filemgr.hxx \
    hunspell/hashmgr.hxx \
    hunspell/htypes.hxx \
    hunspell/hunspell.h \
    hunspell/hunspell.hxx \
    hunspell/hunvisapi.h \
    hunspell/hunzip.hxx \
    hunspell/langnum.hxx \
    hunspell/phonet.hxx \
    hunspell/replist.hxx \
    hunspell/suggestmgr.hxx \
    hunspell/utf_info.hxx \
    hunspell/w_char.hxx \
    nuspell/aff_data.hxx \
    nuspell/dictionary.hxx \
    nuspell/finder.hxx \
    nuspell/locale_utils.hxx \
    nuspell/string_utils.hxx \
    nuspell/structures.hxx \
    parsers/firstparser.hxx \
    parsers/htmlparser.hxx \
    parsers/latexparser.hxx \
    parsers/manparser.hxx \
    parsers/odfparser.hxx \
    parsers/textparser.hxx \
    parsers/xmlparser.hxx

SOURCES    =  \
    qnuspell.cpp \
    hunspell/affentry.cxx \
    hunspell/affixmgr.cxx \
    hunspell/csutil.cxx \
    hunspell/filemgr.cxx \
    hunspell/hashmgr.cxx \
    hunspell/hunspell.cxx \
    hunspell/hunzip.cxx \
    hunspell/phonet.cxx \
    hunspell/replist.cxx \
    hunspell/suggestmgr.cxx \
    nuspell/aff_data.cxx \
    nuspell/dictionary.cxx \
    nuspell/finder.cxx \
    nuspell/locale_utils.cxx \
    nuspell/main.cxx \
    nuspell/structures.cxx \
    nuspell/verify.cxx \
    parsers/firstparser.cxx \
    parsers/htmlparser.cxx \
    parsers/latexparser.cxx \
    parsers/manparser.cxx \
    parsers/odfparser.cxx \
    parsers/testparser.cxx \
    parsers/textparser.cxx \
    parsers/xmlparser.cxx

TEMPLATE = lib
TARGET     = qnuspell
DESTDIR = ../../build/ebookedit/qnuspell

CONFIG += c++14
CONFIG += staticlib

INCLUDEPATH += $$PWD/../plugins # for interface.h (SpellInterface)

# QLOGGER library
unix|win32: LIBS += -lqloggerlib
