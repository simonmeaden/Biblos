#ifndef EBOOKEDITOR_H
#define EBOOKEDITOR_H

#include <QTextEdit>

#include "ebookdocument.h"

class EBookEditor : public QTextEdit
{
  Q_OBJECT

public:
  EBookEditor(QWidget* parent = Q_NULLPTR);
  EBookEditor(const EBookEditor& editor);
  ~EBookEditor();

  void setDocument(EBookDocument *document);
  QVariant data();

protected:
  QVariant m_data;
  EBookDocument* m_document;
};

Q_DECLARE_METATYPE(EBookEditor)

#endif // EBOOKEDITOR_H
