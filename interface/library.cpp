#include "library.h"

quint64 EBookData::m_highest_uid = 0;

EBookLibraryDB::EBookLibraryDB(SeriesDB series_db)
  : m_series_db(series_db)
  , m_modified(false)
{}

EBookLibraryDB::~EBookLibraryDB()
{
  saveLibrary();
}

void
EBookLibraryDB::setFilename(QString filename)
{
  m_filename = filename;
}

bool
EBookLibraryDB::save()
{
  return saveLibrary();
}

bool
EBookLibraryDB::load(QString filename)
{
  setFilename(filename);
  return loadLibrary();
}

quint64
EBookLibraryDB::insertOrUpdateBook(BookData book_data)
{
  if (book_data->uid == 0) { // should already be set.
    book_data->uid = ++EBookData::m_highest_uid;
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
    m_book_by_title.insertMulti(book_data->title.toLower(), book_data);
    m_book_by_file.insert(book_data->filename, book_data);
    m_modified = true;
  }
  return book_data->uid;
}

bool
EBookLibraryDB::removeBook(quint64 index)
{
  if (m_book_data.contains(index)) {
    BookData book = m_book_data.value(index);
    m_book_data.remove(index);
    m_book_by_title.remove(book->title.toLower(), book);
    m_book_by_file.remove(book->filename, book);
    m_modified = true;
    return true;
  }
  return false;
}

BookData
EBookLibraryDB::bookByUid(quint64 uid)
{
  return m_book_data.value(uid);
}

BookList
EBookLibraryDB::bookByTitle(QString title)
{
  return m_book_by_title.values(title.toLower());
}

BookData
EBookLibraryDB::bookByFile(QString filename)
{
  return m_book_by_file.value(filename);
}

bool
EBookLibraryDB::isModified()
{
  return m_modified;
}

void
EBookLibraryDB::setModified(bool modified)
{
  m_modified = modified;
}

bool
EBookLibraryDB::loadLibrary()
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
        BookData book = BookData(new EBookData());
        book->uid = it->first.as<quint64>();

        YAML::Node book_node = it->second;
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
        m_book_by_title.insert(book->title.toLower(), book);
        m_book_by_file.insert(book->filename, book);
      }
    }
    m_modified = false;
    return true;
  }

  return false;
}

bool
EBookLibraryDB::saveLibrary()
{
  if (m_filename.isEmpty())
    return false;

  QFile file(m_filename);
  if (m_modified) {
    if (file.open((QFile::ReadWrite | QFile::Truncate))) {
      YAML::Emitter emitter;
      emitter << YAML::Comment(
        QString("A YAML File is supposed to be user readable/editable but\n"
                "you need to be careful when manually editing.\n"
                "Remember that the uid numbers stand for unique identifier\n"
                "so if you edit these MAKE SURE THAT THEY ARE UNIQUE. If\n"
                "you repeat one the second will overwrite the first."));

      emitter << YAML::BeginMap; // books map
      {
        foreach (BookData book_data, m_book_data) {
          emitter << YAML::Key << book_data->uid;
          emitter << YAML::Value;
          emitter << YAML::BeginMap;
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
          emitter << YAML::EndMap; // individual book map
        }
      }
      emitter << YAML::EndMap; // end books map

      QTextStream out(&file);
      out << emitter.c_str();
    }

    return true;
  }
  return false;

} // library changed
