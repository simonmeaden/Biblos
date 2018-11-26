#include "dbmanager.h"

using namespace qlogger;

const QString DbManager::CREATE_DATABASE =
  "CREATE TABLE IF NOT EXISTS book ("
  "  id UNSIGNED INTEGER PRIMARY KEY,"
  "  title VARCHAR(100) NOT NULL,"
  "  isbn VARCHAR(20)"
  ");"
  "CREATE TABLE IF NOT EXISTS author ("
  "  id UNSIGNED INTEGER PRIMARY KEY,"
  "  forename VARCHAR(12) NOT NULL,"
  "  midnames VARCHAR(30),"
  "  surname VARCHAR(12) NOT NULL"
  ");"
  "CREATE TABLE IF NOT EXISTS books ("
  "  author UNSIGNED INTEGER,"
  "  book UNSIGNED INTEGER,"
  "  FOREIGN KEY(book) REFERENCES book(id),"
  "  FOREIGN KEY(author) REFERENCES author(id)"
  ");";
const QString DbManager::GET_BOOK_ON_NAME = "SELECT * FROM author "
                                            "WHERE title='%1' ";
const QString DbManager::INSERT_INTO_BOOK = "INSERT INTO book (id, title, isbn)"
                                            "VALUES (%1, '%2', '%3')";
const QString DbManager::UPDATE_BOOK = "UPDATE book SET "
                                       "title='%1', isbn='%2' "
                                       "WHERE id=%3";

const QString DbManager::GET_AUTHOR_ON_SURNAME = "SELECT * FROM author "
                                                 "WHERE surname='%1'";
const QString DbManager::GET_AUTHOR_ON_FORE_AND_SURNAME =
  "SELECT * FROM author "
  "WHERE surname='%1' AND forename='%2'";
const QString DbManager::GET_AUTHOR_ON_ALL_NAMES = "SELECT * FROM author "
                                                   "WHERE surname='%1' "
                                                   "AND forename='%2' "
                                                   "AND midnames='%3'";
const QString DbManager::GET_AUTHOR = "SELECT * FROM author";
const QString DbManager::INSERT_INTO_AUTHOR =
  "INSERT INTO author (forename, midnames, surname)"
  "VALUES ('%1', '%2', '%3')";
const QString DbManager::UPDATE_AUTHOR =
  "UPDATE author SET "
  "forename='%1', midnames='%2', surname='%3' "
  "WHERE id=%4";

DbManager::DbManager(QString path, QObject* parent)
  : QObject(parent)
{
  m_db = QSqlDatabase::addDatabase("QSQLITE");
  m_db.setDatabaseName(path);

  if (!m_db.open()) {
    QLOG_DEBUG("Error: connection with database fail")
  } else {
    QLOG_DEBUG("Database: connection ok")
  }

  executeSqlString(CREATE_DATABASE);
}

/*! Parses a sql script string.

   Splits individual SQL statements on semicolons. Handles
   semicolons within single quotes, double quotes and comments.
   It treats both ' and ` as the same character (start/end single quoted
   string) as I have seen `string' used in sql files sometimes. By default tab
   and newline/carriage return characters are removed as these sometimes cause
   problems in some databases. You can specify that these are retained by
   setting removeNewlines and/or removeTabs to false. Please note that it does
   not check for correct SQL, that is down to you.

    Extended from a comment by Numbat in Qt Center.

    \param fileName as a QString.
    \param removeNewLines boolean value, if true then all '\n' and '\r'
   characters are removed otherwise they are left in. \param removeTabs boolean
   value, if true then all '\t' characters are removed otherwise they are left
   in. \return an integer error value or 0 if no error.
*/
QSqlQuery
DbManager::executeSqlString(const QString& data,
                            bool removeNewline,
                            bool removeTabs)
{
  return cleanSql(data, removeNewline, removeTabs);
}

/*! Parses a sql script file.

   Splits individual SQL statements on semicolons. Handles
   semicolons within single quotes, double quotes and comments.
   It treats both ' and ` as the same character (start/end single quoted
   string) as I have seen `string' used in sql files sometimes. By default tab
   and newline/carriage return characters are removed as these sometimes cause
   problems in some databases. You can specify that these are retained by
   setting removeNewlines and/or removeTabs to false. Please note that it does
   not check for correct SQL, that is down to you.

    Extended from a comment by Numbat in Qt Center.

    \param fileName as a QString.
    \param removeNewLines boolean value, if true then all '\n' and '\r'
   characters are removed otherwise they are left in. \param removeTabs boolean
   value, if true then all '\t' characters are removed otherwise they are left
   in. \return an integer error value or 0 if no error.
*/
QSqlQuery
DbManager::executeSqlScriptFile(const QString& fileName,
                                bool removeNewlines,
                                bool removeTabs)
{
  QFile file(fileName);

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    return QSqlQuery();
  }

  QTextStream in(&file);
  QString sqldata = in.readAll();

  return cleanSql(sqldata, removeNewlines, removeTabs);
}

SharedBook
DbManager::book(QString title)
{
  // TODO
  return SharedBook(Q_NULLPTR);
}

