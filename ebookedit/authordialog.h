#ifndef APPROVEAUTHORDIALOG_H
#define APPROVEAUTHORDIALOG_H

#include <QtWidgets>

#include "authors.h"
#include "ebookcommon.h"
#include "focuslineedit.h"
#include "options.h"

class AuthorImage : public QLabel
{
public:
  AuthorImage(QWidget* parent = nullptr);

  bool modified() const;

protected:
  bool m_modified;
  void contextMenuEvent(QContextMenuEvent* event);
  void pasteAuthorImage();
  void loadAuthorImage();
};

class AuthorDialog : public QDialog
{
public:
  enum Type
  {
    FromTitle,
    NoNames,
    ManageAuthors,
    ModifyAuthor,
  };
  AuthorDialog(Options options, AuthorsDB database, QWidget* parent = nullptr);

  int execute(Type type,
              QString title = QString(),
              QStringList name = QStringList());
  int execute(AuthorData author_data);
  int execute(AuthorList author_list);
  AuthorData author();
  AuthorList authors();

protected:
  AuthorsDB m_authors;

  QGridLayout* m_main_layout;
  int m_max_author_row;

  QLabel* m_text_lbl;
  QListWidget* m_author_list;
  AuthorImage* m_image_lbl;
  QFrame* m_btn_frame;
  QPushButton *m_accept_btn, *m_cancel_btn, *m_reset_btn, *m_help_btn;
  QList<FocusLineEdit*> m_forenames, m_middlenames, m_surnames;
  QLineEdit *m_wiki_edit, *m_web_edit;
  QList<bool> m_empty_rows;
  QList<QFrame*> m_frames;
  QList<QPushButton*> m_buttons;
  AuthorList m_current_authors;
  int m_current_index;

  bool m_modified;
  QString m_forename, m_middlename, m_surname;
  QString m_original_website, m_original_wikipedia;

  int m_names_index;
  //  QString m_result;
  QPixmapCache::Key up_key, down_key, plus_key, minus_key;
  AuthorList m_authors_list;
  AuthorData m_result;

  void initGui();
  void setPartialNames(QStringList = QStringList());
  void changeSurname(const QString& name);
  void changeForename(const QString& name);
  void changeMidnames(const QString& name);

  void addAuthorRow();
  void removeAuthorFrame();
  void addAnotherAuthor();
  void upPressed();
  void dnPressed();

  void help();
  void acceptAuthor();
  void cancelChanges();
  void resetChanges();
  void authorDoubleClicked(QListWidgetItem* item);
  void authorRowClicked(bool focussed);
  void websiteChanged(const QString& text);
  void wikipediaChanged(const QString& text);

  bool isEmptyRow(int index)
  {
    if (m_forenames.at(index)->text().isEmpty() &&
        m_middlenames.at(index)->text().isEmpty() &&
        m_surnames.at(index)->text().isEmpty()) {
      return true;
    }
    return false;
  }
  void setEmptyRow(int index)
  {
    if (m_forenames.at(index)->text().isEmpty() &&
        m_middlenames.at(index)->text().isEmpty() &&
        m_surnames.at(index)->text().isEmpty()) {
      m_empty_rows[index] = true;
    } else {
      m_empty_rows[index] = false;
    }
  }
  void checkEmptyRows()
  {
    for (int index = 0; index < m_forenames.size(); index++) {
      setEmptyRow(index);
    }
  }
  void initPositionalButtons();
  QDialogButtonBox* initMainButtonBox();

protected:
  static const QString FULL_COLOR;
  static const QString PARTIAL_COLOR;
  static const QString SURNAMEMATCH_COLOR;
  static const QString NO_AUTHOR_DETAILS_FOUND;

  static const QString FROM_TITLE_TITLE;
  static const QString FROM_TITLE;
  static const QString NO_NAMES;
  //  void resetBtnConditions(QPushButton *plusbtn);
  //  void createAuthorList(AuthorData authors);
};

#endif // APPROVEAUTHORDIALOG_H
