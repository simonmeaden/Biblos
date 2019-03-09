#ifndef LIBARAY_H
#define LIBARAY_H

#include <QFile>
#include <QObject>
#include <QTextStream>

#include <qyaml-cpp/QYamlCpp>

#include "series.h"

struct EBookData
{
  EBookData()
    : uid(0)
    , series(0)
    , modified(false)
  {}
  quint64 uid;
  QString filename;
  QString title;
  quint64 series;
  QString series_index;
  QString current_spine_id;
  int current_spine_lineno;
  bool modified;

  static quint64 m_highest_uid;
  static quint64 nextUid() { return ++m_highest_uid; }
};
typedef QSharedPointer<EBookData> BookData;
typedef QList<BookData> BookList;
typedef QMap<quint64, BookData> BookMap;
typedef QMultiMap<QString, BookData> BookByString;

class EBookLibraryDB : public QObject
{
  Q_OBJECT
public:
  explicit EBookLibraryDB(SeriesDB series_db);
  ~EBookLibraryDB();

  // yaml file stuff
  void setFilename(QString filename);
  bool save();
  bool load(QString filename);

  // book stuff.
  quint64 insertOrUpdateBook(BookData book_data);
  bool removeBook(quint64 index);

  BookData bookByUid(quint64 uid);
  BookList bookByTitle(QString title);
  BookData bookByFile(QString filename);

  QString currentBookId(QString filename);

  bool isModified();
  void setModified(bool modified);

signals:

public slots:

protected:
  SeriesDB m_series_db;
  QString m_filename;
  BookMap m_book_data;
  BookByString m_book_by_title;
  BookByString m_book_by_file;

  bool m_modified;

  bool loadLibrary();
  bool saveLibrary();
};
typedef QSharedPointer<EBookLibraryDB> LibraryDB;

#endif // LIBARAY_H
