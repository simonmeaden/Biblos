#ifndef METADATAEDITOR_H
#define METADATAEDITOR_H

#include <QtWidgets>

#include "ebookcommon.h"
#include "options.h"
#include "authors.h"

class ITextDocument;

class MetadataEditor : public QWidget
{
  Q_OBJECT
public:
  explicit MetadataEditor(Options* options, AuthorsDB *authors, QWidget *parent = nullptr);

  void setDocument(ITextDocument *doc);
signals:

public slots:

protected:
  Options* m_options;
  QLineEdit *m_title_edit
  , *m_filename_edit;
  QLabel *m_fileext_lbl;
  ITextDocument *m_document;
  AuthorsDB *m_author_db;

  QList<QLineEdit*> m_authors;
  QList<QPushButton*> m_buttons;
  AuthorList m_author_list;
  QGridLayout* m_main_layout;
  int m_next_author_row;
  QFrame *m_spacer;

  void addAuthors(AuthorList authorlist);
  void setTitle(QString title);
  void setFileInfo(QString filepath);
  void initGui();
  void editTitle();
  void editAuthor();
  void editFilename();
};

#endif // METADATAEDITOR_H
