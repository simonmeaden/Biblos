#ifndef LIBARAY_H
#define LIBARAY_H

#include <QFile>
#include <QObject>
#include <QTextStream>

#include <qyaml-cpp/QYamlCpp>

struct EBookSeriesData
{
  EBookSeriesData()
    : uid(0)
  {}
  quint64 uid;
  QString name;

  static quint64 m_highest_uid;
  static quint64 nextUid() { return ++m_highest_uid; }
};
typedef QSharedPointer<EBookSeriesData> SeriesData;
typedef QMap<quint64, SeriesData> SeriesMap;
typedef QMap<QString, SeriesData> SeriesByString;
typedef QStringList SeriesList;

struct EBookData
{
  EBookData()
    : uid(0)
    , modified(false)
  {}
  quint64 uid;
  QString filename;
  QString title;
  quint64 series;
  QString series_index;
  int current_spine_index;
  int current_spine_lineno;
  bool modified;

  static quint64 m_highest_uid;
  static quint64 nextUid() { return ++m_highest_uid; }
};
typedef QSharedPointer<EBookData> BookData;
typedef QList<BookData> BookList;
typedef QMap<quint64, BookData> BookMap;
typedef QMultiMap<QString, BookData> BookByString;

class LibraryDB : public QObject
{
  Q_OBJECT
public:
  explicit LibraryDB(QObject* parent = nullptr);
  ~LibraryDB();

  // yaml file stuff
  void setFilename(QString filename);
  bool save();
  bool load(QString filename);

  // series data stuff
  quint64 insertOrGetSeries(QString series);
  bool removeSeries(quint64 index);

  // book stuff.
  quint64 insertOrUpdateBook(BookData book_data);
  bool removeBook(quint64 index);

  BookData bookByUid(quint64 uid);
  BookList bookByTitle(QString title);
  BookData bookByFile(QString filename);

  SeriesList seriesList();

signals:

public slots:

protected:
  QString m_filename;
  BookMap m_book_data;
  BookByString m_book_by_title;
  BookByString m_book_by_file;
  SeriesMap m_series_map;
  SeriesByString m_series_by_name;
  SeriesList m_series_list;

  bool m_library_changed;

  bool loadLibrary();
  bool saveLibrary();

  static quint64 m_highest_uid;
};
// typedef QSharedPointer<LibraryData> LibraryDB;

#endif // LIBARAY_H
