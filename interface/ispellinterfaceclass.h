#ifndef BASESPELLINTERFACECLASS_H
#define BASESPELLINTERFACECLASS_H

#include <QLocale>
#include <QObject>
#include <QtPlugin>

#include "ipluginterfaceclass.h"
#include "ispellinterface.h"

class Options;

class INTERFACESHARED_EXPORT ISpellInterfaceClass : public QObject,
    public IPluginInterfaceClass,
    public ISpellInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID SpellInterface_iid FILE "spellinterface.json")
    Q_INTERFACES(ISpellInterface)
public:
    ISpellInterfaceClass(Options *options = Q_NULLPTR,
                         QObject *parent = Q_NULLPTR);
    ~ISpellInterfaceClass() override {}

    virtual void setLocale(QLocale locale);

    // IEBookInterface interface
    //    QString pluginGroup() const override;
    //    virtual QString pluginName() const override;
    //    QString vendor() override;
    //    bool loaded() override;
    //    QString version() override;
    //    int majorVersion() override;
    //    int minorVersion() override;
    //    int buildVersion() override;
    //    void buildMenu() override;

    // ISpellInterface interface
    CountryData *dictionary(QString language_code = QString()) override;
    QStringList languageCodes(QString language_code = QString()) override;
    QStringList compatibleLanguageCodes(QString language_code) override;
    QString language() override;
    QString country() override;
    QString path() override;
    QString bcp47() override;
    void checkWord(QString word) override;
    void checkWords(QStringList words) override;
    void addWordToBookList(QString word) override;
    void addWordToAuthorList(QString word) override;
    void addWordMatch(QString word, QString match) override;
    void suggestions(QString word) override;

signals:
    void wordCorrect(QString word);
    void wordUnknown(QString word);
    void wordMatched(QString, QString);
    void wordSuggestions(QStringList);

protected:
    Options *m_options;
    CountryData *m_data;
    QString m_current_locale;
    QLocale m_locale;

    QStringList m_book_list;
    QStringList m_author_list;
    QStringList m_good_words;
    QHash<QString, QString> m_words_matched;
};

#endif // BASESPELLINTERFACECLASS_H
