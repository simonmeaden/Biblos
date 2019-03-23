#include "authors.h"

// starts at 1 - 0 == null value
quint64 EBookAuthorsDB::m_highest_uid = 1;

EBookAuthorsDB::EBookAuthorsDB(Options options)
  : m_options(options)
  , m_author_changed(false)
{
  loadAuthors();
}

EBookAuthorsDB::~EBookAuthorsDB()
{
  saveAuthors();
}

void
EBookAuthorsDB::setFilename(QString filename)
{
  m_filename = filename;
}

bool
EBookAuthorsDB::save()
{
  return saveAuthors();
}

bool
EBookAuthorsDB::load(QString filename)
{
  setFilename(filename);
  return loadAuthors();
}

quint64
EBookAuthorsDB::insertAuthor(AuthorData author_data)
{
  if (!author_data)
    return 0;

  if (author_data->uid() == 0) {
    author_data->setUid(nextUid());
  }
  m_author_data.insert(author_data->uid(), author_data);
  if (author_data->displayName().isEmpty()) {
    if (!author_data->isEmpty()) {
      // TODO handle asian type surname first format.
      QString display_name = author_data->forename();
      display_name += (display_name.isEmpty() ? "" : " " + display_name);
      display_name += " " + author_data->surname();
      author_data->setDisplayName(display_name);
    }
  } else {
    if (author_data->isEmpty()) {
      QStringList splits = author_data->displayName().split(" ");
      if (splits.size() == 1) {
        author_data->setSurname(splits.at(0));
      } else if (splits.size() == 2) {
        author_data->setForename(splits.at(0));
        author_data->setSurname(splits.at(1));
      } else {
        QString middle_names;
        author_data->setForename(splits.first());
        author_data->setSurname(splits.last());
        for (int i = 1; i < splits.size() - 1; i++) {
          if (i > 1)
            middle_names += " ";
          middle_names += splits.at(i);
        }
        author_data->setMiddlenames(middle_names);
      }
    }
  }

  if (author_data->fileAs().isEmpty() && !author_data->isEmpty()) {
    if (!author_data->surname().isEmpty() &&
        !author_data->forename().isEmpty()) {
      QString file_as = author_data->surname() + ", " +
                        author_data->forename() +
                        (author_data->middlenames().isEmpty()
                           ? ""
                           : " " + author_data->middlenames());
      author_data->setFile_as(file_as);
    }
  }

  addAuthor(author_data);
  return author_data->uid();
}

QStringList
EBookAuthorsDB::compareAndDiscard(QStringList names)
{
  QStringList cleaned;
  foreach (QString value, names) {
    QString lower = value.toLower();
    // check if the surname list has a match.
    AuthorList list = m_author_by_surname.values(value);
    foreach (AuthorData data, list) {
      if (lower == data->forename().toLower() ||
          lower == data->surname().toLower()) {
        if (!cleaned.contains(value))
          cleaned += value;
      }
    }
    // check if the forename list has a match.
    list = m_author_by_forename.values(value);
    foreach (AuthorData data, list) {
      if (lower == data->forename().toLower() ||
          lower == data->surname().toLower()) {
        if (!cleaned.contains(value))
          cleaned += value;
      }
    }
  }
  return cleaned;
}

