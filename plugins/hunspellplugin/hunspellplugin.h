#ifndef HUNSPELLPLUGIN_H
#define HUNSPELLPLUGIN_H

#include <QDir>
#include <QtPlugin>
#include <QThread>

#include "interface_global.h"
#include "ispellinterfaceclass.h"

class HunspellChecker;

class INTERFACESHARED_EXPORT HunspellPlugin : public ISpellInterfaceClass
{
    Q_OBJECT
    Q_INTERFACES(IPluginInterface)
    Q_INTERFACES(ISpellInterface)
public:
    explicit HunspellPlugin(Options *options, QString dict_path = QString(),
                            QObject* parent = Q_NULLPTR);


    void checkWord(QString word) override;
    void checkWords(QStringList words) override;
    void suggestions(QString word) override;

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
