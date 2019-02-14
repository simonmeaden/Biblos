#include "library.h"

quint64 LibraryDB::m_highest_uid = 0;

LibraryDB::LibraryDB(QObject* parent)
  : QObject(parent)
  , m_library_changed(false)
{
}

LibraryDB::~LibraryDB()
{
  saveLibrary();
}

void LibraryDB::setFilename(QString filename)
{
  m_filename = filename;
}

bool LibraryDB::save()
{
  return saveLibrary();
}

bool LibraryDB::load(QString filename)
{
  setFilename(filename);
  return loadLibrary();
}

quint64 LibraryDB::insertBook(BookData book)
{
  if (book->uid == 0) {
    book->uid = ++m_highest_uid;
  }
  m_book_data.insert(book->uid, book);
  m_book_by_title.insertMulti(book->title, book);
  m_library_changed = true;
  return book->uid;
}

bool LibraryDB::removeBook(quint64 index)
{
  if (m_book_data.contains(index)) {
    BookData book = m_book_data.value(index);
    m_book_data.remove(index);
    m_book_by_title.remove(book->title, book);
    m_library_changed = true;
    return true;
  }
  return false;
}

BookData LibraryDB::book(quint64 uid)
{
  return m_book_data.value(uid);
}

BookList LibraryDB::book(QString title)
{
  return m_book_by_title.values(title);
}

bool LibraryDB::loadLibrary()
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
            m_book_series.insert(series->uid, series);
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

            if (book->uid > m_highest_uid) {
              m_highest_uid = book->uid;
            }

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

bool LibraryDB::saveLibrary()
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
          foreach (SeriesData data, m_book_series) {
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
