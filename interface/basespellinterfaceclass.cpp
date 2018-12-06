#include "basespellinterfaceclass.h"

BaseSpellInterfaceClass::BaseSpellInterfaceClass(Options* options)
  : m_options(options)
{}

void
BaseSpellInterfaceClass::setLocale(QLocale locale)
{
  m_locale = locale;
}

CountryData*
BaseSpellInterfaceClass::dictionary(QString language_code)
{
  return m_data;
}

QStringList
BaseSpellInterfaceClass::languageCodes(QString language_code)
{}

QString
BaseSpellInterfaceClass::language()
{
  return m_data->language_name;
}

QString
BaseSpellInterfaceClass::country()
{
  return m_data->country_name;
}

QString
BaseSpellInterfaceClass::path()
{
  return m_data->path;
}

QString
BaseSpellInterfaceClass::bcp47()
{
  return m_data->bcp47;
}

QStringList
BaseSpellInterfaceClass::compatibleLanguageCodes(QString language_code)
{ // TODO
}

// void
// BaseSpellInterfaceClass::checkWord(QString word)
//{}

// void
// BaseSpellInterfaceClass::checkWords(QStringList words)
//{}

/*!
 * \brief Adds a word to the book list.
 *
 * This holds a list of special 'Book only' words. Normally names or
 * occasional 'special' made up words, foreign or imaginary languages,
 * or possibly technical words.
 *
 * \param word - a QString containing the word to check.
 * \see addWordToAuthorlist(QString)
 * \see addWordMatch(QString,QString)
 */
void
BaseSpellInterfaceClass::addWordToBookList(QString word)
{
  m_book_list.append(word);
}

/*!
 * \brief Adds a word to an author list.
 *
 * This holds a list of special 'Author only' words. Normally names or
 * occasional 'special' made up words, foreign or imaginary languages,
 * or possibly technical words, the smae as the book list but used for
 * words that are common to an Authors series of books.

 * \param word - a QString containing the word to check.
 * \see addWordToBookList(QString)
 * \see addWordMatch(QString,QString)
 */
void
BaseSpellInterfaceClass::addWordToAuthorList(QString word)
{
  m_author_list.append(word);
}

/*!
 * \brief adds a word match.
 *
 * Genarally used to store a common word mistake.
 *
 * \param word - the word to match against.
 * \param match - the matched word.
 * \see addWordToBookList(QString)
 * \see addWordToAuthorlist(QString)
 */
void
BaseSpellInterfaceClass::addWordMatch(QString word, QString match)
{
  m_words_matched[word] = match;
}
