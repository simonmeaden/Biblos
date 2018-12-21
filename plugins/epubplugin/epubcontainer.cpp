#include "epubcontainer.h"

#include <quazip5/quazip.h>
#include <quazip5/quazipfile.h>

#include <QDebug>
#include <QDir>
#include <QDomDocument>
#include <QImage>
#include <QImageReader>
#include <QScopedPointer>
#include <QSharedPointer>

#include <csvsplitter/csvsplitter.h>
#include <qlogger/qlogger.h>

#include <ebookcommon.h>

using namespace qlogger;

const QString EPubContainer::METADATA_FOLDER = "META-INF";
const QString EPubContainer::MIMETYPE_FILE = "mimetype";
const QByteArray EPubContainer::MIMETYPE = "application/epub+zip";
const QString EPubContainer::CONTAINER_FILE = "META-INF/container.xml";
const QString EPubContainer::TOC_FILE = "toc.ncx";

const QString EPubContainer::TITLE = "title";
const QString EPubContainer::CREATOR = "creator";
const QString EPubContainer::IDENTIFIER = "identifier";
const QString EPubContainer::LANGUAGE = "language";

EPubContainer::EPubContainer(QObject* parent) :
    QObject(parent), m_archive(Q_NULLPTR) /*, m_toc(new EPubToc())*/,
    m_metadata(new EPubMetadata())
{
}

EPubContainer::~EPubContainer() {}

bool EPubContainer::loadFile(const QString path)
{
  // open the epub as a zip file
  m_archive = new QuaZip(path);
  m_filename = path; // stored against modification;
  if (!m_archive->open(QuaZip::mdUnzip)) {
    QLOG_DEBUG(tr("Failed to open %1").arg(path));
    return false;
  }

  // get list of filenames from zip file
  m_files = m_archive->getFileNameList();
  if (m_files.isEmpty()) {
    QLOG_DEBUG(tr("Failed to read %1").arg(path));
    return false;
  }

  // Get and check that the mimetype is correct.
  // According to the standard this must be unencrypted.
  if (!parseMimetype()) {
    return false;
  }

  if (!parseContainer()) {
    return false;
  }

  m_archive->close();

  return true;
}

bool EPubContainer::saveFile(const QString path)
{
  m_archive = new QuaZip(path);

  if (!saveMimetype()) {
    return false;
  }
  if (!saveContainer()) {
    return false;
  }
  // TODO the rest

  m_archive->close();
}

EBookItem EPubContainer::epubItem(const QString& id) const
{
  //  return m_items.value(id);
}

QSharedPointer<QuaZipFile> EPubContainer::zipFile(const QString& path)
{
  m_archive->setCurrentFile(path);
  QuaZipFile* file = new QuaZipFile(path);
  file->setZip(m_archive);

  if (!file->open(QIODevice::ReadOnly)) {
    QLOG_DEBUG(tr("Unable to open file %1").arg(path));
    return QSharedPointer<QuaZipFile>();
  }

  return QSharedPointer<QuaZipFile>(file);
}

QImage EPubContainer::image(const QString& id)
{
  //  if (!m_items.contains(id)) {
  //    QLOG_WARN(tr("Asked for unknown item %1").arg(id));
  //    return QImage();
  //  }

  //  const EBookItem& item = m_items.value(id);

  //  if (!QImageReader::supportedMimeTypes().contains(item.mimetype)) {
  //    QLOG_WARN(tr("Asked for unsupported type
  //    %1").arg(QString(item.mimetype))); return QImage();
  //  }

  //  QSharedPointer<QuaZipFile> unzippedFile = zipFile(item.path);

  //  if (!unzippedFile) {
  //    return QImage();
  //  }

  //  return QImage::fromData(unzippedFile->readAll());
}

QStringList EPubContainer::metadata(const QString& key)
{
  //  QStringList list = metadata_old.values(key);
  //  return list;
}

int EPubContainer::removeMetadata(const QString& key)
{
  //  return metadata_old.remove(key);
}

void EPubContainer::setMetadata(const QString& key, const QString& value)
{
  //  metadata_old.insert(key, value);
}

