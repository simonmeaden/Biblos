#ifndef APPROVEAUTHORDIALOG_H
#define APPROVEAUTHORDIALOG_H

#include <QtWidgets>

#include "ebookcommon.h"

class Library;
class DbManager;
class AuthorDialog : public QDialog
{
public:
  enum Type
  {
    FromTitle,
    NoNames,
    ManageAuthors,
  };
  AuthorDialog(DbManager* database, QWidget* parent = Q_NULLPTR);

  int exec(Type, QString title=QString(), QStringList name = QStringList());
  SharedAuthorList authors();

protected:
  //  QSharedPointer<Library> m_library;
  DbManager* m_database;
  //  SharedAuthor  m_current_author;
  QVBoxLayout* authorLayout;
  //  QLineEdit *m_forename_edit, *m_midname_edit, *m_surname_edit;
  QLabel* m_text_lbl;
  QListWidget* m_author_list;
  QPushButton *m_accept_btn, *m_cancel_btn, *m_reset_btn, *m_help_btn;
  bool m_modified;
  QString m_forename, m_middlename, m_surname;
  QList<QLineEdit*> m_forenames, m_midnames, m_surnames;
  QList<QFrame*> m_frames;
  QList<QPushButton*> m_buttons;
  SharedAuthorList m_current_authors;
  int m_names_index;
  QString m_result;
  QPixmapCache m_pix_cache;
  QPixmapCache::Key up_key, down_key, plus_key , minus_key;
  SharedAuthorList m_author_data;

  void initGui();
  void setPartialNames(QStringList=QStringList());
  void changeSurname(const QString &name);
  void changeForename(const QString &name);
  void changeMidnames(const QString &name);

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

  static const QString FULL_COLOR;
  static const QString PARTIAL_COLOR;
  static const QString SURNAMEMATCH_COLOR;
  static const QString NO_AUTHOR_DETAILS_FOUND;

  static const QString FROM_TITLE_TITLE;
  static const QString FROM_TITLE;
  static const QString NO_NAMES;
//  void resetBtnConditions(QPushButton *plusbtn);
  void createAuthorList(SharedAuthor authors);
};

#endif // APPROVEAUTHORDIALOG_H
