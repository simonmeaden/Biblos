#ifndef LIBARAY_H
#define LIBARAY_H

#include <QFile>
#include <QObject>
#include <QTextStream>

#include <qyaml-cpp/QYamlCpp>

struct EBookSeriesData {
  EBookSeriesData()
  {
    uid = 0;
  }
  quint64 uid;
  QString name;
};
typedef QSharedPointer<EBookSeriesData> SeriesData;
typedef QMap<quint64, SeriesData> SeriesMap;

struct EBookData {
  EBookData()
  {
    uid = 0;
  }
  quint64 uid;
  QString filename;
  QString title;
  quint64 series;
  QString series_index;
  int current_spine_index;
  int current_spine_lineno;
};
typedef QSharedPointer<EBookData> BookData;
typedef QList<BookData> BookList;
typedef QMap<quint64, BookData> BookMap;
typedef QMultiMap<QString, BookData> BookByTitle;

class LibraryDB : public QObject
{
  Q_OBJECT
public:
  explicit LibraryDB(QObject* parent = nullptr);
  ~LibraryDB();

  void setFilename(QString filename);
  bool save();
  bool load(QString filename);

  quint64 insertBook(BookData data);
  bool removeBook(quint64 index);

  BookData book(quint64 uid);
  BookList book(QString title);

signals:

public slots:

protected:
  QString m_filename;
  BookMap m_book_data;
  BookByTitle m_book_by_title;
  SeriesMap m_book_series;

  bool m_library_changed;

  bool loadLibrary();
  bool saveLibrary();

  static quint64 m_highest_uid;
};
// typedef QSharedPointer<LibraryData> LibraryDB;

#endif // LIBARAY_H
