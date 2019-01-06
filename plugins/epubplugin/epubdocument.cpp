#include "epubdocument.h"

#include <qlogger/qlogger.h>

#include "private/epubdocument_p.h"

using namespace qlogger;

EPubDocument::EPubDocument(QObject* parent)
  : ITextDocument(parent)
  , d_ptr(new EPubDocumentPrivate(this))
{
  setUndoRedoEnabled(false);
}

EPubDocument::EPubDocument(EPubDocumentPrivate* doc, QObject* parent)
  : ITextDocument(parent)
  , d_ptr(doc)
{
  setUndoRedoEnabled(false);
}

EPubDocument::EPubDocument(const EPubDocument& doc)
  : ITextDocument(doc.parent())
  , d_ptr(doc.d_ptr)
{
}

EPubDocument::EPubDocument(EPubDocumentPrivate& d)
  : ITextDocument(d.q_ptr)
{
}

EPubDocument::~EPubDocument()
{
}

bool EPubDocument::loaded()
{
  Q_D(EPubDocument);
  return d->loaded();
}

void EPubDocument::openDocument(const QString& path)
{
  Q_D(EPubDocument);
  d->openDocument(path);
}

void EPubDocument::saveDocument()
{
  Q_D(EPubDocument);
  d->saveDocument();
}

EPubContents* EPubDocument::cloneData()
{
  Q_D(EPubDocument);
  return d->cloneData();
}

void EPubDocument::setClonedData(EPubContents* cloneData)
{
  Q_D(EPubDocument);
  d->setClonedData(cloneData);
}

QString EPubDocument::filename()
{
  Q_D(EPubDocument);
  return d->filename();
}

void EPubDocument::setFilename(const QString& filename)
{
  Q_D(EPubDocument);
  d->setFilename(filename);
}

QTextDocument* EPubDocument::toc()
{
  Q_D(EPubDocument);
  return d->toc();
}

QString EPubDocument::title()
{
  Q_D(EPubDocument);
  return d->title();
}

// void EPubDocument::setTitle(const QString& title)
//{
//  Q_D(EPubDocument);
//  d->setTitle(title);
//}

QString EPubDocument::subject()
{
  Q_D(EPubDocument);
  return d->subject();
}

void EPubDocument::setSubject(const QString& subject)
{
  Q_D(EPubDocument);
  d->setSubject(subject);
}

QString EPubDocument::language()
{
  Q_D(EPubDocument);
  // TODO
}

void EPubDocument::setLanguage(const QString& language)
{
  Q_D(EPubDocument);
  // TODO
}

QDateTime EPubDocument::date()
{
  Q_D(EPubDocument);
  // TODO
}

void EPubDocument::setDate(const QDateTime& date)
{
  Q_D(EPubDocument);
  // TODO
}

QStringList EPubDocument::creators()
{
  Q_D(EPubDocument);
  return d->creators();
}

// void EPubDocument::setCreators(const QStringList& authors)
//{
//  Q_D(EPubDocument);
//  d->addCreators(authors);
//}

QString EPubDocument::creatorNames(const QStringList &authors)
{
  Q_D(EPubDocument);
  if (authors.isEmpty())
    return d->creatorNames(d->creators());
  else {
    return d->creatorNames(authors);
  }
}

QString EPubDocument::publisher()
{
  Q_D(EPubDocument);
  return d->publisher();
}

void EPubDocument::setPublisher(const QString& publisher)
{
  Q_D(EPubDocument);
  d->setPublisher(publisher);
}

// void EPubDocument::clearCache()
//{
//  Q_D(EPubDocument);
//  d->clearCache();
//}

IEBookInterface* EPubDocument::plugin()
{
  Q_D(EPubDocument);
  return d->plugin();
}

void EPubDocument::setPlugin(IEBookInterface* plugin)
{
  Q_D(EPubDocument);
  d->setPlugin(plugin);
}

// bool EPubDocument::isModified()
//{
//  Q_D(EPubDocument);
//  return d->isModified();
//}

// bool EPubDocument::readOnly()
//{
//  Q_D(EPubDocument);
//  return d->readonly();
//}

// void EPubDocument::setReadOnly(const bool readonly)
//{
//  Q_D(EPubDocument);
//  d->setReadonly(readonly);
//}

QDate EPubDocument::published()
{
  Q_D(EPubDocument);
  return d->published();
}

void EPubDocument::setPublished(const QDate& published)
{
  Q_D(EPubDocument);
  return d->setPublished(published);
}
