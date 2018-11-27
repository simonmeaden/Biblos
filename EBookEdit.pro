TEMPLATE = subdirs

SUBDIRS += \
#    quazip \
#    qlogger \
#    csvsplitter \
    ebook \
#    yaml-cpp \
#    qyaml-cpp \
#    qnuspell \
    plugins \
    ebookedit

DISTFILES += \
    README.md

CONFIG += ordered

quazip.subdir = quazip

#regex.subdir = regex

qlogger.subdir = qlogger

yaml-cpp.subdir = yaml-cpp

qyaml-cpp.subdir = qyaml-cpp
qyaml-cpp.depends = yaml-cpp

csvsplitter.subdir = csvsplitter

ebook.subdir = ebook
ebook.depends = quazip qlogger csvsplitter

qnuspell.subdir = qnuspell

plugins.subdir = plugins
plugins.depends = qnuspell

ebookedit.subdir = ebookedit
ebookedit.depends = ebook qyaml-cpp #regex
