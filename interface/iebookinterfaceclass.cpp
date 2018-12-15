#include "iebookinterfaceclass.h"

#include "ebookcommon.h"

IEBookInterfaceClass::IEBookInterfaceClass(Options *options, QObject *parent)
    : QObject(parent), m_options(options) {}

QString IEBookInterfaceClass::fileFilter() { return m_file_filter; }

EBookDocument *IEBookInterfaceClass::createDocument(QString /*filename*/) {
  return Q_NULLPTR;
}

EBookDocument *
IEBookInterfaceClass::createCodeDocument(EBookDocument * /*document*/) {
  return Q_NULLPTR;
}

void IEBookInterfaceClass::saveDocument(QString /*filename*/,
                                        EBookDocument * /* document */) {}
