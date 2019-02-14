#include "authors.h"

quint64 AuthorsDB::m_highest_uid = 0;

AuthorsDB::AuthorsDB(QObject* parent)
  : QObject(parent)
  , m_author_changed(false)
{
}

AuthorsDB::~AuthorsDB()
{
  saveAuthors();
}

void AuthorsDB::setFilename(QString filename)
{
  m_filename = filename;
}

bool AuthorsDB::save()
{
  return saveAuthors();
}

bool AuthorsDB::load(QString filename)
{
  setFilename(filename);
  return loadAuthors();
}

quint64 AuthorsDB::insertAuthor(AuthorData author)
{
  if (!author)
    return 0;

  if (author->uid == 0) {
    author->uid = ++m_highest_uid;
  }
  m_author_data.insert(author->uid, author);
  m_author_by_surname.insert(author->surname.toLower(), author);
  m_author_by_forename.insert(author->forename.toLower(), author);
  if (!author->display_name.isEmpty()) {
    // TODO handle asian type surname first format.
    author->display_name = author->forename;
    author->display_name +=
      (author->middlenames.isEmpty() ? "" : " " + author->middlenames);
    author->display_name += " " + author->surname;
  }
  m_author_by_displayname.insert(author->display_name, author);
  if (!author->file_as.isEmpty()) {
    author->file_as =
      author->surname + ", " + author->forename +
      (author->middlenames.isEmpty() ? "" : " " + author->middlenames);
  }
  m_author_by_fileas.insert(author->file_as.toLower(), author);
  m_author_changed = true;
  return true;
}

QStringList AuthorsDB::compareAndDiscard(QStringList names)
{
  QStringList cleaned;
  foreach (QString value, names) {
    QString lower = value.toLower();
    // check if the surname list has a match.
    AuthorList list = m_author_by_surname.values(value);
    foreach (AuthorData data, list) {
      if (lower == data->forename.toLower() ||
          lower == data->surname.toLower()) {
        if (!cleaned.contains(value))
          cleaned += value;
      }
    }
    // check if the forename list has a match.
    list = m_author_by_forename.values(value);
    foreach (AuthorData data, list) {
      if (lower == data->forename.toLower() ||
          lower == data->surname.toLower()) {
        if (!cleaned.contains(value))
          cleaned += value;
      }
    }
  }
  return cleaned;
}

bool AuthorsDB::removeBook(quint64 index)
{
  if (m_author_data.contains(index)) {
    AuthorData author = m_author_data.value(index);
    m_author_data.remove(index);
    m_author_by_displayname.remove(author->display_name, author);
    m_author_by_fileas.remove(author->file_as, author);
    m_author_changed = true;
    return true;
  }
  return false;
}

/*!
 * \brief gets the author data by display name if it exists otherwise attempts
 * to use surname only.
 *
 * \param name - display name.
 * \return the relevant AuthorData object.
 */
AuthorData AuthorsDB::author(QString name)
{
  AuthorData data;
  data = m_author_by_displayname.value(name);
  if (!data.isNull()) {
    return data;
  }

  /* The display name is normally in the form 'FORENAME SURNAME' or
   * vicky-verky  in the case of asian names.
   */
  QStringList splits = name.split(" ");
  if (splits.size() > 1) {
    if (splits.size() == 2) { // normal case unless middle names are supplied.
      if (m_author_by_surname.contains(splits.last())) {
        data = m_author_by_surname.value(splits.last());
        if (data->forename == splits.first()) {
          return data;
        } else {
          // TODO different surname?
        }
      }
    } else {
    }
  } else {
  }
  return data;
}

AuthorData AuthorsDB::authorByFileAs(QString file_as)
{
  return m_author_by_fileas.value(file_as);
}

AuthorData AuthorsDB::author(quint64 uid)
{
  return m_author_data.value(uid);
}

AuthorList AuthorsDB::authors()
{
  return m_author_data.values();
}

AuthorList AuthorsDB::authorsBySurname(QString surname)
{
  return m_author_by_surname.values(surname);
}

AuthorList AuthorsDB::authorsByForename(QString surname)
{
  return m_author_by_forename.values(surname);
}

