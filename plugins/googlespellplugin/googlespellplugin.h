#ifndef GOOGLESPELLPLUGIN_H
#define GOOGLESPELLPLUGIN_H

#include <QObject>

#include "interface.h"

class GoogleSpellPlugin
  : public BaseSpellClass
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID EBookInterface_iid FILE "google.json")
  Q_PLUGIN_METADATA(IID SpellInterface_iid FILE "google.json")
public:
  explicit GoogleSpellPlugin(QObject* parent = nullptr);

  // SpellInterface interface
  CountryData* dictionary(QString language_code = QString());
  QStringList languageCodes(QString language_code = QString());
  QString language();
  QString country();
  QString path();
  QString bcp47();
  void buildMenu();
  QStringList compatibleLanguageCodes(QString language_code);
  void checkWord(QString word);
  void checkWords(QStringList words);
  void addWordToBookList(QString word);
  void addWordToAuthorList(QString word);
  void addWordMatch(QString word, QString match);
  QStringList suggestions(QString word);

public:
};

#endif // GOOGLESPELLPLUGIN_H
