#include "ebookcommon.h"

//= Book ======================================================================
Book::Book(QObject* parent)
  : QObject(parent)
  , m_authors(new QList<QSharedPointer<Author>>())
{
}

Book::Book(const Book& book)
  : QObject(book.parent())
{
  m_title = book.m_title;
  m_isbn = book.m_isbn;
  m_authors = book.m_authors;
}

QString Book::title() const
{
  return m_title;
}

void Book::setTitle(const QString& title)
{
  m_title = title;
}

QString Book::isbn() const
{
  return m_isbn;
}

void Book::setIsbn(const QString& isbn)
{
  m_isbn = isbn;
}

QString Book::filename() const
{
  return m_filename;
}

void Book::setFilename(const QString& filename)
{
  m_filename = filename;
}

//= Author ====================================================================
Author::Author(QObject* parent)
  : QObject(parent)
  , m_books(new QList<SharedBook>())
{
}

Author::Author(const Author& author)
  : QObject(author.parent())
{
  setParent(author.parent());
  m_forename = author.m_forename;
  m_middlenames = author.m_middlenames;
  m_surname = author.m_surname;
  m_books = author.m_books;
}

QString Author::name()
{
  QString name;
  if (!m_forename.isEmpty()) {
    name = m_forename;
  }
  if (!m_middlenames.isEmpty()) {
    name += " " + m_middlenames;
  }
  if (!m_surname.isEmpty()) {
    name += " " + m_surname;
  }
  return name;
}

bool Author::setName(QString author)
{
  QStringList names = author.split(" ");
  if (names.length() == 2) {
    m_forename = names.at(0);
    m_surname = names.at(1);
  } else if (names.length() > 2) {
    m_forename = names.at(0);
    m_surname = names.at(names.size() - 1);
    for (int i = 1; i < names.size() - 2; i++) {
      if (i == 1)
        m_middlenames = names.at(i);
      else
        m_middlenames += " " + names.at(i);
    }
    return true;
  }
  return false;
}

bool Author::surnameMatch(QString author)
{
  // This assumes western type names ie forename middlename surname.
  // TODO handle right to left order names and oriental (surname first) names.
  QStringList names = author.split(" ");
  return (m_surname == names.at(names.size() - 1));
}

Author& Author::operator=(const Author& other)
{
  m_forename = other.m_forename;
  m_surname = other.m_surname;
  m_middlenames = other.m_middlenames;

  return *this;
}

bool Author::operator==(const Author& other)
{
  QString f1 = m_forename;
  QString s1 = m_surname;
  QString m1 = m_middlenames;
  QString f2 = other.m_forename;
  QString s2 = other.m_surname;
  QString m2 = other.m_middlenames;

  return ((f1 == f2) && (s1 == s2) && m1 == m2);
}

QString Author::forename() const
{
  return m_forename;
}

void Author::setForename(const QString& forename)
{
  m_forename = forename;
  m_lforename = forename.toLower();
}

bool Author::compareForename(QString value)
{
  return (value.toLower() == m_lforename);
}

QString Author::surname() const
{
  return m_surname;
}

void Author::setSurname(const QString& surname)
{
  m_surname = surname;
  m_lsurname = surname.toLower();
}

bool Author::compareSurname(QString value)
{
  return (value.toLower() == m_lsurname);
}

QString Author::middlenames() const
{
  return m_middlenames;
}

void Author::setMiddlenames(const QString& middlenames)
{
  m_middlenames = middlenames;
  m_lmiddlenames = middlenames.toLower();
}

bool Author::compareMiddlenames(QString value)
{
  return (value.toLower() == m_lmiddlenames);
}

void Author::appendBook(SharedBook book) const
{
  m_books->append(book);
}

QStringList Author::sortToAlphabeticalOrder(QStringList list)
{
  QMap<QString, QString> map;
  foreach (const QString &str, list)
    map.insert(str.toLower(), str);

  list = map.values();
}

Author::Comparison Author::compare(QString forename, QString middlenames, QString surname)
{
  Comparison match = NO_MATCH;
  if (surname.toLower() == m_lsurname)
    match = SURNAME_MATCH;

  if (forename.toLower() == m_lforename) {
    if (match == SURNAME_MATCH)
      match = FORE_AND_SURNAME_MATCH;
    else
      match = FORENAME_MATCH;
  }

  if (middlenames.toLower() == m_lmiddlenames) {
    if (match == FORE_AND_SURNAME_MATCH)
      match = ALL_MATCH;
    else
      match = PARTIAL_MATCH;
  }

  return match;
}

/*! Cleans unwanted results from QStringList.
 *
 * Checks all supplied strings and only returns those that are names in the
 * author. This comparison ignores string case.
 */
QStringList Author::compareAndDiscard(QStringList names)
{
  QStringList cleaned;
  foreach (QString value, names) {
    QString lower = value.toLower();
    if (lower == m_lforename || lower == m_lmiddlenames || lower == m_lsurname) {
      if (!cleaned.contains(value))
        cleaned += value;
    }
  }
  return cleaned;
}

bool Author::isEmpty()
{
  return (m_surname.isEmpty());
}

QList<SharedBook>* Author::books() const
{
  return m_books;
}

QTextCursor TocDisplayDocument::endOfListItems()
{
  return m_end_of_listitems;
}

int TocDisplayDocument::lineCount()
{
  return m_positioning.size();
}

void TocDisplayDocument::setEndOfListItems(QTextCursor &line_position)
{
  m_end_of_listitems = line_position;
}
