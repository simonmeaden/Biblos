#include "mobidocument.h"

MobiDocument::MobiDocument(QObject *parent)
    : QTextDocument(parent), m_loaded(false) {}

MobiDocument::~MobiDocument() {}

void MobiDocument::openDocument(const QString &path) {
  m_documentPath = path;
  loadDocument();
}

void MobiDocument::loadDocument() {

}
