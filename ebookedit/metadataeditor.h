#ifndef METADATAEDITOR_H
#define METADATAEDITOR_H

#include <QtWidgets>

#include "authors.h"
#include "ebookcommon.h"
#include "library.h"
#include "options.h"

class ITextDocument;

class MetadataEditor : public QWidget
{
  Q_OBJECT
public:
  explicit MetadataEditor(Options* options,
                          AuthorsDB* authors,
                          LibraryDB* library,
                          QWidget* parent = nullptr);

  void setDocument(ITextDocument* doc);
signals:

public slots:

protected:
  Options* m_options;
  QLineEdit *m_title_edit, *m_filename_edit, *m_series_edit;
  QDoubleSpinBox* m_series_spin;
  QLabel* m_fileext_lbl;
  ITextDocument* m_document;
  AuthorsDB* m_author_db;
  LibraryDB* m_library_db;

  QList<QLineEdit*> m_author_edits;
  QList<QPushButton*> m_author_btns;
  QList<QCheckBox*> m_author_cboxs;
  AuthorList m_author_list, m_modified_author_list;
  QFileInfo m_file_info;
  QString m_filename, m_original_filename;
  QGridLayout* m_main_layout;
  int m_next_author_row;
  QFrame* m_spacer;
  Calibre m_calibre;
  BookData m_book_data;

  void addAuthors(AuthorList authorlist);
  void setTitle(QString title);
  void setFileInfo(QString filepath);
  void initGui();
  void editTitle();
  void editAuthor();
  void filenameChanged();
  void editSurnameLast(int checked);
  void acceptChanges();
  void rejectChanges();
  void seriesChanged(const QString& text);
  void seriesIndexChanged(const QString& value);
};

#endif // METADATAEDITOR_H
