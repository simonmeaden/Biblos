#ifndef AUTHORS_H
#define AUTHORS_H

#include <QObject>

#include <qyaml-cpp/QYamlCpp>

#include "ebookbasemetadata.h"

struct EBookAuthorData {
  enum Comparison {
    NO_MATCH,
    SURNAME_MATCH,
    FORENAME_MATCH,
    FORE_AND_SURNAME_MATCH,
    ALL_MATCH,
    PARTIAL_MATCH,
  };

  EBookAuthorData()
  {
    uid = 0;
  }
  EBookAuthorData(const EBookAuthorData& other)
  {
    uid = other.uid;
  }
  ~EBookAuthorData()
  {
  }

  quint64 uid;
  QString display_name;
  QString forename;
  QString middlenames;
  QString surname;
  QString file_as;

  QString website;
  QString wikipedia;
  QList<quint64> books;

  bool operator==(const EBookAuthorData& rhs);
  Comparison compare(QString forename, QString middlenames, QString surname);
};

typedef QSharedPointer<EBookAuthorData> AuthorData;
typedef QList<AuthorData> AuthorList;
typedef QMap<quint64, AuthorData> AuthorMap;
typedef QMultiMap<QString, AuthorData> AuthorByString;
Q_DECLARE_METATYPE(AuthorData);

class AuthorsDB : public QObject
{
  Q_OBJECT
public:
  explicit AuthorsDB(QObject* parent = nullptr);
  ~AuthorsDB();

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
  AuthorData addAuthor(QString display_name, FileAsList file_as_list);

  QStringList compareAndDiscard(QStringList names);

signals:

public slots:

protected:
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
// typedef QSharedPointer<AuthorsData> AuthorsDB;

#endif // AUTHORS_H
