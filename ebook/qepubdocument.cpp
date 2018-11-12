#include "qepubdocument.h"

#include <QDebug>
#include <QDir>
#include <QDomDocument>
#include <QElapsedTimer>
#include <QIODevice>
#include <QPainter>
#include <QSvgRenderer>
#include <QTextBlock>
#include <QTextCursor>
#include <QThread>

#include "private/qepubdocument_p.h"
#include <qlogger.h>

using namespace qlogger;

#ifdef DEBUG_CSS
#include <private/qcssparser_p.h>
#endif

EPubDocument::EPubDocument(QObject* parent)
  : QEBookDocument(new EPubDocumentPrivate(this), parent)
{
  setUndoRedoEnabled(false);
}

EPubDocument::EPubDocument(EPubDocumentPrivate* doc, QObject* parent)
  : QEBookDocument(doc, parent)
{
  setUndoRedoEnabled(false);
}

EPubDocument::EPubDocument(const EPubDocument& doc)
  : QEBookDocument(doc)
{}

EPubDocument::EPubDocument(EPubDocumentPrivate& d)
  : QEBookDocument(d)
{}

EPubDocument::~EPubDocument() {}

bool
EPubDocument::loaded()
{
  Q_D(EPubDocument);
  return d->loaded();
}

void
EPubDocument::openDocument(const QString& path)
{
  Q_D(EPubDocument);
  d->openDocument(path);
}

EBookContents *EPubDocument::cloneData()
{
  Q_D(EPubDocument);
  return d->cloneData();
}

void
EPubDocument::setClonedData(EBookContents* cloneData)
{
  Q_D(EPubDocument);
  d->setClonedData(cloneData);
}

void
EPubDocument::clearCache()
{
  Q_D(EPubDocument);
  d->clearCache();
}
