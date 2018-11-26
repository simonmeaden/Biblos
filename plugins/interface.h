#ifndef INTERFACE_H
#define INTERFACE_H

#include <QLocale>
#include <QString>
#include <QtPlugin>

class Options;

struct CountryData
{
  QString loc_code;
  QString country_name;
  QString language_name;
  QString bcp47;
  QString script;
  QString path;
};
// typedef CountryData* country_data_t;
// typedef QMap<QString, country_data_t> country_t;
// typedef QList<country_data_t>* compatible_country_t;
// typedef QMap<QString, compatible_country_t> compatibility_map_t;

/*!
 * \brief The interface for all EBookEdit plugins.
 */
class EBookInterface
{
public:
  virtual ~EBookInterface();

  virtual QString name() = 0;
  virtual void buildMenu() = 0;
};
#define EBookInterface_iid "uk.org.smelecomp.EBookInterface/1.0"
Q_DECLARE_INTERFACE(EBookInterface, EBookInterface_iid)

/*!
 * \brief The interface for all spelling plugins.
 */
class SpellInterface : public EBookInterface
{

public:
  virtual ~SpellInterface();

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
  virtual QStringList suggestions(QString word) = 0;

signals:
  void wordCorrect(QString word);
  void wordUnknown(QString word);
  void wordMatched(QString, QString);
};
#define SpellInterface_iid "uk.org.smelecomp.SpellInterface/1.0"
Q_DECLARE_INTERFACE(SpellInterface, SpellInterface_iid)

class BaseSpellClass
  : public QObject
  , public SpellInterface
{
public:
  BaseSpellClass(QObject* parent);
  BaseSpellClass(Options* options, QObject* parent);
  ~BaseSpellClass();

  void setLocale(QLocale locale);

  /*!
   * \brief Gets a map of possible spell checker names against pointers to teh
   * actual objects.
   *
   * Each spell checker MUST have a unique name.
   *
   * \return a pointer to the map.
   */
  static QMap<QString, SpellInterface*>* spellcheckers();
  QString name();
  void buildMenu() {}

protected:
  Options* m_options;
  CountryData* m_data;
  QString m_current_locale;
  QLocale m_locale;

  static QMap<QString, SpellInterface*>* m_spellcheckers;
  static QString NAME;
};

#endif // INTERFACE_H