bool
EBookAuthorsDB::removeBook(quint64 index)
{
  if (m_author_data.contains(index)) {
    AuthorData author = m_author_data.value(index);
    m_author_data.remove(index);
    m_author_by_displayname.remove(author->displayName(), author);
    m_author_by_fileas.remove(author->fileAs(), author);
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
AuthorData
EBookAuthorsDB::author(QString name)
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
        if (data->forename() == splits.first()) {
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

AuthorData
EBookAuthorsDB::authorByFileAs(QString file_as)
{
  return m_author_by_fileas.value(file_as);
}

AuthorData
EBookAuthorsDB::author(quint64 uid)
{
  return m_author_data.value(uid);
}

AuthorList
EBookAuthorsDB::authors()
{
  return m_author_data.values();
}

AuthorList
EBookAuthorsDB::authorsBySurname(QString surname)
{
  return m_author_by_surname.values(surname);
}

AuthorList
EBookAuthorsDB::authorsByForename(QString surname)
{
  return m_author_by_forename.values(surname);
}

bool
EBookAuthorsDB::loadAuthors()
{
  m_filename = m_options->authorsFile();

  QFile file(m_filename);
  if (file.exists()) {
    YAML::Node authors_map = YAML::LoadFile(m_filename.toStdString());
    if (authors_map && authors_map.IsMap()) {
      for (YAML::const_iterator it1 = authors_map.begin();
           it1 != authors_map.end();
           ++it1) {
        YAML::Node author_node = it1->second;
        AuthorData author = AuthorData(new EBookAuthorData());

        quint64 uid =
          it1->first.as<quint64>(); // author_node["uid"].as<quint64>();
        author->setUid(uid);
        author->setSurname(
          (author_node["surname"] ? author_node["surname"].as<QString>() : ""));
        author->setForename((author_node["forenames"]
                               ? author_node["forenames"].as<QString>()
                               : ""));
        author->setMiddlenames((author_node["middlenames"]
                                  ? author_node["middlenames"].as<QString>()
                                  : ""));
        author->setDisplayName((author_node["display name"]
                                  ? author_node["display name"].as<QString>()
                                  : ""));
        author->setFile_as(
          (author_node["file as"] ? author_node["file as"].as<QString>() : ""));
        author->setWebsite(
          (author_node["website"] ? author_node["website"].as<QString>() : ""));
        author->setWikipedia((author_node["wikipedia"]
                                ? author_node["wikipedia"].as<QString>()
                                : ""));
        author->setSurnameLast((author_node["surname last"]
                                  ? author_node["surname last"].as<bool>()
                                  : true));
        if (author_node["image"]) {
          QPixmap pixmap = author_node["image"].as<QPixmap>();
          if (!pixmap.isNull())
            author->setPixmap(pixmap);
        }
        QStringList words;
        YAML::Node word_list = author_node["word list"];
        if (!word_list.IsNull() && word_list.IsSequence()) {
          for (YAML::const_iterator it2 = word_list.begin();
               it2 != word_list.end();
               ++it2) {
            words << it2->second.as<QString>();
          }
        }
        if (author->uid() > m_highest_uid) {
          m_highest_uid = author->uid();
        }

        addAuthor(author);
      }
    }
    m_author_changed = false;
    return true;
  }

  return false;
}

bool
EBookAuthorsDB::saveAuthors()
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

      emitter << YAML::BeginMap;
      foreach (AuthorData author_data, m_author_data) {
        // for some reason emitter wont take the result directly from the
        // method. might need some work on the YAML files.
        quint64 uid = author_data->uid();
        QString surname = author_data->surname();
        QString forename = author_data->forename();
        QString middlenames = author_data->middlenames();
        QString display_name = author_data->displayName();
        QString file_as = author_data->fileAs();
        QString website = author_data->website();
        QString wikipedia = author_data->wikipedia();
        QPixmap pixmap = author_data->pixmap();
        QStringList words = author_data->wordList();

        emitter << YAML::Key << uid;
        emitter << YAML::Value;
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "surname";
        emitter << YAML::Value << surname;
        emitter << YAML::Key << "forenames";
        emitter << YAML::Value << forename;
        emitter << YAML::Key << "middlenames";
        emitter << YAML::Value << middlenames;
        emitter << YAML::Key << "display name";
        emitter << YAML::Value << display_name;
        emitter << YAML::Key << "file as";
        emitter << YAML::Value << file_as;
        emitter << YAML::Key << "surname last";
        emitter << YAML::Value << author_data->surnameLast();
        emitter << YAML::Key << "website";
        emitter << YAML::Value << website;
        emitter << YAML::Key << "wikipedia";
        emitter << YAML::Value << wikipedia;
        if (!author_data->pixmap().isNull()) {
          emitter << YAML::Key << "image";
          emitter << YAML::Value << pixmap;
        }
        emitter << YAML::Key << "word list";
        emitter << YAML::BeginSeq;
        for (int i = 0; i < words.size(); i++) {
          QString word = words.at(i);
          emitter << word;
        }
        emitter << YAML::EndSeq;

        emitter << YAML::EndMap;
      }
      emitter << YAML::EndMap;
      QTextStream out(&file);
      out << emitter.c_str();
      return true;
    }
  } // authors changed
  return false;
}

AuthorData
EBookAuthorsDB::addAuthor(QString display_name, FileAsList /*file_as_list*/)
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
        if (data->forename() == splits.first()) {
          return AuthorData(nullptr);
        }
      }
    }
  }
  return data;
}