bool AuthorsDB::loadAuthors()
{
  if (m_filename.isEmpty())
    return false;

  QFile file(m_filename);
  if (file.exists()) {
    YAML::Node authors_node = YAML::LoadFile(m_filename.toStdString());
    if (authors_node.IsMap()) {
      if (authors_node["authors"] && authors_node["authors"].IsSequence()) {
        YAML::Node authors = authors_node["authors"];
        for (std::size_t i = 0; i < authors.size(); i++) {
          YAML::Node author_node = authors[i];

          AuthorData author = AuthorData(new EBookAuthorData());
          author->uid = author_node["uid"].as<quint64>();
          author->surname = author_node["surname"].as<QString>();
          author->forename = author_node["forenames"].as<QString>();
          author->display_name = author_node["display name"].as<QString>();
          author->file_as = author_node["file as"].as<QString>();
          author->website = author_node["website"].as<QString>();
          author->wikipedia = author_node["wikipedia"].as<QString>();

          if (author->uid > m_highest_uid) {
            m_highest_uid = author->uid;
          }

          m_author_data.insert(author->uid, author);
          m_author_by_displayname.insert(author->display_name, author);
          m_author_by_forename.insert(author->forename, author);
          m_author_by_surname.insert(author->surname, author);
          m_author_by_fileas.insert(author->file_as, author);
        }
      }
      m_author_changed = true; // TODO change this after testing
    }
  }

  return false;
}

bool AuthorsDB::saveAuthors()
{
  if (m_filename.isEmpty())
    return false;

  QFile file(m_filename);
  if (m_author_changed) {
    if (file.open((QFile::ReadWrite | QFile::Truncate))) {
      YAML::Emitter emitter;
      emitter << YAML::Comment(
                QString("A YAML File is supposed to be user readable/editable but\n"
                        "you need to be careful when manually editing.\n"
                        "Remember that the uid numbers stand for unique identifier\n"
                        "so if you edit these MAKE SURE THAT THEY ARE UNIQUE. If\n"
                        "you repeat one the second will overwrite the first."));
      emitter << YAML::BeginMap; // authors only so far
      {
        emitter << YAML::Key << "authors";
        emitter << YAML::BeginSeq;
        {
          foreach (AuthorData author_data, m_author_data) {
            emitter << YAML::BeginMap;
            emitter << YAML::Key << "uid";
            emitter << YAML::Value << author_data->uid;
            emitter << YAML::Key << "surname";
            emitter << YAML::Value << author_data->surname;
            emitter << YAML::Key << "forenames";
            emitter << YAML::Value << author_data->forename;
            emitter << YAML::Key << "display name";
            emitter << YAML::Value << author_data->display_name;
            emitter << YAML::Key << "file as";
            emitter << YAML::Value << author_data->file_as;
            emitter << YAML::Key << "website";
            emitter << YAML::Value << author_data->website;
            emitter << YAML::Key << "wikipedia";
            emitter << YAML::Value << author_data->wikipedia;
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
  } // authors changed
  return false;
}

AuthorData AuthorsDB::addAuthor(QString display_name, FileAsList file_as_list)
{
  // TODO maybe this is not used.
  /* The display name is normally in the form 'FORENAME SURNAME' or
   * vicky-verky  in the case of asian names.
   */
  AuthorData data;
  QStringList splits = display_name.split(" ");
  if (splits.size() > 1) {
    if (splits.size() == 2) { // normal case unless middle names are supplied.
      if (m_author_by_surname.contains(splits.last())) {
        data = m_author_by_surname.value(splits.last());
        if (data->forename == splits.first()) {
          return AuthorData(nullptr);
        }
      }
    }
  }
  return data;
}

bool EBookAuthorData::operator==(const EBookAuthorData& rhs)
{
  if (uid == rhs.uid) {
    return true;
  }
  return false;
}

EBookAuthorData::Comparison EBookAuthorData::compare(QString forename,
    QString middlenames,
    QString surname)
{
  Comparison match = NO_MATCH;
  if (surname.toLower() == surname)
    match = SURNAME_MATCH;

  if (forename.toLower() == forename) {
    if (match == SURNAME_MATCH)
      match = FORE_AND_SURNAME_MATCH;
    else
      match = FORENAME_MATCH;
  }

  if (middlenames.toLower() == middlenames) {
    if (match == FORE_AND_SURNAME_MATCH)
      match = ALL_MATCH;
    else
      match = PARTIAL_MATCH;
  }

  return match;
}
