#include "hunspellchecker.h"

/*!
 * \brief A spell checker thread that checks words in a separate
 * thread for higher speeds.
 *
 * \param dict_path
 * \param parent
 */
HunspellChecker::HunspellChecker(QObject* parent)
  : QThread(parent)
  , m_running(true)
  , m_dict_path(QString())
{}

HunspellChecker::HunspellChecker(QString dict_path, QObject* parent)
  : QThread(parent)
  , m_running(true)
  , m_dict_path(dict_path + QDir::separator() + "dict")
{}

HunspellChecker::~HunspellChecker() {}

void
HunspellChecker::stopRunning()
{
  m_running = false;
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
HunspellChecker::checkWord(QString word)
{
  m_words_to_test.append(word);
}

void
HunspellChecker::suggestions(QString word)
{
  m_suggestion_words.append(word);
}

/*
 * The run loop.
 */
void
HunspellChecker::run()
{
  // TODO allow the user to set the libraries.
  QString dic = m_dict_path + QDir::separator() + "en_US.dic";
  QString aff = m_dict_path + QDir::separator() + "en_US.aff";

  Hunspell* m_hunspell =
    new Hunspell(dic.toStdString().c_str(), aff.toStdString().c_str());

  QString word;
  QStringList suggestions;

  while (m_running) {

    suggestions.clear();

    if (m_words_to_test.isEmpty() && m_suggestion_words.isEmpty()) {
      thread()->msleep(300);
    }

    if (!m_words_to_test.isEmpty()) {
      word = m_words_to_test.at(0);
      bool result = m_hunspell->spell(word.toStdString());
      if (result) {
        emit wordCorrect(word);
      } else {
        emit wordUnknown(word);
      }
    }

    if (!m_suggestion_words.isEmpty()) {
      word = m_suggestion_words.at(0);
      std::vector<std::string> list = m_hunspell->suggest(word.toStdString());
      for (std::string str : list) {
        suggestions.append(QString::fromStdString(str));
      }
      emit wordSuggestions(suggestions);
    }
  }
}
