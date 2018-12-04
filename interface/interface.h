#ifndef INTERFACE_H
#define INTERFACE_H

#include <QLocale>
#include <QObject>
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

  virtual QString pluginGroup() = 0;
  virtual QString name() = 0;
  virtual void buildMenu() = 0;
  virtual QString vendor() = 0;
  virtual bool loaded() = 0;
  virtual QString version() = 0;
  virtual int majorVersion() = 0;
  virtual int minorVersion() = 0;
  virtual int buildVersion() = 0;
};
#define EBookInterface_iid "uk.org.smelecomp.EBookInterface/1.0"
Q_DECLARE_INTERFACE(EBookInterface, EBookInterface_iid)

/*!
 * \brief The interface for all spelling plugins.
 */
class SpellInterface
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
};
#define SpellInterface_iid "uk.org.smelecomp.SpellInterface/1.0"
Q_DECLARE_INTERFACE(SpellInterface, SpellInterface_iid)

class BaseEBookInterfaceClass
  : public QObject
  , public EBookInterface
{
  Q_OBJECT
  Q_INTERFACES(EBookInterface)
public:
  BaseEBookInterfaceClass(QObject* parent = Q_NULLPTR);
  ~BaseEBookInterfaceClass();

  QString pluginGroup();
  QString name();
  QString vendor();
  bool loaded();
  QString version();
  int majorVersion();
  int minorVersion();
  int buildVersion();
  void buildMenu();

protected:
  QString m_name;
  QString m_plugin_group;
  QString m_vendor;
  QString m_version;
  int m_major_version;
  int m_minor_version;
  int m_build_version;
  bool m_loaded;
};

class BaseSpellClass
  : public BaseEBookInterfaceClass
  , public SpellInterface
{
  Q_OBJECT
  Q_INTERFACES(EBookInterface SpellInterface)
public:
  BaseSpellClass(QObject* parent = Q_NULLPTR);
  BaseSpellClass(Options* options, QObject* parent = Q_NULLPTR);
  ~BaseSpellClass();

  void setLocale(QLocale locale);

  static QMap<QString, SpellInterface*>* spellcheckers();

signals:
  void wordCorrect(QString word);
  void wordUnknown(QString word);
  void wordMatched(QString, QString);

protected:
  Options* m_options;
  CountryData* m_data;
  QString m_current_locale;
  QLocale m_locale;

  static QMap<QString, SpellInterface*>* m_spellcheckers;
};

#endif // INTERFACE_H
