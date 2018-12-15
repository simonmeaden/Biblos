#ifndef SPELLINTERFACE_H
#define SPELLINTERFACE_H

#include <QObject>

#include "interface_global.h"

struct CountryData
{
  QString loc_code;
  QString country_name;
  QString language_name;
  QString bcp47;
  QString script;
  QString path;
};

/*!
 * \brief The interface for all spelling plugins.
 */
class INTERFACESHARED_EXPORT ISpellInterface
{
public:
  virtual ~ISpellInterface() {}

  virtual CountryData* dictionary(QString language_code = QString()) = 0;
  virtual QStringList languageCodes(QString language_code = QString()) = 0;
  virtual QStringList compatibleLanguageCodes(
    QString language_code = QString()) = 0;
  virtual QString language() = 0;
  virtual QString country() = 0;
  virtual QString path() = 0;
  virtual QString bcp47() = 0;

  virtual void checkWord(QString word) = 0;
  virtual void checkWords(QStringList words) = 0;
  virtual void addWordToBookList(QString word) = 0;
  virtual void addWordToAuthorList(QString word) = 0;
  virtual void addWordMatch(QString word, QString match) = 0;
  virtual void suggestions(QString word) = 0;
};
#define SpellInterface_iid "uk.org.smelecomp.SpellInterface/1.0"
Q_DECLARE_INTERFACE(ISpellInterface, SpellInterface_iid)

#endif // SPELLINTERFACE_H
