#ifndef EBOOKEDITOR_H
#define EBOOKEDITOR_H

#include <QTextDocument>
#include <QTextEdit>

#include "iebookdocument.h"

class EBookEditor : public QTextEdit
{
  Q_OBJECT

public:
  EBookEditor(QWidget* parent = Q_NULLPTR);
  EBookEditor(const EBookEditor& editor);
  ~EBookEditor();

  void setDocument(IEBookDocument* document);
  QVariant data();

  QString buildTocFromData();

signals:
  void documentLoaded();

protected:
  QVariant m_data;
  IEBookDocument* m_document;
};

Q_DECLARE_METATYPE(EBookEditor)

#endif  // EBOOKEDITOR_H
