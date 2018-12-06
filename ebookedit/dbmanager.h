#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QFile>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include <qlogger/qlogger.h>

#include "ebookcommon.h"

class DbManager : public QObject
{
  Q_OBJECT
public:
  explicit DbManager(QString path, QObject* parent = Q_NULLPTR);

  QSqlQuery executeSqlString(const QString& data,
                             bool removeNewline = true,
                             bool removeTabs = true);
  QSqlQuery executeSqlScriptFile(const QString&, bool = true, bool = true);

  SharedBook book(QString title);
  SharedAuthorList author(QString surname = QString(), QString forename = QString(), QString midnames = QString());
  bool saveAuthor(SharedAuthor author);

signals:

public slots:

protected:
  QSqlDatabase m_db;

  void createTables();
  QSqlQuery cleanSql(const QString& sqldata,
                     bool removeNewlines,
                     bool removeTabs);

private:
  static const QString CREATE_DATABASE;
  static const QString GET_BOOK_ON_NAME;
  static const QString INSERT_INTO_BOOK;
  static const QString UPDATE_BOOK;

  static const QString GET_AUTHOR;
  static const QString GET_AUTHOR_ON_SURNAME;
  static const QString GET_AUTHOR_ON_FORE_AND_SURNAME;
  static const QString GET_AUTHOR_ON_ALL_NAMES;
  static const QString INSERT_INTO_AUTHOR;
  static const QString UPDATE_AUTHOR;
};

#endif // DBMANAGER_H
