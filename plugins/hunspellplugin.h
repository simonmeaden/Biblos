#ifndef HUNSPELLPLUGIN_H
#define HUNSPELLPLUGIN_H

#include <QObject>
#include <QThread>
#include <QDir>

#include "interface.h"

class HunspellChecker : public QThread {
  Q_OBJECT
public:
  HunspellChecker(QString dict_path, QObject *parent=Q_NULLPTR);
  ~HunspellChecker();

  void stopRunning();
  void checkWord(QString word);
  void suggestions(QString word);

signals:
  void wordCorrect(QString);
  void wordUnknown(QString);
  void wordSuggestions(QStringList);

protected:
  bool m_running;
  QString m_dict_path;

  QStringList m_words_to_test;
  QStringList m_suggestion_words;

  void run();

};

class HunspellPlugin : public  BaseSpellClass
{
  Q_OBJECT
public:
  explicit HunspellPlugin(QString dict_path, QObject *parent = nullptr);

  void checkWord(QString word);
  void checkWords(QStringList words);
  void addWordToBookList(QString word);
  void addWordToAuthorList(QString word);
  void addWordMatch(QString word, QString match);

  QStringList suggestions(QString word);

signals:
  void wordCorrect(QString);
  void wordUnknown(QString);
  void wordMatched(QString, QString);
  void wordSuggestions(QStringList);

protected:
  HunspellChecker* m_checker;

  // May need more than one of each of these i.e. mapped to different book/author.
  // possibly one spellcheck thread per working book.
  QStringList  m_book_list;
  QStringList  m_author_list;
  QStringList m_good_words;
  QHash<QString, QString> m_words_matched;

  void receivedWordCorrect(QString);


};

#endif // HUNSPELLPLUGIN_H
