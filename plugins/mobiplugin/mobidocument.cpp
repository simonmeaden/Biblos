#include "mobidocument.h"

#include <QThread>

#include <qlogger/qlogger.h>

#include "private/mobidocument_p.h"

MobiDocument::MobiDocument(QObject* parent) :
    ITextDocument(parent), d_ptr(new MobiDocumentPrivate(this))
{
}

MobiDocument::MobiDocument(MobiDocumentPrivate* doc, QObject* parent) :
    ITextDocument(parent), d_ptr(doc)
{
}

MobiDocument::MobiDocument(const MobiDocument& doc) :
    ITextDocument(doc.parent()), d_ptr(doc.d_ptr)
{
}

MobiDocument::~MobiDocument() {}

MobiDocument::MobiDocument(MobiDocumentPrivate& d) : ITextDocument(d.q_ptr) {}

QString MobiDocument::filename()
{
  Q_D(MobiDocument);
  return d->filename();
}

void MobiDocument::setFilename(const QString& filename)
{
  Q_D(MobiDocument);
  d->setFilename(filename);
}

void MobiDocument::openDocument(const QString& path)
{
  Q_D(MobiDocument);
  d->openDocument(path);
}

void MobiDocument::saveDocument()
{
  Q_D(MobiDocument);
  d->saveDocument();
}

IEBookInterface* MobiDocument::plugin()
{
  Q_D(MobiDocument);
  return d->plugin();
}

void MobiDocument::setPlugin(IEBookInterface* plugin)
{
  Q_D(MobiDocument);
  d->setPlugin(plugin);
}

bool MobiDocument::isModified()
{
  Q_D(MobiDocument);
  return d->isModified();
}

bool MobiDocument::readOnly()
{
  Q_D(MobiDocument);
  return d->readonly();
}

void MobiDocument::setReadOnly(const bool readonly)
{
  Q_D(MobiDocument);
  d->setReadonly(readonly);
}

QString MobiDocument::title()
{
  Q_D(MobiDocument);
  return d->title();
}

void MobiDocument::setTitle(const QString& title)
{
  Q_D(MobiDocument);
  d->setTitle(title);
}

QString MobiDocument::subject()
{
  Q_D(MobiDocument);
  return d->subject();
}

void MobiDocument::setSubject(const QString& subject)
{
  Q_D(MobiDocument);
  d->setSubject(subject);
}

QString MobiDocument::language() {}

void MobiDocument::setLanguage(const QString& language) {}

QDateTime MobiDocument::date() {}

void MobiDocument::setDate(const QDateTime& date) {}

QStringList MobiDocument::creators()
{
  Q_D(MobiDocument);
  return d->creators();
}

void MobiDocument::setCreators(const QStringList& creators)
{
  Q_D(MobiDocument);
  d->setCreators(creators);
}

QString MobiDocument::creatorNames()
{
  Q_D(MobiDocument);
  return d->creatorNames();
}

QString MobiDocument::publisher()
{
  Q_D(MobiDocument);
  return d->publisher();
}

void MobiDocument::setPublisher(const QString& publisher)
{
  Q_D(MobiDocument);
  return d->setPublisher(publisher);
}

QDate MobiDocument::published()
{
  Q_D(MobiDocument);
  return d->published();
}

void MobiDocument::setPublished(const QDate& published)
{
  Q_D(MobiDocument);
  return d->setPublished(published);
}
