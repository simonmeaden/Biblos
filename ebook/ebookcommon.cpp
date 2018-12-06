#include "ebookcommon.h"

QString
Options::codeOptionToString(Options::CodeOptions options)
{
  switch (options) {
    case NORMAL:
      return QObject::tr("Normal");
    case TAG:
      return QObject::tr("Tag");
    case STRING:
      return QObject::tr("String");
    case ATTRIBUTE:
      return QObject::tr("Attribute");
    case ERROR:
      return QObject::tr("Error");
    case STYLE:
      return QObject::tr("Style");
    case SCRIPT:
      return QObject::tr("Script");
  }
  return QString();
}

QString
Options::weightToString(QFont::Weight weight)
{
  switch (weight) {
    case QFont::Thin:
      return "Thin";
    case QFont::ExtraLight:
      return "ExtraLight";
    case QFont::Light:
      return "Light";
    case QFont::Normal:
      return "Normal";
    case QFont::Medium:
      return "Medium";
    case QFont::DemiBold:
      return "DemiBold";
    case QFont::Bold:
      return "Bold";
    case QFont::ExtraBold:
      return "ExtraBold";
    case QFont::Black:
      return "Black";
  }
  return QString();
}

Options::Options()
  : codeFont(QFont("Courier", 10))
  , normalColor(Qt::black)
  , normalBack(Qt::white)
  , normalItalic(false)
  , normalWeight(QFont::Normal)
  , attributeColor(Qt::cyan)
  , attributeBack(Qt::white)
  , attributeItalic(false)
  , attributeWeight(QFont::Normal)
  , tagColor(Qt::blue)
  , tagBack(Qt::white)
  , tagItalic(false)
  , tagWeight(QFont::Normal)
  , stringColor(Qt::green)
  , stringBack(Qt::white)
  , stringItalic(false)
  , stringWeight(QFont::Normal)
  , errorColor(Qt::yellow)
  , errorBack(Qt::red)
  , errorItalic(false)
  , errorWeight(QFont::Normal)
  , scriptColor("mediumorchid")
  , scriptBack(Qt::white)
  , scriptItalic(false)
  , scriptWeight(QFont::Normal)
  , styleColor("silver")
  , styleBack(Qt::white)
  , styleItalic(false)
  , styleWeight(QFont::Normal)
  , copy_books_to_store(true)
  , delete_old_book(false)
  , never_confirm_delete(false)
{}

Options::~Options() {}

QColor
Options::color(Options::CodeOptions options)
{
  switch (options) {
    case Options::NORMAL:
      return normalColor;
    case Options::TAG:
      return tagColor;
    case Options::ATTRIBUTE:
      return attributeColor;
    case Options::ERROR:
      return errorColor;
    case Options::STRING:
      return stringColor;
    case Options::SCRIPT:
      return scriptColor;
    case Options::STYLE:
      return styleColor;
  }
  return QColor();
}

void
Options::setColor(Options::CodeOptions options, QColor color)
{
  switch (options) {
    case Options::NORMAL:
      normalColor = color;
      break;
    case Options::TAG:
      tagColor = color;
      break;
    case Options::ATTRIBUTE:
      attributeColor = color;
      break;
    case Options::ERROR:
      errorColor = color;
      break;
    case Options::STRING:
      stringColor = color;
      break;
    case Options::SCRIPT:
      scriptColor = color;
      break;
    case Options::STYLE:
      styleColor = color;
      break;
  }
}

QColor
Options::background(Options::CodeOptions options)
{
  switch (options) {
    case Options::NORMAL:
      return normalBack;
    case Options::TAG:
      return tagBack;
    case Options::ATTRIBUTE:
      return attributeBack;
    case Options::ERROR:
      return errorBack;
    case Options::STRING:
      return stringBack;
    case Options::SCRIPT:
      return scriptBack;
    case Options::STYLE:
      return styleBack;
  }
  return QColor();
}

void
Options::setBackground(Options::CodeOptions options, QColor color)
{
  switch (options) {
    case Options::NORMAL:
      normalBack = color;
      break;
    case Options::TAG:
      tagBack = color;
      break;
    case Options::ATTRIBUTE:
      attributeBack = color;
      break;
    case Options::ERROR:
      errorBack = color;
      break;
    case Options::STRING:
      stringBack = color;
      break;
    case Options::SCRIPT:
      scriptBack = color;
      break;
    case Options::STYLE:
      styleBack = color;
      break;
  }
}

bool
Options::italic(Options::CodeOptions options)
{
  switch (options) {
    case Options::NORMAL:
      return normalItalic;
    case Options::TAG:
      return tagItalic;
    case Options::ATTRIBUTE:
      return attributeItalic;
    case Options::ERROR:
      return errorItalic;
    case Options::STRING:
      return stringItalic;
    case Options::SCRIPT:
      return scriptItalic;
    case Options::STYLE:
      return styleItalic;
  }
  return false;
}

