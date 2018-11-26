#ifndef LIBRARY_H
#define LIBRARY_H

#include <QMap>
#include <QObject>
#include <QSharedPointer>
#include <QList>

#include <list>

#include <qyaml-cpp/node.h>
#include <yaml-cpp/yaml.h>

class Author;
class Book : public QObject
{
  Q_OBJECT
public:
  explicit Book(QObject* parent = nullptr);
  Book(const Book& book);

  QString title() const;
  void setTitle(const QString& title);

  QString isbn() const;
  void setIsbn(const QString& isbn);

  QString filename() const;
  void setFilename(const QString &filename);

signals:

public slots:

protected:
  QString m_filename;
  QString m_title;
  QString m_isbn;

  QList<QSharedPointer<Author>>* m_authors;
};
typedef QSharedPointer<Book> SharedBook;
typedef QList<SharedBook> SharedBookList;
Q_DECLARE_METATYPE(Book)
Q_DECLARE_METATYPE(SharedBook)
Q_DECLARE_METATYPE(SharedBookList)

class Author : public QObject
{
  Q_OBJECT
public:
  enum Comparison {
    SURNAME_MATCH,
    FORE_AND_SURNAME_MATCH,
    FORENAME_MATCH,
    ALL_MATCH,
    PARTIAL_MATCH,
    NO_MATCH,
  };
  explicit Author(QObject* parent = Q_NULLPTR);
  Author(const Author& author);

  QString name();
  bool setName(QString author);
  bool surnameMatch(QString author);

  Author& operator=(const Author&);
  bool operator==(const Author&);

  QString forename() const;
  void setForename(const QString& forename);
  bool compareForename(QString value);

  QString surname() const;
  void setSurname(const QString& surname);
  bool compareSurname(QString value);

  QString middlenames() const;
  void setMiddlenames(const QString& middlenames);
  bool compareMiddlenames(QString value);

  void appendBook(SharedBook book) const;
  SharedBookList* books() const;

  Comparison compare(QString forename, QString middlenames, QString surname);
  QStringList compareAndDiscard(QStringList names);
  bool isEmpty();

signals:

public slots:

protected:
  QString m_forename, m_surname, m_middlenames;
  QString m_lforename, m_lsurname, m_lmiddlenames;

  SharedBookList* m_books;
};
typedef QSharedPointer<Author> SharedAuthor;
typedef QList<SharedAuthor> SharedAuthorList;
Q_DECLARE_METATYPE(Author)
Q_DECLARE_METATYPE(SharedAuthor)
Q_DECLARE_METATYPE(SharedAuthorList)

class Library : public QObject
{
  Q_OBJECT
public:
  explicit Library(QObject* parent = nullptr);

  SharedAuthorList getAuthor(QString author);
  SharedAuthorList getAuthorPartial(QString author);
  SharedAuthor addAuthor(QString author);

  QMultiMap<QString, SharedAuthor> *onName() const;
  void setOnName(QMultiMap<QString, SharedAuthor> *on_name);
  QMultiMap<QString, SharedAuthor> *onPartialName() const;
  void setOnPartialName(QMultiMap<QString, SharedAuthor> *on_partial_name);
  QMap<QString, SharedBookList> *books() const;
  void setBooks(QMap<QString, SharedBookList> *books);




signals:

public slots:

protected:
  QMultiMap<QString, SharedAuthor>* m_on_name;
  QMultiMap<QString, SharedAuthor>* m_on_partial_name;

  QMap<QString, SharedBookList>* m_books;
};
typedef QSharedPointer<Library> SharedLibrary;


//=============================================================================
// YAML-CPP extensions for EBookEdit. These require the QYAML-CPP extensions
// to work as they support several common Qt classes.
//=============================================================================
//void
//operator>>(const YAML::Node& node, Book& book);

//void
//operator<<(YAML::Node node, const Book book);

//void
//operator>>(const YAML::Node& node, SharedBook& book);

//void
//operator<<(YAML::Node node, const SharedBook book);

//void
//operator>>(const YAML::Node& node, SharedBookList& books);

//void
//operator<<(YAML::Node node, const SharedBookList books);

//void
//operator>>(const YAML::Node& node, SharedAuthor& author);

//void
//operator<<(YAML::Node node, const SharedAuthor author);

//void
//operator>>(const YAML::Node& node, SharedLibrary& library);

//void
//operator<<(YAML::Node node, const SharedLibrary library);

//void
//operator>>(const YAML::Node& node, SharedAuthorList& library);

//void
//operator<<(YAML::Node node, const SharedAuthorList library);

#endif // LIBRARY_H