void
EBookAuthorsDB::addAuthor(AuthorData author_data)
{
  if (author_data->isValid()) {
    if (!m_author_data.contains(author_data->uid())) {
      m_author_data.insert(author_data->uid(), author_data);
      if (!author_data->surname().isEmpty())
        m_author_by_surname.insert(author_data->surname().toLower(),
                                   author_data);
      if (!author_data->forename().isEmpty())
        m_author_by_forename.insert(author_data->forename().toLower(),
                                    author_data);
      if (!author_data->displayName().isEmpty())
        m_author_by_displayname.insert(author_data->displayName(), author_data);
      if (!author_data->fileAs().isEmpty())
        m_author_by_fileas.insert(author_data->fileAs().toLower(), author_data);
    }
  }
}

QString
EBookAuthorData::forename() const
{
  return m_forename;
}

void
EBookAuthorData::setForename(const QString& value)
{
  m_modified = true;
  m_forename = value;
}

QString
EBookAuthorData::middlenames() const
{
  return m_middlenames;
}

void
EBookAuthorData::setMiddlenames(const QString& value)
{
  m_modified = true;
  m_middlenames = value;
}

QString
EBookAuthorData::surname() const
{
  return m_surname;
}

void
EBookAuthorData::setSurname(const QString& surname)
{
  m_modified = true;
  m_surname = surname;
}

QString
EBookAuthorData::fileAs() const
{
  return m_file_as;
}

void
EBookAuthorData::setFile_as(const QString& file_as)
{
  m_modified = true;
  m_file_as = file_as;
}

quint64
EBookAuthorData::uid() const
{
  return m_uid;
}

void
EBookAuthorData::setUid(const quint64& uid)
{
  m_uid = uid;
}

QString
EBookAuthorData::displayName() const
{
  return m_display_name;
}

void
EBookAuthorData::setDisplayName(const QString& display_name)
{
  m_modified = true;
  m_display_name = display_name;
}

QPixmap
EBookAuthorData::pixmap() const
{
  return m_pixmap;
}

void
EBookAuthorData::setPixmap(const QPixmap& pixmap)
{
  m_modified = true;
  m_pixmap = pixmap;
}

bool
EBookAuthorData::surnameLast() const
{
  return m_surname_last;
}

void
EBookAuthorData::setSurnameLast(bool surname_last)
{
  m_modified = true;
  m_surname_last = surname_last;
}

QStringList
EBookAuthorData::wordList() const
{
  return m_word_list;
}

void
EBookAuthorData::setWordList(const QStringList& word_list)
{
  m_word_list = word_list;
}

QList<quint64>
EBookAuthorData::books() const
{
  return m_books;
}

void
EBookAuthorData::setBooks(const QList<quint64>& books)
{
  m_modified = true;
  m_books = books;
}

QString
EBookAuthorData::wikipedia() const
{
  return m_wikipedia;
}

void
EBookAuthorData::setWikipedia(const QString& wikipedia)
{
  m_modified = true;
  m_wikipedia = wikipedia;
}

QString
EBookAuthorData::website() const
{
  return m_website;
}

void
EBookAuthorData::setWebsite(const QString& website)
{
  m_modified = true;
  m_website = website;
}

EBookAuthorData::EBookAuthorData()
  : m_modified(false)
  , m_uid(0)
  , m_surname_last(true)
{}

EBookAuthorData::EBookAuthorData(const EBookAuthorData& other)
{
  m_modified = other.m_modified;
  m_uid = other.m_uid;
  m_display_name = other.m_display_name;
  m_surname = other.m_surname;
  m_forename = other.m_forename;
  m_middlenames = other.m_middlenames;
  m_surname_last = other.m_surname_last;
  m_file_as = other.m_file_as;
  m_website = other.m_website;
  m_wikipedia = other.m_wikipedia;
  m_pixmap = other.m_pixmap;
}

EBookAuthorData::~EBookAuthorData() {}

bool
EBookAuthorData::isValid()
{
  return (m_uid > 0);
}

bool
EBookAuthorData::isEmpty()
{
  return (m_surname.isEmpty() && m_forename.isEmpty() &&
          m_middlenames.isEmpty());
}

bool
EBookAuthorData::isModified()
{
  return m_modified;
}

EBookAuthorData::Comparison
EBookAuthorData::compare(QString forename, QString middlenames, QString surname)
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
