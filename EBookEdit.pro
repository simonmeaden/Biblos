TEMPLATE = subdirs

SUBDIRS += \
    interface \
    plugins \
    ebookedit

DISTFILES += \
    README.md

interface.subdir = interface

plugins.subdir = plugins
plugins.depends = interface

ebookedit.subdir = ebookedit
ebookedit.depends = interface
