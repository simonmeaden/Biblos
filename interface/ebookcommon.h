#ifndef COMMON_H
#define COMMON_H

#include <QDateTime>
#include <QFont>
#include <QHash>
#include <QImage>
#include <QMap>
#include <QObject>
#include <QPair>
#include <QPoint>
#include <QSharedPointer>
#include <QtPlugin>
#include <QTextCursor>
#include <QTextDocument>

// struct EPubPageReference {
//  enum StandardType {
//    CoverPage,
//    TitlePage,
//    TableOfContents,
//    Index,
//    Glossary,
//    Acknowledgements,
//    Bibliography,
//    Colophon,
//    CopyrightPage,
//    Dedication,
//    Epigraph,
//    Foreword,
//    ListOfIllustrations,
//    ListOfTables,
//    Notes,
//    Preface,
//    Text,
//    Other
//  };

//  static StandardType typeFromString(const QString& name);

//  QString target;
//  QString title;
//};

struct TocLinePosition {
  QTextCursor start;
  QTextCursor end;

  bool isValid()
  {
    return (start.position() != end.position());
  }
};

class TocDisplayDocument : public QTextDocument
{
  Q_OBJECT
public:
  TocDisplayDocument(QObject* parent);
  ~TocDisplayDocument();

  void addLinePosition(int line_index, QTextCursor& line_position);
  QTextCursor linePosition(int index);
  void setEndOfListItems(QTextCursor& line_position);
  QTextCursor endOfListItems();
  int lineCount();

protected:
  QMap<int, QTextCursor> m_positioning;
  QTextCursor m_end_of_listitems;
};



struct EPubNavPoint {
  EPubNavPoint(QString classname, QString id, QString label, QString src)
  {
    this->classname = classname;
    this->id = id;
    this->label = label;
    this->src = src;
  }
  QString classname;
  QString id;
  QString label;
  QString src;
};
typedef QSharedPointer<EPubNavPoint> navpoint_t;

enum DocumentType { PLAINTEXT, HTML };
enum EBookType { UNSUPPORTED_TYPE, EPUB, MOBI, AZW, PDF, };

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
  void setFilename(const QString& filename);

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

  static QStringList sortToAlphabeticalOrder(QStringList list);

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

#endif // COMMON_H
