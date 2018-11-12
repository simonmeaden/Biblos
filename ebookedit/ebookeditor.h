#ifndef EBOOKEDITOR_H
#define EBOOKEDITOR_H

#include <QTextEdit>

#include "qebookdocument.h"

class EBookEditor : public QTextEdit
{
  Q_OBJECT

public:
  EBookEditor(QWidget* parent = Q_NULLPTR);
  EBookEditor(const EBookEditor& editor);
  ~EBookEditor();

  void setDocument(QEBookDocument* document);
  QVariant data();

protected:
  QVariant m_data;
  QEBookDocument* m_document;
};

Q_DECLARE_METATYPE(EBookEditor)

#endif // EBOOKEDITOR_H