void
Options::setItalic(Options::CodeOptions options, bool italic)
{
  switch (options) {
    case Options::NORMAL:
      normalItalic = italic;
      break;
    case Options::TAG:
      tagItalic = italic;
      break;
    case Options::ATTRIBUTE:
      attributeItalic = italic;
      break;
    case Options::ERROR:
      errorItalic = italic;
      break;
    case Options::STRING:
      stringItalic = italic;
      break;
    case Options::SCRIPT:
      scriptItalic = italic;
      break;
    case Options::STYLE:
      styleItalic = italic;
      break;
  }
}

QFont::Weight
Options::weight(Options::CodeOptions options)
{
  switch (options) {
    case Options::NORMAL:
      return normalWeight;
    case Options::TAG:
      return tagWeight;
    case Options::ATTRIBUTE:
      return attributeWeight;
    case Options::ERROR:
      return errorWeight;
    case Options::STRING:
      return stringWeight;
    case Options::SCRIPT:
      return scriptWeight;
    case Options::STYLE:
      return styleWeight;
  }
  return QFont::Normal;
}

void
Options::setWeight(Options::CodeOptions options, QFont::Weight weight)
{
  switch (options) {
    case Options::NORMAL:
      normalWeight = weight;
      break;
    case Options::TAG:
      tagWeight = weight;
      break;
    case Options::ATTRIBUTE:
      attributeWeight = weight;
      break;
    case Options::ERROR:
      errorWeight = weight;
      break;
    case Options::STRING:
      stringWeight = weight;
      break;
    case Options::SCRIPT:
      scriptWeight = weight;
      break;
    case Options::STYLE:
      styleWeight = weight;
      break;
  }
}

QColor
Options::contrastingColor(QColor color)
{
  int v = (color.red() + color.green() + color.blue()) / 3 > 127 ? 0 : 255;
  return QColor(v, v, v);
}

//= Book ======================================================================
Book::Book(QObject* parent)
  : QObject(parent)
  , m_authors(new QList<QSharedPointer<Author>>())
{}

Book::Book(const Book& book)
  : QObject(book.parent())
{
  m_title = book.m_title;
  m_isbn = book.m_isbn;
  m_authors = book.m_authors;
}

QString
Book::title() const
{
  return m_title;
}

void
Book::setTitle(const QString& title)
{
  m_title = title;
}

QString
Book::isbn() const
{
  return m_isbn;
}

void
Book::setIsbn(const QString& isbn)
{
  m_isbn = isbn;
}

QString Book::filename() const
{
  return m_filename;
}

void Book::setFilename(const QString &filename)
{
  m_filename = filename;
}

//= Author ====================================================================
Author::Author(QObject* parent)
  : QObject(parent)
  , m_books(new QList<SharedBook>())
{}

Author::Author(const Author& author)
  : QObject(author.parent())
{
  setParent(author.parent());
  m_forename = author.m_forename;
  m_middlenames = author.m_middlenames;
  m_surname = author.m_surname;
  m_books = author.m_books;
}

QString
Author::name()
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

bool
Author::setName(QString author)
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

bool
Author::surnameMatch(QString author)
{
  // This assumes western type names ie forename middlename surname.
  // TODO handle right to left order names.
  QStringList names = author.split(" ");
  return (m_surname == names.at(names.size() - 1));
}

Author&
Author::operator=(const Author& other)
{
  m_forename = other.m_forename;
  m_surname = other.m_surname;
  m_middlenames = other.m_middlenames;

  return *this;
}

bool
Author::operator==(const Author& other)
{
  QString f1 = m_forename;
  QString s1 = m_surname;
  QString m1 = m_middlenames;
  QString f2 = other.m_forename;
  QString s2 = other.m_surname;
  QString m2 = other.m_middlenames;

  return ((f1 == f2) && (s1 == s2) && m1 == m2);
}

QString
Author::forename() const
{
  return m_forename;
}

void
Author::setForename(const QString& forename)
{
  m_forename = forename;
  m_lforename = forename.toLower();
}

bool
Author::compareForename(QString value)
{
  return (value.toLower() == m_lforename);
}

QString
Author::surname() const
{
  return m_surname;
}

void
Author::setSurname(const QString& surname)
{
  m_surname = surname;
  m_lsurname = surname.toLower();
}

bool
Author::compareSurname(QString value)
{
  return (value.toLower() == m_lsurname);
}

QString
Author::middlenames() const
{
  return m_middlenames;
}

void
Author::setMiddlenames(const QString& middlenames)
{
  m_middlenames = middlenames;
  m_lmiddlenames = middlenames.toLower();
}

bool
Author::compareMiddlenames(QString value)
{
  return (value.toLower() == m_lmiddlenames);
}

void
Author::appendBook(SharedBook book) const
{
  m_books->append(book);
}

Author::Comparison
Author::compare(QString forename, QString middlenames, QString surname)
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
QStringList
Author::compareAndDiscard(QStringList names)
{
  QStringList cleaned;
  foreach (QString value, names) {
    QString lower = value.toLower();
    if (lower == m_lforename || lower == m_lmiddlenames ||
        lower == m_lsurname) {
      if (!cleaned.contains(value))
        cleaned += value;
    }
  }
  return cleaned;
}

bool
Author::isEmpty()
{
  return (m_surname.isEmpty());
}

QList<SharedBook>*
Author::books() const
{
  return m_books;
}
