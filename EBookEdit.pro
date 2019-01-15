TEMPLATE = subdirs

SUBDIRS += \
    scan \
    interface \
#    ebook \
    plugins \
    ebookedit

DISTFILES += \
    README.md

CONFIG += ordered
