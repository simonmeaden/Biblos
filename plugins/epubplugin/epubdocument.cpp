#include "epubdocument.h"

#include <QDebug>
#include <QDir>
#include <QDomDocument>
#include <QElapsedTimer>
#include <QIODevice>
#include <QPainter>
#include <QSvgRenderer>
#include <QTextBlock>
#include <QTextCursor>
#include <QTextDocument>
#include <QThread>

#include <qlogger/qlogger.h>

#include "private/epubdocument_p.h"

using namespace qlogger;

#ifdef DEBUG_CSS
#include <private/qcssparser_p.h>
#endif

EPubDocument::EPubDocument(QObject* parent) : ITextDocument(parent), d_ptr(new EPubDocumentPrivate(this))
{
  setUndoRedoEnabled(false);
}

EPubDocument::EPubDocument(EPubDocumentPrivate* doc, QObject* parent) : ITextDocument(parent), d_ptr(doc)
{
  setUndoRedoEnabled(false);
}

EPubDocument::EPubDocument(const EPubDocument& doc) : ITextDocument(doc.parent()), d_ptr(doc.d_ptr) {}

EPubDocument::EPubDocument(EPubDocumentPrivate& d) : ITextDocument(d.q_ptr) {}

EPubDocument::~EPubDocument() {}

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

QString EPubDocument::filename() const { return m_filename; }

void EPubDocument::setFilename(const QString& filename) { m_filename = filename; }

IEBookInterface* EPubDocument::plugin() const { return m_plugin; }

void EPubDocument::setPlugin(IEBookInterface* plugin) { m_plugin = plugin; }

bool EPubDocument::isModified() const { return m_modified; }

bool EPubDocument::readOnly() const { return m_readonly; }

void EPubDocument::setReadOnly(const bool readonly) { m_readonly = readonly; }

QString EPubDocument::title()
{
  Q_D(EPubDocument);
  return d->title();
}

void EPubDocument::setTitle(const QString& title)
{
  Q_D(EPubDocument);
  d->setTitle(title);
}

QString EPubDocument::subject()
{
  Q_D(EPubDocument);
  // TODO
}

void EPubDocument::setSubject(const QString& subject)
{
  Q_D(EPubDocument);
  // TODO
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

QStringList EPubDocument::authors()
{
  Q_D(EPubDocument);
  return d->authors();
}

void EPubDocument::setAuthors(const QStringList& authors)
{
  Q_D(EPubDocument);
  d->insertAuthors(authors);
}

QString EPubDocument::authorNames() { Q_D(EPubDocument); }

QString EPubDocument::publisher()
{
  Q_D(EPubDocument);
  // TODO
}

void EPubDocument::setPublisher(const QString& publisher)
{
  Q_D(EPubDocument);
  // TODO
}

void EPubDocument::clearCache()
{
  Q_D(EPubDocument);
  d->clearCache();
}
