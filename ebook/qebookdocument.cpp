#include "qebookdocument.h"

QEBookDocument::QEBookDocument(QEBookDocumentPrivate* doc, QObject* parent)
  : QTextDocument(parent)
  , d_ptr(doc)
{}

QEBookDocument::QEBookDocument(QObject* parent)
  : QTextDocument(parent)
  , d_ptr(new QEBookDocumentPrivate(this))
{}

QEBookDocument::QEBookDocument(const QEBookDocument &doc)
  : QTextDocument(doc.parent())
  , d_ptr(doc.d_ptr)
{}

QEBookDocument::QEBookDocument(QEBookDocumentPrivate& d)
  : d_ptr(&d)
{}

QEBookDocument::~QEBookDocument() {}

bool
QEBookDocument::isModified()
{
  return d_ptr->isModified();
}

void
QEBookDocument::setModified(bool modified)
{
  d_ptr->setModified(modified);
}

QString
QEBookDocument::documentPath() const
{
  return d_ptr->documentPath();
}

void
QEBookDocument::setDocumentPath(const QString& path)
{
  d_ptr->setDocumentPath(path);
}

bool
QEBookDocument::readonly() const
{
  return d_ptr->readonly();
}

void
QEBookDocument::setReadonly(bool readonly)
{
  d_ptr->setReadonly(readonly);
}

EBookData*
QEBookDocument::data()
{
  return d_ptr->data();
}

QString
QEBookDocument::title() const
{
  return d_ptr->title();
}

void
QEBookDocument::setTitle(const QString& title)
{
  Q_D(QEBookDocument);
  d->setTitle(title);
}

QString
QEBookDocument::subject() const
{
  return d_ptr->subject();
}

void
QEBookDocument::setSubject(const QString& subject)
{
  Q_D(QEBookDocument);
  d->setSubject(subject);
}

QString
QEBookDocument::language() const
{
  return d_ptr->language();
}

void
QEBookDocument::setLanguage(const QString& language)
{
  Q_D(QEBookDocument);
  d->setLanguage(language);
}

QDateTime
QEBookDocument::date() const
{
  return d_ptr->date();
}

void
QEBookDocument::setDate(const QDateTime& date)
{
  Q_D(QEBookDocument);
  d->setDate(date);
}

QStringList
QEBookDocument::authors() const
{
  return d_ptr->authors();
}

void
QEBookDocument::setAuthors(const QStringList& authors)
{
  Q_D(QEBookDocument);
  d->setAuthors(authors);
}

QString
QEBookDocument::authorNames() const
{
  return d_ptr->authorNames();
}

QString
QEBookDocument::publisher() const
{
  return d_ptr->publisher();
}

void
QEBookDocument::setPublisher(const QString& publisher)
{
  Q_D(QEBookDocument);
  d->setPublisher(publisher);
}
