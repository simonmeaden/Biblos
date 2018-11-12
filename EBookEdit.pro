TEMPLATE = subdirs

SUBDIRS += \
    quazip \
    qlogger \
    csvsplitter \
#    regex \
    ebook \
    yaml-cpp \
    qyaml-cpp \
    ebookedit \
    plugin/nuspellplugin


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

ebookedit.subdir = ebookedit
ebookedit.depends = ebook qyaml-cpp #regex
