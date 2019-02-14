#ifndef HUNSPELLPLUGIN_H
#define HUNSPELLPLUGIN_H

#include <QDir>
#include <QMap>
#include <QThread>
#include <QtPlugin>

#include "interface_global.h"
#include "ispellinterface.h"

class HunspellChecker;
class Options;

class INTERFACESHARED_EXPORT HunspellPlugin : public QObject,
                                              public ISpellInterface {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID SpellInterface_iid FILE "spellinterface.json")
  Q_INTERFACES(IPluginInterface)
  Q_INTERFACES(ISpellInterface)
public:
  explicit HunspellPlugin(QObject *parent = nullptr);
  explicit HunspellPlugin(Options *options, QString dict_path = QString(),
                          QObject *parent = nullptr);

  // IPluginInterface interface
  QString pluginGroup() const override;
  QString pluginName() const override;
  QString vendor() const override;
  bool loaded() const override;
  void setLoaded(bool loaded) override;
  QString version() const override;
  int majorVersion() const override;
  int minorVersion() const override;
  int buildVersion() const override;
  void buildMenu() override;

  // ISpellInterface interface
  CountryData *dictionary(QString language_code) override;
  QStringList languageCodes(QString language_code) override;
  QStringList compatibleLanguageCodes(QString language_code) override;
  QString language() override;
  QString country() override;
  QString path() override;
  QString bcp47() override;

  void checkWord(QString word) override;
  void checkWords(QStringList words) override;
  void suggestions(QString word) override;
  void addWordToBookList(QString word) override;
  void addWordToAuthorList(QString word) override;
  void addWordMatch(QString word, QString match) override;

signals:
  void wordCorrect(QString);
  void wordUnknown(QString);
  void wordMatched(QString, QString);
  void wordSuggestions(QStringList);

protected:
  static const QString m_plugin_group;
  static const QString m_plugin_name;
  static const QString m_vendor;
  static const QString m_version;
  static const int m_major_version;
  static const int m_minor_version;
  static const int m_build_version;
  static bool m_loaded;

  QStringList m_book_list;
  QStringList m_good_words;
  QStringList m_author_list;
  QHash<QString, QString> m_words_matched;

  CountryData *m_data;

  HunspellChecker *m_checker;
  Options *m_options;

  void receivedWordCorrect(QString);
};

#endif // HUNSPELLPLUGIN_H
