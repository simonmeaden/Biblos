#include "iebookinterfaceclass.h"

#include "ebookcommon.h"

IEBookInterfaceClass::IEBookInterfaceClass(Options *options, QObject *parent)
    : QObject(parent), m_options(options) {}

EBookDocument *IEBookInterfaceClass::createDocument(QString filename) {}

void IEBookInterfaceClass::saveDocument(QString filename,
                                        EBookDocument *document) {}
