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

class EPubContainer;

class Options {
 public:
  enum CodeOptions {
    NORMAL,
    TAG,
    STRING,
    ATTRIBUTE,
    ERROR,
    STYLE,
    SCRIPT,
  };

  Options();
  ~Options();

  QColor color(CodeOptions options);
  void setColor(CodeOptions options, QColor color);

  QColor background(CodeOptions options);
  void setBackground(CodeOptions options, QColor color);

  bool italic(CodeOptions options);
  void setItalic(CodeOptions options, bool italic);

  QFont::Weight weight(CodeOptions options);
  void setWeight(CodeOptions options, QFont::Weight weight);

  static QColor contrastingColor(QColor color);
  static QString codeOptionToString(CodeOptions options);
  static QString weightToString(QFont::Weight weight);

  QRect rect;
  QSize options_dlg;
  //  int x, y, width, height;
  //  int prefx, prefy, prefwidth, prefheight;
  int popuptimeout;
  int currentindex;
  int bookcount;
  QStringList current_files;
  QStringList current_lib_files;
  bool enablepopup;

  // Code Editor options
  QFont codeFont;
  QColor normalColor;
  QColor normalBack;
  bool normalItalic;
  QFont::Weight normalWeight;
  QColor attributeColor;
  QColor attributeBack;
  bool attributeItalic;
  QFont::Weight attributeWeight;
  QColor tagColor;
  QColor tagBack;
  bool tagItalic;
  QFont::Weight tagWeight;
  QColor stringColor;
  QColor stringBack;
  bool stringItalic;
  QFont::Weight stringWeight;
  QColor errorColor;
  QColor errorBack;
  bool errorItalic;
  QFont::Weight errorWeight;
  QColor scriptColor;
  QColor scriptBack;
  bool scriptItalic;
  QFont::Weight scriptWeight;
  QColor styleColor;
  QColor styleBack;
  bool styleItalic;
  QFont::Weight styleWeight;

  bool copy_books_to_store;
  bool use_calibre_library;
  bool delete_old_book;
  bool never_confirm_delete;
};

struct EBookData {
  EBookData()
  //        : toc(Q_NULLPTR)
  {
    title = "";
    subject = "";
    language = "";
    author_names = "";
    authors.clear();
    date = QDateTime::currentDateTime();
    publisher = "";
  }
  QString title;
  QString subject;
  QString language;
  QString author_names;
  QStringList authors;
  QDateTime date;
  QString publisher;
  //    ebooktoc_t toc;
  QString dict_path;
  QString country;
};
Q_DECLARE_METATYPE(EBookData);

enum DocumentType { PLAINTEXT, HTML };

// struct EBookContents {
//    QHash<QString, QByteArray> m_svgs;
//    QHash<QString, QImage> m_renderedSvgs;
//    EPubContainer *m_container;
//    EBookItem m_currentItem;
//    bool m_loaded;
//};

class Author;
class Book : public QObject {
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

class Author : public QObject {
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

#endif  // COMMON_H
