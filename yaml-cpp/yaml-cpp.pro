#-------------------------------------------------
#
# Project created by QtCreator 2018-10-09T09:17:42
#
#-------------------------------------------------

QT       -= core gui

TEMPLATE = lib

DEFINES += YAMLCPP_LIBRARY

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
#CONFIG += shared_and_static # Build both static and shared libraries
#CONFIG += staticlib

TARGET = yaml-cpp
#DESTDIR = ../build/yaml-cpp
#OBJECTS_DIR = $$DESTDIR/.obj
#MOC_DIR = $$DESTDIR/.moc
#RCC_DIR = $$DESTDIR/.qrc
#UI_DIR = $$DESTDIR/.ui

SOURCES += \
    src/contrib/graphbuilder.cpp \
    src/contrib/graphbuilderadapter.cpp \
    src/binary.cpp \
    src/convert.cpp \
    src/directives.cpp \
    src/emit.cpp \
    src/emitfromevents.cpp \
    src/emitter.cpp \
    src/emitterstate.cpp \
    src/emitterutils.cpp \
    src/exceptions.cpp \
    src/exp.cpp \
    src/memory.cpp \
    src/node.cpp \
    src/node_data.cpp \
    src/nodebuilder.cpp \
    src/nodeevents.cpp \
    src/null.cpp \
    src/ostream_wrapper.cpp \
    src/parse.cpp \
    src/parser.cpp \
    src/regex_yaml.cpp \
    src/scanner.cpp \
    src/scanscalar.cpp \
    src/scantag.cpp \
    src/scantoken.cpp \
    src/simplekey.cpp \
    src/singledocparser.cpp \
    src/stream.cpp \
    src/tag.cpp

HEADERS += \
    include/anchor.h \
    include/binary.h \
    include/dll.h \
    include/emitfromevents.h \
    include/emitter.h \
    include/emitterdef.h \
    include/emittermanip.h \
    include/emitterstyle.h \
    include/eventhandler.h \
    include/exceptions.h \
    include/mark.h \
    include/noncopyable.h \
    include/null.h \
    include/ostream_wrapper.h \
    include/parser.h \
    include/stlemitter.h \
    include/traits.h \
    include/yaml.h \
    include/contrib/anchordict.h \
    include/contrib/graphbuilder.h \
    include/contrib/anchordict.h \
    include/contrib/graphbuilder.h \
    include/node/detail/bool_type.h \
    include/node/detail/impl.h \
    include/node/detail/iterator.h \
    include/node/detail/iterator_fwd.h \
    include/node/detail/memory.h \
    include/node/detail/node.h \
    include/node/detail/node_data.h \
    include/node/detail/node_iterator.h \
    include/node/detail/node_ref.h \
    include/node/convert.h \
    include/node/emit.h \
    include/node/impl.h \
    include/node/iterator.h \
    include/node/node.h \
    include/node/parse.h \
    include/node/ptr.h \
    include/node/type.h \
    include/anchor.h \
    include/binary.h \
    include/dll.h \
    include/emitfromevents.h \
    include/emitter.h \
    include/emitterdef.h \
    include/emittermanip.h \
    include/emitterstyle.h \
    include/eventhandler.h \
    include/exceptions.h \
    include/mark.h \
    include/noncopyable.h \
    include/null.h \
    include/ostream_wrapper.h \
    include/parser.h \
    include/stlemitter.h \
    include/traits.h \
    include/yaml.h \
    src/contrib/graphbuilderadapter.h \
    src/collectionstack.h \
    src/directives.h \
    src/emitterstate.h \
    src/emitterutils.h \
    src/exp.h \
    src/indentation.h \
    src/nodebuilder.h \
    src/nodeevents.h \
    src/ptr_vector.h \
    src/regex_yaml.h \
    src/regeximpl.h \
    src/scanner.h \
    src/scanscalar.h \
    src/scantag.h \
    src/setting.h \
    src/singledocparser.h \
    src/stream.h \
    src/streamcharsource.h \
    src/stringsource.h \
    src/tag.h \
    src/token.h \
    include/yaml-cpp_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
