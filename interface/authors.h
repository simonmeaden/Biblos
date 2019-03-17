#ifndef AUTHORS_H
#define AUTHORS_H

#include <QObject>
#include <QPixmap>

#include <qyaml-cpp/QYamlCpp>

#include "ebookbasemetadata.h"
#include "options.h"

class EBookAuthorData
{
public:
  enum Comparison
  {
    NO_MATCH,
    SURNAME_MATCH,
    FORENAME_MATCH,
    FORE_AND_SURNAME_MATCH,
    ALL_MATCH,
    PARTIAL_MATCH,
  };

  EBookAuthorData();
  EBookAuthorData(const EBookAuthorData& other);
  ~EBookAuthorData();

  bool isValid();
  bool isEmpty();
  bool isModified();
  bool operator==(const EBookAuthorData& rhs)
  {
    if (m_uid == rhs.m_uid) {
      return true;
    }
    return false;
  }
  Comparison compare(QString m_forename,
                     QString m_middlenames,
                     QString m_surname);

  QString forename() const;
  void setForename(const QString& value);
  QString middlenames() const;
  void setMiddlenames(const QString& value);
  QString surname() const;
  void setSurname(const QString& surname);
  QString fileAs() const;
  void setFile_as(const QString& file_as);
  quint64 uid() const;
  void setUid(const quint64& uid);
  QString displayName() const;
  void setDisplayName(const QString& displayName);
  QString website() const;
  void setWebsite(const QString& website);
  QString wikipedia() const;
  void setWikipedia(const QString& wikipedia);
  QList<quint64> books() const;
  void setBooks(const QList<quint64>& books);
  QPixmap pixmap() const;
  void setPixmap(const QPixmap& pixmap);
  bool surnameLast() const;
  void setSurnameLast(bool surnameLast);
  QStringList wordList() const;
  void setWordList(const QStringList& wordList);

protected:
  bool m_modified;
  quint64 m_uid;
  QString m_display_name;
  QString m_forename;
  QString m_middlenames;
  QString m_surname;
  QString m_file_as;
  bool m_surname_last;
  QStringList m_word_list;

  QString m_website;
  QString m_wikipedia;
  QList<quint64> m_books;
  QPixmap m_pixmap;

public:
};

typedef QSharedPointer<EBookAuthorData> AuthorData;
typedef QList<AuthorData> AuthorList;
typedef QMap<quint64, AuthorData> AuthorMap;
typedef QMultiMap<QString, AuthorData> AuthorByString;
Q_DECLARE_METATYPE(AuthorData);

class EBookAuthorsDB
{
public:
  explicit EBookAuthorsDB(Options options);
  ~EBookAuthorsDB();

  void setFilename(QString filename);
  bool save();
  bool load(QString filename);

  bool removeBook(quint64 index);

  AuthorData author(QString name);
  AuthorData author(quint64 uid);
  AuthorList authors();
  AuthorList authorsBySurname(QString surname);
  AuthorList authorsByForename(QString surname);
  AuthorData authorByFileAs(QString file_as);
  quint64 insertAuthor(AuthorData author);
  AuthorData addAuthor(QString display_name,
                       FileAsList file_as_list = FileAsList());
  void addAuthor(AuthorData author_data);
  QStringList compareAndDiscard(QStringList names);

  static quint64 nextUid() { return ++m_highest_uid; }

signals:

public slots:

protected:
  Options m_options;
  QString m_filename;
  AuthorMap m_author_data;
  AuthorByString m_author_by_fileas;
  AuthorByString m_author_by_displayname;
  AuthorByString m_author_by_surname;
  AuthorByString m_author_by_forename;

  bool m_author_changed;

  bool loadAuthors();
  bool saveAuthors();

  static quint64 m_highest_uid;
};
typedef QSharedPointer<EBookAuthorsDB> AuthorsDB;

#endif // AUTHORS_H
