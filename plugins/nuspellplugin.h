#ifndef NUSPELLPLUGIN_H
#define NUSPELLPLUGIN_H

#include <QObject>
#include <QThread>

#include "ebookcommon.h"
#include "interface.h"
#include "qnuspell.h"

class NuSpellChecker : public QThread {
  Q_OBJECT
public:
  NuSpellChecker(QObject *parent=Q_NULLPTR);
  ~NuSpellChecker();

  void checkWord(QString);

signals:
  void wordCorrect(QString word);
  void wordUnknown(QString word);

protected:
  bool m_running;

  QList<QString> m_good_words;

  // May need more than one of each of these i.e. mapped to book/author.
  QList<QString> m_book_list;
  QList<QString> m_authorList;

  void run();

};

class NuSpellPlugin : public BaseSpellClass
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID SpellInterface_iid FILE "nuspellplugin.json")
  Q_INTERFACES(SpellInterface)

public:
  explicit NuSpellPlugin(QObject* parent = nullptr);
//  explicit NuSpellPlugin(Options* options, QObject* parent = nullptr);

  void setOptions(Options *options);
  // SpellInterface interface
  CountryData* dictionary(QString language_code = QString());
  QStringList languageCodes(QString language_code = QString());
  QStringList compatibleLanguageCodes(QString language_code = QString());
  QString language();
  QString country();
  QString path();
  QString bcp47();
  void buildMenu();

  void checkWord(QString word);
  void addWordToBookList(QString word);
  void addWordToAuthorList(QString word);

protected:
  QNuSpell* m_nuspell;
  NuSpellChecker *spellchecker;

private:
  static const QString DEF_LOCALE;
};


#endif // NUSPELLPLUGIN_H