QList<QMap<QString, QString>>
    EPubContainer::metadataAttributes(const QString& key)
{
  //  QList<QMap<QString, QString>> list = m_tag_attributes.values(key);
  //  return list;
}

void EPubContainer::setMetadataAttributes(const QString& key,
                                          QMap<QString, QString> values)
{
  //  m_tag_attributes.insert(key, values);
}

int EPubContainer::removeMetadataAttributes(const QString& key)
{
  //  return m_tag_attributes.remove(key);
}

QStringList EPubContainer::items()
{ /*return m_orderedItems;*/
}

QString EPubContainer::standardPage(EPubPageReference::StandardType type)
{
  //  return m_standardReferences.value(type).target;
}

bool EPubContainer::parseMimetype()
{
  if (m_files.contains(MIMETYPE_FILE)) {
    m_archive->setCurrentFile(MIMETYPE_FILE);
    QuaZipFile mimetypeFile(m_archive);

    if (!mimetypeFile.open(QIODevice::ReadOnly)) {
      m_archive->getZipError();
      QLOG_DEBUG(tr("Unable to open mimetype file"));
      return false;
    }

    QByteArray mimetype = mimetypeFile.readAll();
    if (mimetype != MIMETYPE) {
      QLOG_DEBUG(tr("Unexpected mimetype %1").arg(QString(mimetype)));
    }
  } else {
    QLOG_DEBUG(tr("Unable to find mimetype in file"));
    return false;
  }
  return true;
}

bool EPubContainer::saveMimetype()
{
  m_archive->setCurrentFile(MIMETYPE_FILE);
  QuaZipFile mimetypeFile(m_archive);

  if (!mimetypeFile.open(QIODevice::WriteOnly)) {
    m_archive->getZipError();
    QLOG_DEBUG(tr("Unable to open mimetype file"));
    return false;
  }

  QByteArray mimetype = MIMETYPE;
  qint64 size = mimetypeFile.write(mimetype);
  if (size != mimetype.size()) {
    QLOG_DEBUG(tr("Unexpected mimetype size %1 should be %2")
                   .arg(size)
                   .arg(mimetype.size()));
    return false;
  }
  return true;
}

bool EPubContainer::parseContainer()
{
  if (m_files.contains(CONTAINER_FILE)) {
    m_archive->setCurrentFile(CONTAINER_FILE);
    QuaZipFile containerFile(m_archive);
    containerFile.setZip(m_archive);

    if (!containerFile.open(QIODevice::ReadOnly)) {
      int error = m_archive->getZipError();
      QLOG_DEBUG(tr("Unable to open container file error %1").arg(error));
      return false;
    }

    QString container(containerFile.readAll());
    m_container_document = SharedDomDocument(new QDomDocument());
    m_container_document->setContent(container);
    QDomNodeList root_nodes =
        m_container_document->elementsByTagName("rootfile");
    for (int i = 0; i < root_nodes.count(); i++) {
      QDomElement rootElement = root_nodes.at(i).toElement();
      QString full_path = rootElement.attribute("full-path");
      QString xmlns = rootElement.attribute("xmlns");
      if (full_path.isEmpty()) {
        QLOG_WARN(tr("Invalid root file entry"));
        continue;
      }
      if (parsePackageFile(full_path)) {
        return true;
      }
    }

  } else {
    QLOG_DEBUG(tr("Unable to find container information"));
    return false;
  }

  // Limitations:
  //  - We only read one rootfile
  //  - We don't read the following from META-INF/
  //     - manifest.xml (unknown contents, just reserved)
  //     - metadata.xml (unused according to spec, just reserved)
  //     - rights.xml (reserved for DRM, not standardized)
  //     - signatures.xml (signatures for files, standardized)
  // Actually these are rarely included in an epub file anyway.

  QLOG_DEBUG(tr("Unable to find and use any content files"));
  return false;
}

