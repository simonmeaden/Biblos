#ifndef LIBRARY_H
#define LIBRARY_H

#include <QMap>
#include <QObject>
#include <QSharedPointer>
#include <QList>

#include <list>

#include <qyaml-cpp/node.h>
#include <yaml-cpp/yaml.h>


//class Author;
//class Library : public QObject
//{
//  Q_OBJECT
//public:
//  explicit Library(QObject* parent = nullptr);

//  SharedAuthorList getAuthor(QString author);
//  SharedAuthorList getAuthorPartial(QString author);
//  SharedAuthor addAuthor(QString author);

//  QMultiMap<QString, SharedAuthor> *onName() const;
//  void setOnName(QMultiMap<QString, SharedAuthor> *on_name);
//  QMultiMap<QString, SharedAuthor> *onPartialName() const;
//  void setOnPartialName(QMultiMap<QString, SharedAuthor> *on_partial_name);
//  QMap<QString, SharedBookList> *books() const;
//  void setBooks(QMap<QString, SharedBookList> *books);




//signals:

//public slots:

//protected:
//  QMultiMap<QString, SharedAuthor>* m_on_name;
//  QMultiMap<QString, SharedAuthor>* m_on_partial_name;

//  QMap<QString, SharedBookList>* m_books;
//};
//typedef QSharedPointer<Library> SharedLibrary;


//=============================================================================
// YAML-CPP extensions for EBookEdit. These require the QYAML-CPP extensions
// to work as they support several common Qt classes.
//=============================================================================
//void
//operator>>(const YAML::Node& node, Book& book);

//void
//operator<<(YAML::Node node, const Book book);

//void
//operator>>(const YAML::Node& node, SharedBook& book);

//void
//operator<<(YAML::Node node, const SharedBook book);

//void
//operator>>(const YAML::Node& node, SharedBookList& books);

//void
//operator<<(YAML::Node node, const SharedBookList books);

//void
//operator>>(const YAML::Node& node, SharedAuthor& author);

//void
//operator<<(YAML::Node node, const SharedAuthor author);

//void
//operator>>(const YAML::Node& node, SharedLibrary& library);

//void
//operator<<(YAML::Node node, const SharedLibrary library);

//void
//operator>>(const YAML::Node& node, SharedAuthorList& library);

//void
//operator<<(YAML::Node node, const SharedAuthorList library);

#endif // LIBRARY_H
