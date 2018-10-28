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
#include <QThread>

#include <qlogger.h>

#include "epubcontainer.h"

using namespace qlogger;

#ifdef DEBUG_CSS
#include <private/qcssparser_p.h>
#endif

EPubDocument::EPubDocument(QObject *parent)
    : EBookDocument(parent), m_container(nullptr), m_loaded(false) {
  setUndoRedoEnabled(false);
}

EPubDocument::EPubDocument(const EPubDocument &doc) {}

EPubDocument::~EPubDocument() {}

void EPubDocument::openDocument(const QString &path) {
  m_documentPath = path;
  loadDocument();
  m_data.title = title();
  m_data.authors = authors();
  m_data.author_names = authorNames();
  m_data.language = language();
  m_data.subject = subject();
  m_data.date = date();
  m_data.publisher = publisher();
}

void EPubDocument::loadDocument() {
  QElapsedTimer timer;
  timer.start();
  m_container = new EPubContainer(this);
  connect(m_container, &EPubContainer::errorHappened,
          [](QString error) { QLOG_WARN(QString("%1").arg(error)) });

  if (!m_container->openFile(m_documentPath)) {
    return;
  }
  QTextCursor cursor(this);
  cursor.movePosition(QTextCursor::End);

  QStringList items = m_container->items();

  QString cover = m_container->standardPage(EpubPageReference::CoverPage);
  if (cover.isEmpty()) {
    items.prepend(cover);
    QLOG_DEBUG(QString("Cover is empty %1").arg(cover))
  }

  QTextBlockFormat pageBreak;
  pageBreak.setPageBreakPolicy(QTextFormat::PageBreak_AlwaysBefore);
  for (const QString &chapter : items) {
    m_currentItem = m_container->epubItem(chapter);
    if (m_currentItem.path.isEmpty()) {
      continue;
    }

    QSharedPointer<QuaZipFile> zipFile =
        m_container->zipFile(m_currentItem.path);
    if (!zipFile) {
      QLOG_WARN(QString("Unable to get iodevice for chapter %1")
                    .arg(m_currentItem.path))
      continue;
    }

    QByteArray data = zipFile->readAll();
    if (data.isEmpty()) {
      QLOG_WARN(QString("Got an empty document"))
      continue;
    }
    setBaseUrl(QUrl(m_currentItem.path));
    QDomDocument newDocument;
    newDocument.setContent(data);
    fixImages(newDocument);

    cursor.insertHtml(newDocument.toString());
    cursor.insertBlock(pageBreak);
  }

  setBaseUrl(QUrl());
  m_loaded = true;

  emit loadCompleted();
  QLOG_DEBUG(QString("Done in %1 mS").arg(timer.elapsed()))
}

void EPubDocument::fixImages(QDomDocument &newDocument) {
  { // Fix relative URLs, images are lazily loaded so the base URL might not
    // be correct when they are loaded
    QDomNodeList imageNodes = newDocument.elementsByTagName("img");
    for (int i = 0; i < imageNodes.count(); i++) {
      QDomElement image = imageNodes.at(i).toElement();
      if (!image.hasAttribute("src")) {
        continue;
      }
      QUrl href = QUrl(image.attribute("src"));
      href = baseUrl().resolved(href);
      image.setAttribute("src", href.toString());
    }
  }

  { // QImage which QtSvg uses isn't able to read files from inside the archive,
    // so embed image data inline
    QDomNodeList imageNodes =
        newDocument.elementsByTagName("image"); // SVG images
    for (int i = 0; i < imageNodes.count(); i++) {
      QDomElement image = imageNodes.at(i).toElement();
      if (!image.hasAttribute("xlink:href")) {
        continue;
      }
      QString path = image.attribute("xlink:href");
      QByteArray fileData = loadResource(0, QUrl(path)).toByteArray();
      QByteArray data = "data:image/jpeg;base64," + fileData.toBase64();
      image.setAttribute("xlink:href", QString::fromLatin1(data));
    }
  }

  static int svgCounter = 0;

  // QTextDocument isn't fond of SVGs, so rip them out and store them
  // separately, and give it <img> instead
  QDomNodeList svgNodes = newDocument.elementsByTagName("svg");
  for (int i = 0; i < svgNodes.count(); i++) {
    QDomElement svgNode = svgNodes.at(i).toElement();

    // Serialize out the old SVG, store it
    QDomDocument tempDocument;
    tempDocument.appendChild(tempDocument.importNode(svgNode, true));
    QString svgId = QString::number(++svgCounter);
    m_svgs.insert(svgId, tempDocument.toByteArray());

    // Create <img> node pointing to our SVG image
    QDomElement imageElement = newDocument.createElement("img");
    imageElement.setAttribute("src", "svgcache:" + svgId);

    // Replace <svg> node with our <img> node
    QDomNode parent = svgNodes.at(i).parentNode();
    parent.replaceChild(imageElement, svgNode);
  }
}