bool EPubContainer::parsePackageFile(QString& full_path)
{
  m_archive->setCurrentFile(full_path);
  QuaZipFile contentFile(m_archive);
  contentFile.setZip(m_archive);

  if (!contentFile.open(QIODevice::ReadOnly)) {
    QLOG_DEBUG(tr("Malformed content file, unable to get content metadata"));
    return false;
  }

  QMap<QString, SharedDomDocument> map;
  m_current_rootfile = map;
  m_rootfiles.insert(full_path, m_current_rootfile);

  QString content(contentFile.readAll());

  SharedDomDocument package_document(new QDomDocument());
  // handles more than one package.
  m_current_rootfile.insert(full_path, package_document);

  package_document->setContent(content, true); // turn on namespace processing
  // parse root element attributes.
  QDomElement root_element = package_document->documentElement();
  QDomNamedNodeMap node_map = root_element.attributes();
  for (int i = 0; i < node_map.size(); i++) {
    QDomNode node = node_map.item(i);
    QString name = node.nodeName();
    QString value = node.nodeValue();
    // parse package attributes.
    if (name == "version") {
      if (value == "2.0") {
        m_version = 2;
      } else if (value == "3.0") {
        m_version = 3;
      }
    } else if (name == "unique-identifier") {
      m_unique_identifier_name = value;
    } else if (name == "xml:lang") {
      m_language = value;
    } else if (name == "prefix") {
      // TODO - handle prefix mapping - may not need this, just store value.
      m_prefix = value;
    } else if (name == "dir") {
      m_dir = value;
    } else if (name == "id") {
      m_id = value;
    }
  }

  // parse metadata.
  QDomNodeList metadataNodeList =
      package_document->elementsByTagName("metadata");
  for (int i = 0; i < metadataNodeList.count(); i++) {
    QDomNodeList metadataChildList = metadataNodeList.at(i).childNodes();
    for (int j = 0; j < metadataChildList.count(); j++) {
      parseMetadataItem(metadataChildList.at(j));
    }
  }

  //  // Extract current path, for resolving relative paths
  //  QString contentFileFolder;
  //  int separatorIndex = full_path.lastIndexOf('/');
  //  if (separatorIndex > 0) {
  //    contentFileFolder = full_path.left(separatorIndex + 1);
  //  }

  //  // Parse out all the components/items in the epub
  //  QDomNodeList manifestNodeList =
  //      package_document->elementsByTagName("manifest");
  //  for (int i = 0; i < manifestNodeList.count(); i++) {
  //    QDomElement manifestElement = manifestNodeList.at(i).toElement();
  //    QDomNodeList manifestItemList =
  //    manifestElement.elementsByTagName("item");

  //    for (int j = 0; j < manifestItemList.count(); j++) {
  //      parseManifestItem(manifestItemList.at(j), contentFileFolder);
  //    }
  //  }

  //  // Parse out the document order
  //  QDomNodeList spineNodeList = package_document->elementsByTagName("spine");
  //  for (int i = 0; i < spineNodeList.count(); i++) {
  //    QDomElement spineElement = spineNodeList.at(i).toElement();

  //    QString tocId = spineElement.attribute("toc");
  //    if (!tocId.isEmpty() && m_items.keys().contains(tocId)) {
  //      EPubPageReference tocReference;
  //      tocReference.title = tr("Table of Contents");
  //      tocReference.target = tocId;
  //      m_standardReferences.insert(EPubPageReference::TableOfContents,
  //                                  tocReference);
  //    }

  //    QDomNodeList spineItemList = spineElement.elementsByTagName("itemref");
  //    for (int j = 0; j < spineItemList.count(); j++) {
  //      parseSpineItem(spineItemList.at(j));
  //    }
  //  }

  //  // Parse out standard items
  //  QDomNodeList guideNodeList = package_document->elementsByTagName("guide");
  //  for (int i = 0; i < guideNodeList.count(); i++) {
  //    QDomElement guideElement = guideNodeList.at(i).toElement();

  //    QDomNodeList guideItemList =
  //    guideElement.elementsByTagName("reference"); for (int j = 0; j <
  //    guideItemList.count(); j++) {
  //      parseGuideItem(guideItemList.at(j));
  //    }
  //  }

  return true;
}

