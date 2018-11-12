#include "qebookdocument_p.h"

QEBookDocumentPrivate::QEBookDocumentPrivate(QEBookDocument* parent)
  : q_ptr(parent)
{
  m_data = new EBookData();
}

QEBookDocumentPrivate::~QEBookDocumentPrivate() {}

EBookData*
QEBookDocumentPrivate::data()
{
  return m_data;
}

QString
QEBookDocumentPrivate::title()
{
  return m_data->title;
}

void
QEBookDocumentPrivate::setTitle(const QString& title)
{
  m_data->title = title;
}

QString
QEBookDocumentPrivate::subject()
{
  return m_data->subject;
}

void
QEBookDocumentPrivate::setSubject(const QString& subject)
{
  m_data->subject = subject;
}

QString
QEBookDocumentPrivate::language()
{
  return m_data->language;
}

void
QEBookDocumentPrivate::setLanguage(const QString& language)
{
  m_data->language = language;
}

QDateTime
QEBookDocumentPrivate::date()
{
  return m_data->date;
}

void
QEBookDocumentPrivate::setDate(const QDateTime& date)
{
  m_data->date = date;
}

QStringList
QEBookDocumentPrivate::authors()
{
  return m_data->authors;
}

void
QEBookDocumentPrivate::setAuthors(const QStringList& authors)
{
  m_data->authors = authors;
}

QString
QEBookDocumentPrivate::authorNames()
{
  return m_data->author_names;
}

QString
QEBookDocumentPrivate::publisher()
{
  return m_data->publisher;
}

void
QEBookDocumentPrivate::setPublisher(const QString& publisher)
{
  m_data->publisher = publisher;
}

bool
QEBookDocumentPrivate::isModified()
{
  return m_modified;
}

void
QEBookDocumentPrivate::setModified(bool modified)
{
  m_modified = modified;
}
bool
QEBookDocumentPrivate::readonly() const
{
  return m_readonly;
}
void
QEBookDocumentPrivate::setReadonly(bool readonly)
{
  m_readonly = readonly;
}

QString
QEBookDocumentPrivate::documentPath() const
{
  return m_documentPath;
}

void
QEBookDocumentPrivate::setDocumentPath(const QString& documentPath)
{
  m_documentPath = documentPath;
}
