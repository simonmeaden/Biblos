#ifndef HUNSPELLPLUGIN_H
#define HUNSPELLPLUGIN_H

#include <QDir>
#include <QThread>

#include "basespellclass.h"

class HunspellChecker;

class HunspellPlugin : public BaseSpellClass
{
  Q_OBJECT
public:
  explicit HunspellPlugin(Options *options, QString dict_path = QString(),
                          QObject* parent = Q_NULLPTR);

  void checkWord(QString word);
  void checkWords(QStringList words);
//  void addWordToBookList(QString word);
//  void addWordToAuthorList(QString word);
//  void addWordMatch(QString word, QString match);

  void suggestions(QString word);

signals:
  void wordCorrect(QString);
  void wordUnknown(QString);
  void wordMatched(QString, QString);
  void wordSuggestions(QStringList);

protected:
  HunspellChecker* m_checker;

  // May need more than one of each of these i.e. mapped to different
  // book/author. possibly one spellcheck thread per working book.
//  QStringList m_book_list;
//  QStringList m_author_list;
//  QStringList m_good_words;
//  QHash<QString, QString> m_words_matched;

  void receivedWordCorrect(QString);
};

#endif // HUNSPELLPLUGIN_H
