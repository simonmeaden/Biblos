TEMPLATE = subdirs

SUBDIRS += \
#    qyaml-cpp\
    interface \
    plugins \
    ebookedit

DISTFILES += \
    README.md

#qyaml-cpp.subdir= qyaml-cpp

interface.subdir = interface
#interface.depends = qyaml-cpp

plugins.subdir = plugins
plugins.depends = interface #qyaml-cpp

ebookedit.subdir = ebookedit
ebookedit.depends = interface #qyaml-cpp
