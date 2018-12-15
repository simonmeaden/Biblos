#include "ispellinterfaceclass.h"

#include "ebookcommon.h"

ISpellInterfaceClass::ISpellInterfaceClass(Options *options, QObject *parent)
    : QObject(parent), m_options(options) {}

void ISpellInterfaceClass::setLocale(QLocale locale)
{
    m_locale = locale;
}

CountryData *ISpellInterfaceClass::dictionary(QString /*language_code*/)
{
    return m_data;
}

QStringList ISpellInterfaceClass::languageCodes(QString /*language_code*/)
{
    // TODO just a placeholder.
    return QStringList();
}

QString ISpellInterfaceClass::language()
{
    return m_data->language_name;
}

QString ISpellInterfaceClass::country()
{
    return m_data->country_name;
}

QString ISpellInterfaceClass::path()
{
    return m_data->path;
}

QString ISpellInterfaceClass::bcp47()
{
    return m_data->bcp47;
}

QStringList
ISpellInterfaceClass::compatibleLanguageCodes(QString /*language_code*/)
{
    // TODO just a placeholder.
    return QStringList();
}

void ISpellInterfaceClass::checkWord(QString /*word*/)
{
    // TODO just a placeholder.
}

void ISpellInterfaceClass::checkWords(QStringList /*words*/)
{
    // TODO just a placeholder.
}

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
void ISpellInterfaceClass::addWordToBookList(QString word)
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
void ISpellInterfaceClass::addWordToAuthorList(QString word)
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
void ISpellInterfaceClass::addWordMatch(QString word, QString match)
{
    m_words_matched[word] = match;
}

void ISpellInterfaceClass::suggestions(QString /*word*/)
{
    // TODO just a placeholder.
}