void EPubContainer::saveTitles(QDomElement metadata_element)
{
  QString id;
  SharedTitle shared_title;
  QDomElement elem;
  if (m_metadata->titles.keys().size() == 1) {
    shared_title = m_metadata->titles.first();
    elem = QDomElement();
    elem.setTagName("dc:title");
    elem.setNodeValue(shared_title->title);
    if (shared_title->id.isEmpty()) {
      // no id exists so just create an arbitrary id.
      id = "title";
    } else {
      // otherwise use existing id.
      id = shared_title->id;
    }
    elem.setAttribute("id", id);
    id = "#" + id;
    // create basic meta refines for title.
    metadata_element.appendChild(elem);

    elem = QDomElement();
    elem.setTagName("meta");
    elem.setAttribute("refines", id);
    elem.setAttribute("title-type", "main");
    metadata_element.appendChild(elem);
  } else {
    for (int i = 0; i < m_metadata->titles.keys().size(); i++) {
      QString key = m_metadata->titles.keys().at(i);
      shared_title = m_metadata->titles.value(key);
      elem = QDomElement();
      elem.setNodeValue(shared_title->title);
      // ALL of these should have an id.
      id = shared_title->id;
      elem.setAttribute("id", id);
      metadata_element.appendChild(elem);

      // ALL of these should have a title-type
      id = "#" + id;
      elem = QDomElement();
      elem.setTagName("meta");
      elem.setAttribute("refines", id);
      elem.setAttribute("title-type", "main");

      // this is optional
      if (shared_title->sequence >= 0) {
        elem = QDomElement();
        elem.setTagName("meta");
        elem.setAttribute("display-seq",
                          QString::number(shared_title->sequence));
        metadata_element.appendChild(elem);
      }
    }
  }
}

bool EPubContainer::savePackageFile(QString& full_path)
{
  QDomDocument doc(full_path);
  // create the base package tags including optional elements.
  QDomElement root = doc.createElement("package");
  root.setAttribute("xmlns", "http://www.idpf.org/2007/opf");
  root.setAttribute("version", "3.0");
  if (!m_unique_identifier_name
           .isEmpty()) { // actually this is a required element
    root.setAttribute("unique-identifier", m_unique_identifier_name);
  } else {
    // TODO create a unique identifier ??
  }
  if (!m_language.isEmpty()) {
    root.setAttribute("xml:lang", m_language);
  }
  if (!m_prefix.isEmpty()) {
    root.setAttribute("prefix", m_prefix);
  }
  if (!m_dir.isEmpty()) {
    root.setAttribute("dir", m_dir);
  }
  if (!m_id.isEmpty()) {
    root.setAttribute("id", m_id);
  }

  QDomElement metadata_element, elem;
  metadata_element.setTagName("metadata");

  // add the unique identifier meta. This is basically mandatory because
  // the unique-identifier attribute of package is mandatory in 2.0 and 3.0.
  SharedIdentifier unigue_identifier =
      m_metadata->identifiers.value(m_unique_identifier_name);
  elem.setTagName("dc:identifier");
  elem.setAttribute("id", m_unique_identifier_name);
  elem.setNodeValue(unigue_identifier->uid);
  metadata_element.appendChild(elem);

  saveTitles(metadata_element);
  //    <dc:creator opf:role="aut" opf:file-as="Flint, Eric">Eric
  //    Flint</dc:creator>

  //  <dc:creator id="creator">Herman Melville</dc:creator>
  //   <meta refines="#creator" property="file-as">MELVILLE, HERMAN</meta>
  //   <meta refines="#creator" property="role"
  //   scheme="marc:relators">aut</meta>

  //  <dc:creator id="aut1">Various</dc:creator>
  //   <meta refines="#aut1" property="role" scheme="marc:relators">aut</meta>

  // TODO - the rest of the saves.
  return true;
}

