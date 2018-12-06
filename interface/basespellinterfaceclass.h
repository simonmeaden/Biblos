#ifndef BASESPELLINTERFACECLASS_H
#define BASESPELLINTERFACECLASS_H

#include <QLocale>
#include <QObject>
#include <QtPlugin>

#include "spellinterface.h"

class Options;

class INTERFACESHARED_EXPORT BaseSpellInterfaceClass : public ISpellInterface
{
  Q_INTERFACES(ISpellInterface)
  Q_PLUGIN_METADATA(IID SpellInterface_iid FILE "spellinterface.json")
public:
  BaseSpellInterfaceClass(Options* options);
  ~BaseSpellInterfaceClass() {}

  void setLocale(QLocale locale);

  CountryData* dictionary(QString language_code = QString());
  QStringList languageCodes(QString language_code = QString());
  QString language();
  QString country();
  QString path();
  QString bcp47();
  QStringList compatibleLanguageCodes(QString language_code);
//  void checkWord(QString word);
//  void checkWords(QStringList words);
  void addWordToBookList(QString word);
  void addWordToAuthorList(QString word);
  void addWordMatch(QString word, QString match);
//  void suggestions(QString word);


protected:
  Options* m_options;
  CountryData* m_data;
  QString m_current_locale;
  QLocale m_locale;

  QStringList m_book_list;
  QStringList m_author_list;
  QStringList m_good_words;
  QHash<QString, QString> m_words_matched;

};

#endif // BASESPELLINTERFACECLASS_H
