#include "epubdocument_p.h"

#include <csvsplitter/csvsplitter.h>
#include <qlogger/qlogger.h>
using namespace qlogger;

EPubDocumentPrivate::EPubDocumentPrivate(EPubDocument* parent) :
    q_ptr(parent), m_container(nullptr), m_loaded(false)
{
}

EPubDocumentPrivate::~EPubDocumentPrivate() {}

bool EPubDocumentPrivate::loaded() { return m_loaded; }

void EPubDocumentPrivate::openDocument(const QString& path)
{
  //  Q_Q(EPubDocument);
  setDocumentPath(path);
  loadDocument();

  //    m_data->toc = m_container->toc();
}

void EPubDocumentPrivate::saveDocument() { m_container->saveFile(); }

void EPubDocumentPrivate::clearCache() { m_renderedSvgs.clear(); }

void EPubDocumentPrivate::setDocumentPath(const QString& documentPath)
{
  m_documentPath = documentPath;
}

void EPubDocumentPrivate::loadDocument()
{
  Q_Q(EPubDocument);

  m_container = new EPubContainer(q_ptr);

  if (!m_container->loadFile(m_documentPath)) {
    return;
  }

  QTextCursor cursor(q_ptr);
  cursor.movePosition(QTextCursor::End);

  QStringList items = m_container->spineKeys();

  QTextBlockFormat pageBreak;
  pageBreak.setPageBreakPolicy(QTextFormat::PageBreak_AlwaysBefore);
  for (const QString& chapter : items) {
    SharedDomDocument item_data = m_container->item(chapter);
    if (item_data.isNull()) {
      QLOG_WARN(QString("Got an empty document"))
      continue;
    }
    //    fixImages(item_data);

    cursor.insertHtml(item_data->toString());
    cursor.insertBlock(pageBreak);
  }

  q->setBaseUrl(QUrl());
  m_loaded = true;

  emit q->loadCompleted();
  //  QLOG_DEBUG(QString("Done in %1 mS").arg(timer.elapsed()))
}

// void EPubDocumentPrivate::fixImages(SharedDomDocument newDocument)
//{
//  Q_Q(EPubDocument);
//  {
//    // Fix relative URLs, images are lazily loaded so the base URL might not
//    // be correct when they are loaded
//    QDomNodeList imageNodes = newDocument->elementsByTagName("img");
//    for (int i = 0; i < imageNodes.count(); i++) {
//      QDomElement image = imageNodes.at(i).toElement();
//      if (!image.hasAttribute("src")) {
//        continue;
//      }
//      QUrl href = QUrl(image.attribute("src"));
//      href = q->baseUrl().resolved(href);
//      image.setAttribute("src", href.toString());
//    }
//  }

//  {
//    // QImage which QtSvg uses isn't able to read files from inside the
//    archive,
//    // so embed image data inline
//    QDomNodeList imageNodes =
//        newDocument->elementsByTagName("image"); // SVG images
//    for (int i = 0; i < imageNodes.count(); i++) {
//      QDomElement image = imageNodes.at(i).toElement();
//      if (!image.hasAttribute("xlink:href")) {
//        continue;
//      }
//      QString path = image.attribute("xlink:href");
//      QByteArray fileData = loadResource(0, QUrl(path)).toByteArray();
//      QByteArray data = "data:image/jpeg;base64," + fileData.toBase64();
//      image.setAttribute("xlink:href", QString::fromLatin1(data));
//    }
//  }

//  static int svgCounter = 0;

//  // QTextDocument isn't fond of SVGs, so rip them out and store them
//  // separately, and give it <img> instead
//  QDomNodeList svgNodes = newDocument.elementsByTagName("svg");
//  for (int i = 0; i < svgNodes.count(); i++) {
//    QDomElement svgNode = svgNodes.at(i).toElement();

//    // Serialize out the old SVG, store it
//    QDomDocument tempDocument;
//    tempDocument.appendChild(tempDocument.importNode(svgNode, true));
//    QString svgId = QString::number(++svgCounter);
//    m_svgs.insert(svgId, tempDocument.toByteArray());

//    // Create <img> node pointing to our SVG image
//    QDomElement imageElement = newDocument.createElement("img");
//    imageElement.setAttribute("src", "svgcache:" + svgId);

//    // Replace <svg> node with our <img> node
//    QDomNode parent = svgNodes.at(i).parentNode();
//    parent.replaceChild(imageElement, svgNode);
//  }
//}

// const QImage& EPubDocumentPrivate::getSvgImage(const QString& id)
//{
//  Q_Q(EPubDocument);
//    if (m_manifest..contains(id)) {
//      return m_renderedSvgs[id];
//    }
//    if (!m_svgs.contains(id)) {
//      //    qWarning() << "Couldn't find SVG" << id;
//      static QImage nullImg;
//      return nullImg;
//    }

//  QSize imageSize(int(q->pageSize().width() - q->documentMargin() * 4),
//                  int(q->pageSize().height() - q->documentMargin() * 4));

//  QSvgRenderer renderer(m_svgs.value(id));
//  QSize svgSize(renderer.viewBox().size());

//  if (svgSize.isValid()) {
//    svgSize.scale(imageSize, Qt::KeepAspectRatio);
//  } else {
//    svgSize = imageSize;
//  }

//  QImage rendered(svgSize, QImage::Format_ARGB32);
//  QPainter painter(&rendered);
//  renderer.render(&painter);
//  painter.end();

//  m_renderedSvgs.insert(id, rendered);
//  return m_renderedSvgs[id];
//}

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
  contents->m_svgs = m_svgs;
  contents->m_renderedSvgs = m_renderedSvgs;
  contents->m_currentItem = m_currentItem;
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
