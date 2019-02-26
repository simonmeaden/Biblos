#include "library.h"

quint64 LibraryDB::m_highest_uid = 0;
quint64 EBookData::m_highest_uid = 0;
quint64 EBookSeriesData::m_highest_uid = 0;

LibraryDB::LibraryDB(QObject* parent)
  : QObject(parent)
  , m_library_changed(false)
{}

LibraryDB::~LibraryDB()
{
  saveLibrary();
}

void
LibraryDB::setFilename(QString filename)
{
  m_filename = filename;
}

bool
LibraryDB::save()
{
  return saveLibrary();
}

bool
LibraryDB::load(QString filename)
{
  setFilename(filename);
  return loadLibrary();
}

quint64
LibraryDB::insertOrGetSeries(QString series)
{
  if (!m_series_by_name.contains(series)) {
    SeriesData series_data = SeriesData(new EBookSeriesData());
    quint64 uid = series_data->nextUid();
    series_data->uid = uid;
    series_data->name = series;
    m_series_map.insert(uid, series_data);
    m_series_by_name.insert(series, series_data);
    m_series_list.append(series);
    return uid;
  }
  return m_series_by_name.value(series)->uid;
}

bool
LibraryDB::removeSeries(quint64 index)
{
  if (m_series_map.contains(index)) {
    SeriesData data = m_series_map.value(index);
    m_series_map.remove(index);
    m_series_list.removeOne(data->name);
    return true;
  }
  return false;
}

quint64
LibraryDB::insertOrUpdateBook(BookData book_data)
{
  if (book_data->uid == 0) { // should already be set.
    book_data->uid = ++m_highest_uid;
  }
  if (m_book_data.contains(book_data->uid) && book_data->modified) {
    BookData existing_book_data = m_book_data.value(book_data->uid);
    existing_book_data->uid = book_data->uid;
    existing_book_data->filename = book_data->filename;
    existing_book_data->title = book_data->title;
    existing_book_data->series = book_data->series;
    existing_book_data->series_index = book_data->series_index;
    existing_book_data->current_spine_index = book_data->current_spine_index;
    existing_book_data->current_spine_lineno = book_data->current_spine_lineno;
  } else {
    m_book_data.insert(book_data->uid, book_data);
    m_book_by_title.insertMulti(book_data->title, book_data);
    m_book_by_file.insert(book_data->filename, book_data);
    m_library_changed = true;
  }
  return book_data->uid;
}

bool
LibraryDB::removeBook(quint64 index)
{
  if (m_book_data.contains(index)) {
    BookData book = m_book_data.value(index);
    m_book_data.remove(index);
    m_book_by_title.remove(book->title, book);
    m_book_by_file.remove(book->filename, book);
    m_library_changed = true;
    return true;
  }
  return false;
}

BookData
LibraryDB::bookByUid(quint64 uid)
{
  return m_book_data.value(uid);
}

BookList
LibraryDB::bookByTitle(QString title)
{
  return m_book_by_title.values(title);
}

BookData
LibraryDB::bookByFile(QString filename)
{
  return m_book_by_file.value(filename);
}

SeriesList
LibraryDB::seriesList()
{
  return m_series_list;
}

bool
LibraryDB::loadLibrary()
{
  if (m_filename.isEmpty()) {
    return false;
  }

  QFile file(m_filename);
  YAML::Node library_node;
  if (file.exists()) {
    library_node = YAML::LoadFile(file);
    if (library_node.IsMap()) {
      if (library_node["series"] && library_node["series"].IsSequence()) {
        YAML::Node series_list = library_node["series"];
        for (std::size_t i = 0; i < series_list.size(); i++) {
          YAML::Node series_node = series_list[i];
          if (series_node.IsMap()) {
            SeriesData series = SeriesData(new EBookSeriesData());
            series->uid = series_node["uid"].as<quint64>();
            series->name = series_node["name"].as<QString>();

            EBookSeriesData::m_highest_uid =
              (series->uid > EBookSeriesData::m_highest_uid
                 ? series->uid
                 : EBookSeriesData::m_highest_uid);

            m_series_map.insert(series->uid, series);
            m_series_list.append(series->name);
          }
        }
      }

      if (library_node["books"] && library_node["books"].IsSequence()) {
        YAML::Node books = library_node["books"];
        for (std::size_t i = 0; i < books.size(); i++) {
          YAML::Node book_node = books[i];
          if (book_node.IsMap()) {
            BookData book = BookData(new EBookData());
            book->uid = book_node["uid"].as<quint64>();
            book->title = book_node["title"].as<QString>();
            book->filename = book_node["filename"].as<QString>();
            book->series = book_node["series uid"].as<quint64>();
            book->series_index = book_node["series index"].as<QString>();
            book->current_spine_index = book_node["spine index"].as<int>();
            book->current_spine_lineno = book_node["spine lineno"].as<int>();

            EBookData::m_highest_uid =
              (book->uid > EBookData::m_highest_uid ? book->uid
                                                    : EBookData::m_highest_uid);

            m_book_data.insert(book->uid, book);
            m_book_by_title.insert(book->title, book);
          }
        }
      }
    }
    m_library_changed = false;
  }

  return true;
}

bool
LibraryDB::saveLibrary()
{
  if (m_filename.isEmpty())
    return false;

  QFile file(m_filename);
  if (m_library_changed) {
    if (file.open((QFile::ReadWrite | QFile::Truncate))) {
      YAML::Emitter emitter;
      emitter << YAML::Comment(
        QString("A YAML File is supposed to be user readable/editable but\n"
                "you need to be careful when manually editing.\n"
                "Remember that the uid numbers stand for unique identifier\n"
                "so if you edit these MAKE SURE THAT THEY ARE UNIQUE. If\n"
                "you repeat one the second will overwrite the first."));
      emitter << YAML::BeginMap; // series & books so far
      {
        emitter << YAML::Key << ("series");
        emitter << YAML::BeginSeq; // series sequence
        {
          foreach (SeriesData data, m_series_map) {
            emitter << YAML::BeginMap; // individual series map
            emitter << YAML::Key << "uid";
            emitter << YAML::Value << data->uid;
            emitter << YAML::Key << "name";
            emitter << YAML::Value << data->name;
            emitter << YAML::EndMap; // end individual series map
          }
        }
        emitter << YAML::EndSeq; // end series sequence

        emitter << YAML::Key << "books";
        emitter << YAML::BeginSeq;
        {
          foreach (BookData book_data, m_book_data) {
            emitter << YAML::BeginMap;
            emitter << YAML::Key << "uid";
            emitter << YAML::Value << book_data->uid;
            emitter << YAML::Key << "title";
            emitter << YAML::Value << book_data->title;
            emitter << YAML::Key << "filename";
            emitter << YAML::Value << book_data->filename;
            emitter << YAML::Key << "series uid";
            emitter << YAML::Value << book_data->series;
            emitter << YAML::Key << "series index";
            emitter << YAML::Value << book_data->series_index;
            emitter << YAML::Key << "spine index";
            emitter << YAML::Value << book_data->current_spine_index;
            emitter << YAML::Key << "spine lineno";
            emitter << YAML::Value << book_data->current_spine_lineno;
            emitter << YAML::EndMap;
          }
        }
        emitter << YAML::EndSeq;
      }
      emitter << YAML::EndMap;

      QTextStream out(&file);
      out << emitter.c_str();
      return true;
    }
  } // library changed
  return false;
}
