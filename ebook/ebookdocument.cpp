#include "ebookdocument.h"

EBookDocument::EBookDocument(QObject *parent)
    : QTextDocument(parent), m_modified(false), m_readonly(true) {
}

EBookDocument::EBookDocument(EBookDocument &doc) {}

EBookDocument::~EBookDocument() {}

bool EBookDocument::isModified() { return m_modified; }

void EBookDocument::setModified(bool modified) { m_modified = modified; }

QString EBookDocument::documentPath() const { return m_documentPath; }

bool EBookDocument::readonly() const { return m_readonly; }

void EBookDocument::setReadonly(bool readonly) { m_readonly = readonly; }

EBookData EBookDocument::data() { return m_data; }
