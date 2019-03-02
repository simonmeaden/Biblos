#ifndef SERIES_H
#define SERIES_H

#include <QFile>
#include <QObject>
#include <QTextStream>

#include <qyaml-cpp/QYamlCpp>

struct EBookSeriesData
{
  EBookSeriesData();
  ~EBookSeriesData();
  quint64 uid;
  QString name;

  static quint64 m_highest_uid;
  static quint64 nextUid() { return ++m_highest_uid; }
};
typedef QSharedPointer<EBookSeriesData> SeriesData;
typedef QMap<quint64, SeriesData> SeriesMap;
typedef QMap<QString, SeriesData> SeriesByString;
typedef QStringList SeriesList;

class EBookSeriesDB : public QObject
{
  Q_OBJECT
public:
  explicit EBookSeriesDB();
  EBookSeriesDB(const EBookSeriesDB& other);
  ~EBookSeriesDB();

  // yaml file stuff
  void setFilename(QString filename);
  bool save();
  bool load(QString filename);

  // series data stuff
  quint64 insertOrGetSeries(QString series);
  void insertSeries(SeriesData series_data);
  bool removeSeries(quint64 index);

  SeriesMap seriesMap() { return m_series_map; }
  SeriesList seriesList();
  SeriesData series(quint64 uid);
  SeriesData seriesByName(QString name);

protected:
  QString m_filename;
  bool m_series_changed;
  SeriesMap m_series_map;
  SeriesByString m_series_by_name;
  SeriesList m_series_list;

  bool loadSeries();
  bool saveSeries();

  static quint64 m_highest_uid;
};
typedef QSharedPointer<EBookSeriesDB> SeriesDB;

#endif // SERIES_H
