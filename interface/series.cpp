#include "series.h"

quint64 EBookSeriesData::m_highest_uid = 0;

EBookSeriesData::EBookSeriesData()
  : uid(0)
{}

EBookSeriesData::~EBookSeriesData() {}

EBookSeriesDB::EBookSeriesDB()
  : m_series_changed(false)
{}

EBookSeriesDB::EBookSeriesDB(const EBookSeriesDB& other)
{
  m_filename = other.m_filename;
  m_series_map = other.m_series_map;
  m_series_by_name = other.m_series_by_name;
  m_series_list = other.m_series_list;
}

EBookSeriesDB::~EBookSeriesDB()
{
  saveSeries();
}

void
EBookSeriesDB::setFilename(QString filename)
{
  m_filename = filename;
}

bool
EBookSeriesDB::save()
{
  return saveSeries();
}

bool
EBookSeriesDB::load(QString filename)
{
  setFilename(filename);
  return loadSeries();
}

SeriesList
EBookSeriesDB::seriesList()
{
  return m_series_list;
}

SeriesData
EBookSeriesDB::series(quint64 uid)
{
  return m_series_map.value(uid);
}

SeriesData
EBookSeriesDB::seriesByName(QString name)
{
  return m_series_by_name.value(name.toLower());
}

bool
EBookSeriesDB::loadSeries()
{
  if (m_filename.isEmpty()) {
    return false;
  }

  QFile file(m_filename);
  if (file.exists()) {
    YAML::Node library_node = YAML::LoadFile(file);
    if (library_node.IsMap()) {
      for (YAML::const_iterator it = library_node.begin();
           it != library_node.end();
           ++it) {
        SeriesData series = SeriesData(new EBookSeriesData());
        series->uid = it->first.as<quint64>();

        //          YAML::Node series_node = it->second;
        series->name = it->second["name"].as<QString>();

        EBookSeriesData::m_highest_uid =
          (series->uid > EBookSeriesData::m_highest_uid
             ? series->uid
             : EBookSeriesData::m_highest_uid);

        insertSeries(series);
      }
    }
    m_series_changed = false;
  }

  return true;
}

bool
EBookSeriesDB::saveSeries()
{
  if (m_filename.isEmpty())
    return false;

  QFile file(m_filename);
  if (m_series_changed) {
    if (file.open((QFile::ReadWrite | QFile::Truncate))) {
      YAML::Emitter emitter;
      emitter << YAML::Comment(
        QString("A YAML File is supposed to be user readable/editable but\n"
                "you need to be careful when manually editing.\n"
                "Remember that the uid numbers stand for unique identifier\n"
                "so if you edit these MAKE SURE THAT THEY ARE UNIQUE. If\n"
                "you repeat one the second will overwrite the first."));

      emitter << YAML::BeginMap; // series map
      {
        foreach (SeriesData data, m_series_map) {
          emitter << YAML::Key << data->uid; // map key
          emitter << YAML::Value;
          emitter << YAML::BeginMap; // individual series data
          emitter << YAML::Key << "name";
          emitter << YAML::Value << data->name;
          emitter << YAML::EndMap; // end individual series data
        }
      }
      emitter << YAML::EndMap; // end series map

      QTextStream out(&file);
      out << emitter.c_str();
    }

    return true;
  }
  return false;
}

quint64
EBookSeriesDB::insertOrGetSeries(QString series)
{
  if (!m_series_by_name.contains(series.toLower())) {
    SeriesData series_data = SeriesData(new EBookSeriesData());
    quint64 uid = series_data->nextUid();
    series_data->uid = uid;
    series_data->name = series;
    insertSeries(series_data);
    return uid;
  }
  return m_series_by_name.value(series.toLower())->uid;
}

void
EBookSeriesDB::insertSeries(SeriesData series_data)
{
  m_series_map.insert(series_data->uid, series_data);
  m_series_by_name.insert(series_data->name.toLower(), series_data);
  m_series_list.append(series_data->name);
}

bool
EBookSeriesDB::removeSeries(quint64 index)
{
  if (m_series_map.contains(index)) {
    SeriesData data = m_series_map.value(index);
    m_series_map.remove(index);
    m_series_list.removeOne(data->name);
    return true;
  }
  return false;
}
