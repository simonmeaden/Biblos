#include "dictionaryhandler.h"

DictionaryHandler::DictionaryHandler(Options options, QObject* parent)
  : QObject(parent)
  , m_options(options)
  , m_bdic_dir(options->bdicDir())
  , m_bdic_app("./qwebengine_convert_dict")
{
  m_dic_types << QStringLiteral("*.dic");
}

QStringList
DictionaryHandler::loadDictionaries()
{
  if (!m_bdic_dir.exists()) {
    m_bdic_dir.mkpath(m_options->bdicDir()); // if not already there create it.
    // If you just created it there will not be any dictionaries to create
    return QStringList();
  }
  QStringList languages;
  QStringList files = m_bdic_dir.entryList(m_dic_types, QDir::Files);
  for (int i = 0; i < files.size(); i++) {
    QString filename = files.at(i);
    QFileInfo info(filename);
    QString basename = info.baseName();
    languages.append(basename);
    QString inname = basename + QStringLiteral(".dic");
    QString outname = basename + QStringLiteral(".bdic");
    QFile file(outname);
    if (!file.exists()) {
      convertDicToBdic(inname, outname);
    }
    m_language_map.insert(basename, qMakePair(QStringList(), QStringList()));
  }
  m_languages = languages;
  return languages;
}

bool
DictionaryHandler::convertDicToBdic(QString in_name, QString out_name)
{
  QStringList arguments;
  arguments << in_name << out_name;
  QProcess* process = new QProcess(this);
  connect(process,
          QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
          this,
          &DictionaryHandler::processFinished);
  connect(process,
          QOverload<QProcess::ProcessError>::of(&QProcess::errorOccurred),
          this,
          &DictionaryHandler::processError);
  process->setWorkingDirectory(m_bdic_dir.path());
  process->setProgram(m_bdic_app);
  process->setArguments(arguments);
  process->start();
  QProcess::ProcessError error = process->error();
  if (error != QProcess::UnknownError)
    return false;
  return true;
}

QStringList
DictionaryHandler::languages() const
{
  return m_languages;
}

void
DictionaryHandler::addAuthorWord(QString word, QString language)
{
  QStringList words = authorWords(language);
  if (!words.contains(word)) {
    words.append(word);
    setAuthorWords(language, words);
  }
}

void
DictionaryHandler::addSeriesWord(QString word, QString language)
{
  QStringList words = seriesWords(language);
  if (!words.contains(word)) {
    words.append(word);
    setSeriesWords(language, words);
  }
}

void
DictionaryHandler::setAuthorWords(QString language, QStringList words)
{
  QPair<QStringList, QStringList> pair = m_language_map.value(language);
  pair.first = words;
  m_language_map.insert(language, pair);
}

void
DictionaryHandler::setSeriesWords(QString language, QStringList words)
{
  QPair<QStringList, QStringList> pair = m_language_map.value(language);
  pair.second = words;
  m_language_map.insert(language, pair);
}

void
DictionaryHandler::addWords(QString language,
                            QStringList author_words,
                            QStringList series_words)
{
  QStringList words;
  QPair<QStringList, QStringList> languages = m_language_map.value(language);
  for (int i = 0; i < author_words.size(); i++) {
    QString word = author_words.at(i);
    if (!words.contains(word))
      words.append(word);
    if (!languages.first.contains(word)) {
      languages.first.append(word);
    }
  }
  for (int i = 0; i < series_words.size(); i++) {
    QString word = series_words.at(i);
    if (!words.contains(word))
      words.append(word);
    if (!languages.second.contains(word)) {
      languages.second.append(word);
    }
  }
  if (words.size() > 0) {
    QFile file(m_options->bdicDir() + QDir::separator() + language +
               QStringLiteral(".dic_delta"));
    QTextStream stream(&file);
    if (file.open(QFile::WriteOnly)) {
      for (int i = 0; i < words.size(); i++) {
        QString word = words.at(i);
        stream << word;
        stream << QStringLiteral("\n");
      }
      file.close();
    }
  }
}

QStringList
DictionaryHandler::seriesWords(QString language)
{
  return m_language_map.value(language).second;
}

QStringList
DictionaryHandler::authorWords(QString language)
{
  return m_language_map.value(language).first;
}

void
DictionaryHandler::setCurrentLanguage(QString languages)
{
  m_current_language.clear();
  m_current_language << languages;
}

void
DictionaryHandler::processError(QProcess::ProcessError error_code)
{
  if (error_code == QProcess::ReadError) {
    emit processFailedToStart("");
  } else if (error_code == QProcess::WriteError) {
    emit processWriteError("");
  } else if (error_code == QProcess::Crashed) {
    emit processCrashed("");
  } else if (error_code == QProcess::FailedToStart) {
    emit processFailedToStart("");
  } else if (error_code == QProcess::Timedout) {
    emit processTimedOut("");
  }
}

void
DictionaryHandler::processFinished()
{}