bool EPubContainer::parseMetadataItem(const QDomNode& metadataNode)
{
  QDomElement metadata_element = metadataNode.toElement();
  QString tag_name = metadata_element.tagName();
  QDomNamedNodeMap node_map = metadata_element.attributes();
  QDomNode node;
  QString name;

  if (tag_name == "meta") {
    node = node_map.namedItem("refines");
    if (!node.isNull()) { // # refines earler values title etc.
      name = node.nodeValue();
      if (name.startsWith("#")) {
        name = name.right(name.length() - 1);
        if (m_metadata->titles.contains(name)) {
          // is it a title?
          SharedTitle shared_title = m_metadata->titles.value(name);
          node = node_map.namedItem("property");
          if (!node.isNull()) {
            name = node.nodeValue();
            if (name == "title-type")
              shared_title->type =
                  EPubTitle::fromString(metadata_element.text());
            else if (name == "display-seq")
              shared_title->sequence = metadata_element.text().toInt();
          }
        } else if (m_metadata->creators.contains(name)) {
          // is it a creator
          SharedCreator shared_creator = m_metadata->creators.value(name);
          node = node_map.namedItem("property");
          if (!node.isNull()) {
            name = node.nodeValue();
            if (name == "role") {
              node = node_map.namedItem("scheme");
              name = node.nodeValue();
              if (name == "marc:relators") {
                shared_creator->type =
                    EPubCreator::fromCreatorString(metadata_element.text());
                if (shared_creator->type == EPubCreator::string_creator_type)
                  shared_creator->string_creator = metadata_element.text();
              } else {
                // TODO treat as a string if not a recognised scheme type;
                shared_creator->string_scheme = metadata_element.text();
              }
            }
          }
        }
      }
    } else {
      // metadata elements that do not refine other elements.
      node = node_map.namedItem("property");
      if (!node.isNull()) {
        name = node.nodeValue();
        if (name == "dcterms:modified") {
          extractDateModified(node_map, metadata_element.text());
        } else if (name == "dcterms:source") {
          if (m_metadata->source.isEmpty()) {
            m_metadata->source = metadata_element.text();
          } else {
            // Not certain whether multi sources are allowed?
            QLOG_DEBUG(
                QString(
                    "Problem setting document source : %1 when %2 already set.")
                    .arg(metadata_element.text().arg(m_metadata->source)));
          }
        }
      }
    }
  } else if (metadata_element.prefix() == "dc" ||
             metadata_element.prefix() == "opf") {
    if (tag_name == "creator") {
      extractCreatorMetadata(metadata_element);
    } else if (tag_name == "title") {
      extractTitleMetadata(metadata_element);
    } else if (tag_name == "identifier") {
      extractIdentifierMetadata(metadata_element);
    } else if (tag_name == "language") {
      extractLanguageMetadata(metadata_element);
    } else if (tag_name == "description") {
      m_metadata->description = metadata_element.text();
    } else if (tag_name == "publisher") {
      m_metadata->publisher = metadata_element.text();
    } else if (tag_name == "date") {
      m_metadata->date =
          QDateTime::fromString(metadata_element.text(), Qt::ISODate);
      QDomNode node = node_map.namedItem("id");
      if (!node.isNull()) {
        m_metadata->modified.id = node.nodeValue();
      }
    } else if (tag_name == "rights") {
      m_metadata->rights = metadata_element.text();
    } else if (tag_name == "contributor") {
      // TODO
    } else if (tag_name == "subject") {
      // TODO
    } else {
      // TODO
    }
  } else {
    // TODO not a dc element, maybe calibre?
  }

  return true;
}

void EPubContainer::extractTitleMetadata(QDomElement metadata_element)
{
  QDomNamedNodeMap node_map = metadata_element.attributes();
  QDomNode node = node_map.namedItem("id");
  SharedTitle shared_title = SharedTitle(new EPubTitle());
  SharedTitle title = SharedTitle(new EPubTitle());
  node = node_map.namedItem("id");
  if (node.isNull()) { // id element is NOT used in EPUB 2.0
    title->id = node.nodeValue();
    shared_title = title.dynamicCast<EPubTitle>();
  }
  // TODO optional lang and dir attributes.
  shared_title->title = metadata_element.text();
  m_metadata->titles.insert(shared_title->id, shared_title);
}

void EPubContainer::extractCreatorMetadata(QDomElement metadata_element)
{
  QDomNamedNodeMap node_map = metadata_element.attributes();
  SharedCreator shared_creator = SharedCreator(new EPubCreator());
  QDomNode node = node_map.namedItem("id");
  if (node.isNull()) { // id element is NOT used in EPUB 2.0
    shared_creator->id = node.nodeValue();
  }
  node = node_map.namedItem("role");
  if (!node.isNull()) {
    shared_creator->role = node.nodeValue();
  }
  node = node_map.namedItem("file-as");
  if (!node.isNull()) {
    shared_creator->file_as = node.nodeValue();
  }
  shared_creator->name = metadata_element.text();
  m_metadata->creators.insert(shared_creator->name, shared_creator);
}

