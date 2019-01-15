#ifndef EBOOKTOCEDITOR_H
#define EBOOKTOCEDITOR_H

#include<QtWidgets>
#include <QDomDocument>

#include "ebooktocwidget.h"

class EBookTocEditor : public QDialog
{
public:
  EBookTocEditor(QWidget* parent = Q_NULLPTR);

  void setDocument(QTextDocument *document);
  void setDocumentString(QString document_string);

protected:
  EBookTOCWidget* m_toc_widget;
  QTableWidget *m_toc_editor;
  void initGui();
};

#endif // EBOOKTOCEDITOR_H
