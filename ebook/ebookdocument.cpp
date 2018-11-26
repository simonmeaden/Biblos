#include "ebookdocument.h"

EBookDocument::EBookDocument(EBookDocumentPrivate *doc, QObject* parent)
  : QTextDocument(parent)
  , d_ptr(doc)
{}

EBookDocument::EBookDocument(QObject* parent)
  : QTextDocument(parent)
  , d_ptr(new EBookDocumentPrivate(this))
{}

EBookDocument::EBookDocument(const EBookDocument &doc)
  : QTextDocument(doc.parent())
  , d_ptr(doc.d_ptr)
{}

EBookDocument::EBookDocument(EBookDocumentPrivate& d)
  : d_ptr(&d)
{}

EBookDocument::~EBookDocument() {}

bool
EBookDocument::isModified()
{
  return d_ptr->isModified();
}

void
EBookDocument::setModified(bool modified)
{
  d_ptr->setModified(modified);
}

QString
EBookDocument::documentPath() const
{
  return d_ptr->documentPath();
}

void
EBookDocument::setDocumentPath(const QString& path)
{
  d_ptr->setDocumentPath(path);
}

bool
EBookDocument::readonly() const
{
  return d_ptr->readonly();
}

void
EBookDocument::setReadonly(bool readonly)
{
  d_ptr->setReadonly(readonly);
}

EBookData*
EBookDocument::data()
{
  return d_ptr->data();
}

QString
EBookDocument::title() const
{
  return d_ptr->title();
}

void
EBookDocument::setTitle(const QString& title)
{
  Q_D(EBookDocument);
  d->setTitle(title);
}

QString
EBookDocument::subject() const
{
  return d_ptr->subject();
}

void
EBookDocument::setSubject(const QString& subject)
{
  Q_D(EBookDocument);
  d->setSubject(subject);
}

QString
EBookDocument::language() const
{
  return d_ptr->language();
}

void
EBookDocument::setLanguage(const QString& language)
{
  Q_D(EBookDocument);
  d->setLanguage(language);
}

QDateTime
EBookDocument::date() const
{
  return d_ptr->date();
}

void
EBookDocument::setDate(const QDateTime& date)
{
  Q_D(EBookDocument);
  d->setDate(date);
}

QStringList
EBookDocument::authors() const
{
  return d_ptr->authors();
}

void
EBookDocument::setAuthors(const QStringList& authors)
{
  Q_D(EBookDocument);
  d->setAuthors(authors);
}

QString
EBookDocument::authorNames() const
{
  return d_ptr->authorNames();
}

QString
EBookDocument::publisher() const
{
  return d_ptr->publisher();
}

void
EBookDocument::setPublisher(const QString& publisher)
{
  Q_D(EBookDocument);
  d->setPublisher(publisher);
}