void EPubContainer::extractIdentifierMetadata(QDomElement metadata_element)
{
  QDomNamedNodeMap node_map = metadata_element.attributes();
  QDomNode node = node_map.namedItem("id");
  SharedIdentifier shared_identifier = SharedIdentifier(new EPubIdentifier());
  QString value = node.nodeValue();
  node = node_map.namedItem("id");
  if (!node.isNull()) {
    shared_identifier->uid = metadata_element.text();
    if (value == m_unique_identifier_name) {
      m_unique_identifier = shared_identifier->uid;
    }
  }
  m_metadata->identifiers.insert(shared_identifier->uid, shared_identifier);
}

void EPubContainer::extractLanguageMetadata(QDomElement metadata_element)
{
  QDomNamedNodeMap node_map = metadata_element.attributes();
  QDomNode node = node_map.namedItem("id");
  SharedLanguage shared_language = SharedLanguage(Q_NULLPTR);
  node = node_map.namedItem("id");
  if (!node.isNull()) {
    shared_language = SharedLanguage(new EPubLanguage());
    shared_language->language = metadata_element.text();
  }
  m_metadata->languages.insert(shared_language->language, shared_language);
}

void EPubContainer::extractDateModified(QDomNamedNodeMap node_map, QString text)
{
  QDateTime date_modified = QDateTime::fromString(text, Qt::ISODate);
  m_metadata->modified.date = date_modified;
  QDomNode node = node_map.namedItem("id");
  if (!node.isNull()) {
    m_metadata->modified.id = node.nodeValue();
  }
}

bool EPubContainer::parseManifestItem(const QDomNode& manifestNode,
                                      const QString currentFolder)
{
}

bool EPubContainer::parseSpineItem(const QDomNode& spineNode) {}

bool EPubContainer::parseGuideItem(const QDomNode& guideItem) {}

bool EPubContainer::parseBindingsItem(const QDomNode& bindingsItem) {}

EPubPageReference::StandardType
    EPubPageReference::typeFromString(const QString& name)
{
  if (name == "cover") {
    return CoverPage;
  } else if (name == "title-page") {
    return TitlePage;
  } else if (name == "toc") {
    return TableOfContents;
  } else if (name == "index") {
    return Index;
  } else if (name == "glossary") {
    return Glossary;
  } else if (name == "acknowledgements") {
    return Acknowledgements;
  } else if (name == "bibliography") {
    return Bibliography;
  } else if (name == "colophon") {
    return Colophon;
  } else if (name == "copyright-page") {
    return CopyrightPage;
  } else if (name == "dedication") {
    return Dedication;
  } else if (name == "epigraph") {
    return Epigraph;
  } else if (name == "foreword") {
    return Foreword;
  } else if (name == "loi") {
    return ListOfIllustrations;
  } else if (name == "lot") {
    return ListOfTables;
  } else if (name == "notes") {
    return Notes;
  } else if (name == "preface") {
    return Preface;
  } else if (name == "text") {
    return Text;
  } else {
    return Other;
  }
}

bool EPubContainer::saveContainer()
{
  //  m_archive->setCurrentFile(CONTAINER_FILE);
  //  QuaZipFile containerFile(m_archive);
  //  containerFile.setZip(m_archive);

  //  if (!containerFile.open(QIODevice::WriteOnly)) {
  //    int error = m_archive->getZipError();
  //    QLOG_DEBUG(tr("Unable to open container file error %1").arg(error));
  //    return false;
  //  }

  //  QByteArray container = m_container_document->toByteArray();
  //  qint64 size = containerFile.write(container);
  //  if (size != container.size()) {
  //    QLOG_DEBUG(tr("Unexpected container size %1 should be %2")
  //                   .arg(size)
  //                   .arg(container.size()));
  //    return false;
  //  }
  //  return true;
}
