#ifndef BDIC_H
#define BDIC_H

#include <QDir>
#include <QObject>
#include <QProcess>

#include "options.h"

class DictionaryHandler : public QObject
{
  Q_OBJECT
public:
  explicit DictionaryHandler(Options options, QObject* parent = nullptr);

  QStringList loadDictionaries();
  bool convertDicToBdic(QString in_name, QString out_name);

  QStringList languages() const;

  void addAuthorWord(QString word, QString language);
  void addSeriesWord(QString word, QString language);
  void addWords(QString language,
                QStringList author_words,
                QStringList series_words);

  bool addLanguage(QString language);
  QStringList seriesWords(QString language);
  QStringList authorWords(QString language);

  void setCurrentLanguage(QString language);

signals:
  void finished();
  void bdicConvertFinished();
  void processCrashed(QString);
  void processFailedToStart(QString);
  void processTimedOut(QString);
  void processReadError(QString);
  void processWriteError(QString);

protected:
  Options m_options;
  QDir m_bdic_dir;
  QString m_bdic_app;
  QStringList m_dic_types;
  QStringList m_languages;
  QStringList m_current_language;
  QMap<QString, QPair<QStringList, QStringList>> m_language_map;

  void processError(QProcess::ProcessError error_code);
  void processFinished();

  void setAuthorWords(QString language, QStringList words);
  void setSeriesWords(QString language, QStringList words);
};

#endif // BDIC_H