SharedAuthorList
DbManager::author(QString surname, QString forename, QString midnames)
{
  SharedAuthorList list;
  QString sql;
  QSqlQuery qry;
  if (surname.isEmpty()) {
    sql = GET_AUTHOR;
  } else {
    if (forename.isEmpty()) {
      sql = GET_AUTHOR_ON_SURNAME.arg(surname);
    } else {
      if (midnames.isEmpty()) {
        sql = GET_AUTHOR_ON_FORE_AND_SURNAME.arg(surname).arg(forename);
      } else {
        sql = GET_AUTHOR_ON_ALL_NAMES.arg(surname).arg(forename).arg(midnames);
      }
    }
  }
  qry = executeSqlString(sql);
  while (qry.next()) {
    QString forename = qry.value("forename").toString();
    QString midnames = qry.value("midnames").toString();
    QString surname = qry.value("surname").toString();
    SharedAuthor author(new Author());
    author->setForename(forename);
    author->setMiddlenames(midnames);
    author->setSurname(surname);
    if (!list.contains(author)) {
      list.append(author);
    }
  }

  return list;
}

bool
DbManager::saveAuthor(SharedAuthor author)
{
  QString sql = INSERT_INTO_AUTHOR.arg(
    author->forename(), author->middlenames(), author->surname());
  QSqlQuery qry = executeSqlString(sql);
  if (qry.isActive()) {
    return true;
  }
  return false;
}

QSqlQuery
DbManager::cleanSql(const QString& sqldata,
                    bool removeNewlines,
                    bool removeTabs)
{

  if (sqldata.length() == 0) {
    return QSqlQuery();
  }

  QString data = sqldata;

  QList<int> splitPoints;
  enum
  {
    IN_SINGLEQUOTE_STRING,   // Inside a single-quoted string
    IN_DOUBLEQUOTE_STRING,   // Inside a double-quoted string.
    IN_ESCAPE,               // Detect an escaped character
    START_MULTILINE_COMMENT, // Start Multi-line comments. /*  */
    START_INLINE_COMMENT,  // Start -- inline comments. detects possible first -
                           // character.
    END_MULTILINE_COMMENT, // End multi-line comments. detects second *
                           // character.
    IN_MULTILINE_COMMENT,  // Inside a multi-line comment.
    IN_INLINE_COMMENT,     // # or -- style inline comments
    NORMAL,                // Normal state - any normal character.
  } state = NORMAL;

  int successCount = 0;

  if (removeTabs || removeNewlines) {
    QString s;

    for (int i = 0; i < data.length(); i++) {
      const int character = data.at(i).unicode();

      if ((character == '\n' || character == '\r') && removeNewlines) {
        continue;

      } else if (character == '\t' && removeTabs) {
        continue;

      } else {
        s += char(character);
      }
    }

    data = s;
  }

  for (int i = 0; i < data.length(); i++) {
    const int character = data.at(i).unicode();

    switch (state) {

      case IN_SINGLEQUOTE_STRING: // Inside a single-quoted string
        switch (character) {
          case '`':
          case '\'': // end of single-quoted string
            state = NORMAL;
            break;

          case '\\': // escaped character in single-quoted string
            state = IN_ESCAPE;
            break;
        }

        break;

      case IN_DOUBLEQUOTE_STRING: // Inside a single-quoted string
        switch (character) {
          case '\"': // end of single-quoted string
            state = NORMAL;
            break;

          case '\\': // escaped character in single-quoted string
            state = IN_ESCAPE;
            break;
        }

        break;

      case START_MULTILINE_COMMENT:
        switch (character) {
          case '*': // second character in multi-line comment
            state = IN_MULTILINE_COMMENT;
            break;

          default
            : // haven't received second character of inline/multi-line comment
            state = NORMAL; // Possibly throw error/exception
            break;
        }

        break;

      case START_INLINE_COMMENT: // Start of single-quote comment
        switch (character) {
          case '-': // second character in -- inline comment
            state = IN_INLINE_COMMENT;
            break;

          default
            : // haven't received second character of inline/multi-line comment
            state = NORMAL; // Possibly throw error/exception
            break;
        }

        break;

      case IN_MULTILINE_COMMENT:
        switch (character) {
          case '*':
            state = END_MULTILINE_COMMENT;
            break;
        }

        break;

      case END_MULTILINE_COMMENT:
        switch (character) {
          case '/':
            state = NORMAL;
            break;

          default
            : // haven't received second character of inline/multi-line comment
            state = NORMAL; // Possibly throw error/exception
            break;
        }

        break;

      case IN_INLINE_COMMENT:
        switch (character) {
          case '\n':
            state = NORMAL;
            break;
        }

        break;

      case IN_ESCAPE: // Detect an escaped character
        state = IN_SINGLEQUOTE_STRING;
        break;

      case NORMAL: {
        switch (character) {
          case ';': // Inside a single-quoted string
            splitPoints.push_back(i);
            break;

          case '`':
          case '\'': // Inside a single-quoted string
            state = IN_SINGLEQUOTE_STRING;
            break;

          case '\"': // Inside a double-quoted string
            state = IN_DOUBLEQUOTE_STRING;
            break;

          case '/': // Start Multi-line comments. /*  */
            state = START_MULTILINE_COMMENT;
            break;

          case '-': // inside inline comments
            state = START_INLINE_COMMENT;
            break;

          case '#': // inside inline comments
            state = IN_INLINE_COMMENT;
            break;
        }

        break;
      }
    }
  }

  splitPoints.push_back(data.length() - 1);

  QSqlQuery query;
  for (int i = 0, j = 0; i < splitPoints.length(); i++) {
    QString statement = data.mid(j, splitPoints.at(i) - j + 1);
    j = splitPoints.at(i) + 1;

    if (statement.trimmed().length() > 0) {
      query = QSqlQuery(m_db);

      if (query.exec(statement)) {
        successCount++;
      }

      else {
        QLOG_TRACE(
          QString("Database::parseSqlScriptFile() failed : %1 \nReason : %2")
            .arg(statement)
            .arg(query.lastError().text()))
      }
    }
  }

  return query;
}