const QImage &EPubDocument::getSvgImage(const QString &id) {
  if (m_renderedSvgs.contains(id)) {
    return m_renderedSvgs[id];
  }
  if (!m_svgs.contains(id)) {
    qWarning() << "Couldn't find SVG" << id;
    static QImage nullImg;
    return nullImg;
  }

  QSize imageSize(int(pageSize().width() - documentMargin() * 4),
                  int(pageSize().height() - documentMargin() * 4));

  QSvgRenderer renderer(m_svgs.value(id));
  QSize svgSize(renderer.viewBox().size());

  if (svgSize.isValid()) {
    svgSize.scale(imageSize, Qt::KeepAspectRatio);
  } else {
    svgSize = imageSize;
  }

  QImage rendered(svgSize, QImage::Format_ARGB32);
  QPainter painter(&rendered);
  renderer.render(&painter);
  painter.end();

  m_renderedSvgs.insert(id, rendered);
  return m_renderedSvgs[id];
}

QVariant EPubDocument::loadResource(int type, const QUrl &url) {
  Q_UNUSED(type);

  if (url.scheme() == "svgcache") {
    return getSvgImage(url.path());
  }

  QSharedPointer<QIODevice> ioDevice = m_container->zipFile(url.path());
  if (!ioDevice) {
    qWarning() << "Unable to get io device for" << url;
    return QVariant();
  }
  QByteArray data = ioDevice->readAll();

  if (type == QTextDocument::StyleSheetResource) {
    QString cssData = QString::fromLocal8Bit(data);
    cssData.replace("@charset \"", "@charset\"");
    data = cssData.toLocal8Bit();

#ifdef DEBUG_CSS
//        QCss::Parser parser(cssData);
//        QCss::StyleSheet stylesheet;
//        qDebug() << "Parse success?" << parser.parse(&stylesheet);
//        qDebug().noquote() << parser.errorIndex <<
//        parser.errorSymbol().lexem();
#endif
  }

  addResource(type, url, data);

  return data;
}

QString EPubDocument::title() const { return m_container->metadata("title"); }

void EPubDocument::setTitle(const QString &title) {
  m_container->setMetadata("title", title);
}

QString EPubDocument::subject() const {
  return m_container->metadata("subject");
}

void EPubDocument::setSubject(const QString &subject) {
  m_container->setMetadata("subject", subject);
}

QString EPubDocument::language() const {
  return m_container->metadata("language");
}

void EPubDocument::setLanguage(const QString &language) {
  m_container->setMetadata("language", language);
}

QDateTime EPubDocument::date() const {
  QDateTime date = QDateTime::fromString(m_container->metadata("date"));
  return date;
}

void EPubDocument::setDate(const QDateTime &date) {
  m_container->setMetadata("date", date.toString(Qt::ISODate));
}

QStringList EPubDocument::authors() const {
  QStringList authors =
      CSVSplitter::splitOnSemicolon(m_container->metadata("creator"));
  return authors;
}

void EPubDocument::setAuthors(const QStringList &authors) {
  QString authorlist;
  foreach (QString author, authors) {
    if (!authorlist.isEmpty()) {
      authorlist += "; ";
    }
    authorlist += author;
  }
  m_container->setMetadata("creator", authorlist);
}

QString EPubDocument::authorNames() const
{
  return m_container->metadata("creator");
}


QString EPubDocument::publisher() const {
  return m_container->metadata("publisher");
}

void EPubDocument::setPublisher(const QString &publisher) {
  m_container->setMetadata("publisher", publisher);
}
