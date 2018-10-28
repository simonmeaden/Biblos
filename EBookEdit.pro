TEMPLATE = subdirs

SUBDIRS += \
    quazip \
    qlogger \
    csvsplitter \
    ebook \
    yaml-cpp \
    qyaml-cpp \
    ebookedit


CONFIG += ordered

quazip.subdir = quazip

qlogger.subdir = qlogger

yaml-cpp.subdir = yaml-cpp

qyaml-cpp.subdir = qyaml-cpp
qyaml-cpp.depends = yaml-cpp

csvsplitter.subdir = csvsplitter

ebook.subdir = ebook
ebook.depends = quazip qlogger csvsplitter

ebookedit.subdir = ebookedit
ebookedit.depends = ebook qyaml-cpp
