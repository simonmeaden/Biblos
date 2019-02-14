#ifndef APPROVEAUTHORDIALOG_H
#define APPROVEAUTHORDIALOG_H

#include <QtWidgets>

#include "focuslineedit.h"
#include "authors.h"
#include "ebookcommon.h"


class AuthorDialog : public QDialog
{
public:
  enum Type {
    FromTitle,
    NoNames,
    ManageAuthors,
    ModifyAuthor,
  };
  AuthorDialog(AuthorsDB* database, QWidget* parent = nullptr);

  int execute(Type type, QString title = QString(),
              QStringList name = QStringList());
  int execute(AuthorData author_data);
  int execute(AuthorList author_list);
  AuthorList authors();

protected:
  //  QSharedPointer<Library> m_library;
  AuthorsDB* m_authors;
  //  SharedAuthor  m_current_author;
  QVBoxLayout* authorLayout;
  //  QLineEdit *m_forename_edit, *m_midname_edit, *m_surname_edit;
  QLabel* m_text_lbl;
  QListWidget* m_author_list;
  QPushButton *m_accept_btn, *m_cancel_btn, *m_reset_btn, *m_help_btn;
  bool m_modified;
  QString m_forename, m_middlename, m_surname;
  QList<FocusLineEdit*> m_forenames, m_middlenames, m_surnames;
  QList<bool> m_empty_rows;
  QList<QFrame*> m_frames;
  QList<QPushButton*> m_buttons;
  AuthorList m_current_authors;
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

  QFrame* createAuthorFrame();
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
