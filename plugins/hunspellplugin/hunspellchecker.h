#ifndef HUNSPELLCHECKER_H
#define HUNSPELLCHECKER_H

#include <QThread>
#include <QDir>

#include <hunspell/hunspell.hxx>

class HunspellChecker : public QThread
{
    Q_OBJECT

public:
    HunspellChecker(QObject* parent = Q_NULLPTR);
    HunspellChecker(QString dict_path, QObject* parent = Q_NULLPTR);
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

#endif // HUNSPELLCHECKER_H
