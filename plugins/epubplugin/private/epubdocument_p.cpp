#include "epubdocument_p.h"

#include <csvsplitter/csvsplitter.h>
#include <qlogger/qlogger.h>
using namespace qlogger;

EPubDocumentPrivate::EPubDocumentPrivate(EPubDocument* parent) :
  q_ptr(parent), m_container(nullptr), m_loaded(false)
{
}

EPubDocumentPrivate::~EPubDocumentPrivate() {}

bool EPubDocumentPrivate::loaded()
{
  return m_loaded;
}

void EPubDocumentPrivate::openDocument(const QString& path)
{
  //  Q_Q(EPubDocument);
  setDocumentPath(path);
  loadDocument();

  //    m_data->toc = m_container->toc();
}

void EPubDocumentPrivate::saveDocument()
{
  m_container->saveFile();
}

// void EPubDocumentPrivate::clearCache() { /*m_renderedSvgs.clear();*/ }

void EPubDocumentPrivate::setDocumentPath(const QString& documentPath)
{
  m_documentPath = documentPath;
}

QString EPubDocumentPrivate::buildTocFromFiles()
{
  return m_container->buildTocfromHtml();
}

void EPubDocumentPrivate::loadDocument()
{
  Q_Q(EPubDocument);

  m_container = new EPubContainer(q_ptr);

  if (!m_container->loadFile(m_documentPath)) {
    return;
  }

  // add the images as resources
  QSize image_size(int(q->pageSize().width() - q->documentMargin() * 4),
                   int(q->pageSize().height() - q->documentMargin() * 4));
  foreach (QString name, m_container->imageKeys()) {
    SharedManifestItem item = m_container->item(name);
    QImage image = m_container->image(name, image_size);
    q->addResource(QTextDocument::ImageResource, QUrl(item->path),
                   QVariant(image));
  }

  foreach (QString name, m_container->cssKeys()) {
    SharedManifestItem item = m_container->item(name);
    QString data = m_container->css(name);
    q->addResource(QTextDocument::StyleSheetResource, QUrl(item->path),
                   QVariant(data));
  }

  // TODO QTextDocument doesn't seem to support javascript yet
  //  foreach (QString name, m_container->jsKeys()) {
  //    SharedManifestItem item = m_container->item(name);
  //    QString data = m_container->javascript(name);
  //    q->addResource(QTextDocument::StyleSheetResource, QUrl(item->path),
  //                   QVariant(data));
  //  }

  QStringList spine_items = m_container->spineKeys();
  QTextCursor cursor(q_ptr);
  cursor.movePosition(QTextCursor::End);
  //  SharedTextCursor cursor = SharedTextCursor(new QTextCursor(q_ptr));
  QTextBlockFormat pageBreak;
  pageBreak.setPageBreakPolicy(QTextFormat::PageBreak_AlwaysBefore);
  for (const QString& chapter : spine_items) {
    SharedManifestItem item = m_container->item(chapter);
//    SharedDomDocument shared_domdocument = item->dom_document;
    QString document = item->document_string;
    if (document.isEmpty()) {
      QLOG_WARN(QString("Got an empty document"))
      continue;
    }

    // mark the start of the block
    int start = cursor.position();
    //    item->start = SharedTextCursor(new QTextCursor(*cursor.data()));

//    QString data = shared_domdocument->toString();
    cursor.insertHtml(document);
    cursor.insertBlock(pageBreak);

    // mark the end of the block
    int end = cursor.position();

    item->start_cursor = QTextCursor(q_ptr);
    item->start_cursor.setPosition(start);
    item->end_cursor = QTextCursor(q_ptr);
    item->end_cursor.setPosition(end);
  }

  q->setBaseUrl(QUrl()); // base url to empty.
  m_loaded = true;

  emit q->loadCompleted();
}

QString EPubDocumentPrivate::toc()
{
  return m_container->tocAsString();
}

// QVariant EPubDocumentPrivate::loadResource(int type, const QUrl& url)
//{
//  Q_Q(EPubDocument);

//  if (url.scheme() == "svgcache") {
//    return getSvgImage(url.path());
//  }

//  QSharedPointer<QIODevice> ioDevice = m_container->zipFile(url.path());
//  if (!ioDevice) {
//    //    qWarning() << "Unable to get io device for" << url;
//    return QVariant();
//  }
//  QByteArray data = ioDevice->readAll();

//  if (type == QTextDocument::StyleSheetResource) {
//    QString cssData = QString::fromLocal8Bit(data);
//    cssData.replace("@charset \"", "@charset\"");
//    data = cssData.toLocal8Bit();
//  }

//  q->addResource(type, url, data);

//  return data;
//}

EPubContents* EPubDocumentPrivate::cloneData()
{
  EPubContents* contents = new EPubContents();
  contents->m_container = m_container;
  contents->m_loaded = m_loaded;

  return contents;
}

void EPubDocumentPrivate::setClonedData(EPubContents* clone)
{
  Q_Q(EPubDocument);

  //  QElapsedTimer timer;
  //  timer.start();

  //  m_container = clone->m_container;
  //  m_svgs = clone->m_svgs;
  //  m_renderedSvgs = clone->m_renderedSvgs;
  //  m_currentItem = clone->m_currentItem;
  //  m_loaded = clone->m_loaded;

  //  QTextCursor cursor(q_ptr);
  //  cursor.movePosition(QTextCursor::End);

  //  QStringList items = m_container->items();

  //  QString cover = m_container->standardPage(EPubPageReference::CoverPage);
  //  if (cover.isEmpty()) {
  //    items.prepend(cover);
  //    QLOG_DEBUG(QString("Cover is empty %1").arg(cover))
  //  }

  //  QTextBlockFormat pageBreak;
  //  pageBreak.setPageBreakPolicy(QTextFormat::PageBreak_AlwaysBefore);
  //  for (const QString& chapter : items) {
  //    m_currentItem = m_container->epubItem(chapter);
  //    if (m_currentItem.path.isEmpty()) {
  //      continue;
  //    }

  //    QSharedPointer<QuaZipFile> zipFile =
  //        m_container->zipFile(m_currentItem.path);
  //    if (!zipFile) {
  //      QLOG_WARN(QString("Unable to get zipped file for chapter %1")
  //                    .arg(m_currentItem.path))
  //      continue;
  //    }

  //    QByteArray itemdata = zipFile->readAll();
  //    if (itemdata.isEmpty()) {
  //      QLOG_WARN(QString("Got an empty document"))
  //      continue;
  //    }
  //    q->setBaseUrl(QUrl(m_currentItem.path));
  //    QDomDocument newDocument;
  //    newDocument.setContent(itemdata);
  //    // TOD extract text for processing
  //    fixImages(newDocument);

  //    cursor.insertText(newDocument.toString());
  //    cursor.insertBlock(pageBreak);
  //  }

  //  q->setBaseUrl(QUrl());
  //  m_loaded = true;

  //  emit q->loadCompleted();
  //  //  QLOG_DEBUG(QString("Done in %1 mS").arg(timer.elapsed()))
}

QStringList EPubDocumentPrivate::creators()
{
  return m_container->creators();
}

QString EPubDocumentPrivate::title()
{
  SharedTitle first = m_container->metadata()->ordered_titles.first();
  if (!first.isNull())
    return first->title;
  else {
    return QString();
  }
}
