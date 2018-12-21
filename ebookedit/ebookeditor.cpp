#include "ebookeditor.h"

EBookEditor::EBookEditor(QWidget* parent) : QTextEdit(parent) {}

EBookEditor::EBookEditor(const EBookEditor& editor)
    : QTextEdit(dynamic_cast<QWidget*>(editor.parent())) {}

EBookEditor::~EBookEditor() {}

void EBookEditor::setDocument(IEBookDocument* document) {
  QTextDocument* doc = dynamic_cast<QTextDocument*>(document);
  QTextEdit::setDocument(doc);
  m_document = document;
  //  m_data.setValue(*document->data());
  emit documentLoaded();
}

QVariant EBookEditor::data() { return m_data; }
