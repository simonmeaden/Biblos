#include "ebookdocument_p.h"

EBookDocumentPrivate::EBookDocumentPrivate(EBookDocument* parent)
  : q_ptr(parent)
  , m_modified(false)
{
  m_data = new EBookData();
}

EBookDocumentPrivate::~EBookDocumentPrivate() {}

EBookData*
EBookDocumentPrivate::data()
{
  return m_data;
}

QString
EBookDocumentPrivate::title()
{
  return m_data->title;
}

void
EBookDocumentPrivate::setTitle(const QString& title)
{
  m_data->title = title;
}

QString
EBookDocumentPrivate::subject()
{
  return m_data->subject;
}

void
EBookDocumentPrivate::setSubject(const QString& subject)
{
  m_data->subject = subject;
}

QString
EBookDocumentPrivate::language()
{
  return m_data->language;
}

void
EBookDocumentPrivate::setLanguage(const QString& language)
{
  m_data->language = language;
}

QDateTime
EBookDocumentPrivate::date()
{
  return m_data->date;
}

void
EBookDocumentPrivate::setDate(const QDateTime& date)
{
  m_data->date = date;
}

QStringList
EBookDocumentPrivate::authors()
{
  return m_data->authors;
}

void
EBookDocumentPrivate::setAuthors(const QStringList& authors)
{
  m_data->authors = authors;
}

QString
EBookDocumentPrivate::authorNames()
{
  return m_data->author_names;
}

QString
EBookDocumentPrivate::publisher()
{
  return m_data->publisher;
}

void
EBookDocumentPrivate::setPublisher(const QString& publisher)
{
  m_data->publisher = publisher;
}

bool
EBookDocumentPrivate::isModified()
{
  return m_modified;
}

void
EBookDocumentPrivate::setModified(bool modified)
{
  m_modified = modified;
}

bool
EBookDocumentPrivate::readonly() const
{
  return m_readonly;
}

void
EBookDocumentPrivate::setReadonly(bool readonly)
{
  m_readonly = readonly;
}

QString
EBookDocumentPrivate::documentPath() const
{
  return m_documentPath;
}

void
EBookDocumentPrivate::setDocumentPath(const QString& documentPath)
{
  m_documentPath = documentPath;
}
