#include "hunspellplugin.h"

#include "hunspellchecker.h"

HunspellPlugin::HunspellPlugin(Options *options, QString dict_path, QObject* parent)
  : BaseSpellClass(options, parent)
{
  m_name = "Hunspell";
  m_plugin_group = "Spellchecker";
  m_vendor = "SM Electronic Components";
  m_major_version = HUNSPELL_VERSION_MAJOR;
  m_minor_version = HUNSPELL_VERSION_MINOR;
  m_build_version = HUNSPELL_VERSION_BUILD;
  m_version = QString("%1.%2.%3")
                .arg(m_major_version)
                .arg(m_minor_version)
                .arg(m_build_version);

  // create spell checker thread.
  m_checker = new HunspellChecker(dict_path, this);

  // pass correct words to correct word handler.
  connect(m_checker,
          &HunspellChecker::wordCorrect,
          this,
          &HunspellPlugin::receivedWordCorrect);
  // pass unknown words straight through.
  connect(m_checker,
          &HunspellChecker::wordUnknown,
          this,
          &HunspellPlugin::wordUnknown);
  // pass suggestions straight through
  connect(m_checker,
          &HunspellChecker::wordSuggestions,
          this,
          &HunspellPlugin::wordSuggestions);

  // start thread
  m_checker->start();
}

/*!
 * \brief Checks a word against various word lists and dictionaries.
 *
 * The plugin first checks the word against maintained Author/Book word lists
 * and a list of words already checked by the spell checker, then
 * against a map of words against a match defined by the user and finally if no
 * in any of those it is passed to the spell checker.
 *
 * \param word - a QString containing the word to check.
 */
void
HunspellPlugin::checkWord(QString word)
{
  if (m_book_list.contains(word) || m_author_list.contains(word) ||
      m_good_words.contains(word)) {
    emit wordCorrect(word);
  } else {
    QString word_match = m_words_matched[word];
    if (word_match.isEmpty())
      m_checker->checkWord(word);
    else
      emit wordMatched(word, word_match);
  }
}

/*!
 * \brief Checks multiple word against various word lists and dictionaries.
 *
 * The plugin first checks the words against maintained Author/Book word lists
 * and a list of words already checked by the spell checker, then
 * against a map of words against a match defined by the user and finally if no
 * in any of those it is passed to the spell checker.
 *
 * This method passes each word individually to be checked by
 * checkWord(QString).
 *
 * \param words - a QStringList containing a number of words to check.
 */
void
HunspellPlugin::checkWords(QStringList words)
{
  foreach (QString word, words) {
    checkWord(word);
  }
}

///*!
// * \brief Adds a word to the book list.
// *
// * This holds a list of special 'Book only' words. Normally names or
// * occasional 'special' made up words, foreign or imaginary languages,
// * or possibly technical words.
// *
// * \param word - a QString containing the word to check.
// * \see addWordToAuthorlist(QString)
// * \see addWordMatch(QString,QString)
// */
//void
//HunspellPlugin::addWordToBookList(QString word)
//{
//  m_book_list.append(word);
//}

///*!
// * \brief Adds a word to an author list.
// *
// * This holds a list of special 'Author only' words. Normally names or
// * occasional 'special' made up words, foreign or imaginary languages,
// * or possibly technical words, the smae as the book list but used for
// * words that are common to an Authors series of books.

// * \param word - a QString containing the word to check.
// * \see addWordToBookList(QString)
// * \see addWordMatch(QString,QString)
// */
//void
//HunspellPlugin::addWordToAuthorList(QString word)
//{
//  m_author_list.append(word);
//}

///*!
// * \brief adds a word match.
// *
// * Genarally used to store a common word mistake.
// *
// * \param word - the word to match against.
// * \param match - the matched word.
// * \see addWordToBookList(QString)
// * \see addWordToAuthorlist(QString)
// */
//void
//HunspellPlugin::addWordMatch(QString word, QString match)
//{
//  m_words_matched[word] = match;
//}

void
HunspellPlugin::suggestions(QString word)
{
  m_checker->suggestions(word);
}

/*
 * Handles a word is correct signal from the spell checker.
 */
void
HunspellPlugin::receivedWordCorrect(QString word)
{
  if (!m_good_words.contains(word)) {
    m_good_words.append(word);
  }
  emit wordCorrect(word);
}

//== HunspellChecker ==========================================================
