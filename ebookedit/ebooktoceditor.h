#ifndef EBOOKTOCEDITOR_H
#define EBOOKTOCEDITOR_H

#include <QDomDocument>
#include <QtWidgets>

#include "ebooktocwidget.h"


class EBookTocEditor : public QDialog
{
public:
  EBookTocEditor(QWidget* parent = Q_NULLPTR);

  void setDocument(QTextDocument* document);
  void setDocumentString(QString document_string);
  QString documentString();

protected:
  EBookTOCWidget* m_toc_display;
  QTableWidget* m_toc_editor;
  QString m_original_string, m_result_string;
  bool m_modified, m_loaded, m_initialised;

  void initGui();
  void tocEditorCellChanged(int row, int column);
  void tocLineEnabledClicked();
  void cancelClicked();
  void acceptClicked();
  void helpClicked();


  static const QString TOC_TITLE;
  static const QString LIST_START;
  static const QString LIST_END;
  static const QString LIST_ITEM;
  static const QString LIST_BUILD_ITEM;
  static const QString LIST_FILEPOS;
  static const QString LIST_SEPARATOR;

  //  void rebuildTocString(int row);
  void updateLine(int row, bool enabled = true);
};

#endif // EBOOKTOCEDITOR_H
