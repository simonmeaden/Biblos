#include "ebookeditor.h"

EBookEditor::EBookEditor(QWidget* parent)
  : QTextEdit(parent)
{}

EBookEditor::EBookEditor(const EBookEditor& editor)
  : QTextEdit(dynamic_cast<QWidget*>(editor.parent()))
{}

EBookEditor::~EBookEditor() {}

void
EBookEditor::setDocument(EBookDocument* document)
{
  QTextEdit::setDocument(document);
  m_document = document;
  m_data.setValue(*document->data());
}

QVariant
EBookEditor::data()
{
  return m_data;
}

