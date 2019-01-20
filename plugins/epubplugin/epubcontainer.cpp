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

const QString EPubContainer::TOC_TITLE = "<h2>%1</h2>";
const QString EPubContainer::LIST_START = "<html><body><ul>";
const QString EPubContainer::LIST_END = "</ul></body></html>";
const QString EPubContainer::LIST_ITEM = "<li><a href=\"%1\">%2</li>";
const QString EPubContainer::LIST_BUILD_ITEM = "<li><a href=\"%1#%2\">%3</li>";
const QString EPubContainer::LIST_FILEPOS = "position%1";

EPubContainer::EPubContainer(QObject* parent)
  : QObject(parent)
  , m_archive(Q_NULLPTR) /*, m_toc(new EPubToc())*/
  , m_metadata(new EPubMetadata())
{
}

EPubContainer::~EPubContainer()
{
}

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

  return true;
}

bool EPubContainer::saveFile()
{
  m_archive = new QuaZip(m_filename);
  // TODO check if file exists and overwrite it if it does.

  if (!saveMimetype()) {
    return false;
  }
  if (!saveContainer()) {
    return false;
  }
  // TODO the rest

  m_archive->close();
}

// QByteArray EPubContainer::epubItem(const QString& id) const
//{
//  SharedManifestItem item = m_manifest.items.value(id);
//  if (item) {
//    m_archive->setCurrentFile(item->href);
//    QuaZipFile* file = new QuaZipFile(item->href);
//    file->setZip(m_archive);

//    if (!file->open(QIODevice::ReadOnly)) {
//      m_archive->getZipError();
//      QLOG_DEBUG(tr("Unable to open image file %1").arg(item->path));
//      return QByteArray();
//    }

//    QByteArray data = file->readAll();
//    return data;
//  }
//  return QByteArray();
//}

// QSharedPointer<QuaZipFile> EPubContainer::zipFile(const QString& path)
//{
//  m_archive->setCurrentFile(path);
//  QuaZipFile* file = new QuaZipFile(path);
//  file->setZip(m_archive);

//  if (!file->open(QIODevice::ReadOnly)) {
//    QLOG_DEBUG(tr("Unable to open file %1").arg(path));
//    return QSharedPointer<QuaZipFile>();
//  }

//  return QSharedPointer<QuaZipFile>(file);
//}

QImage EPubContainer::image(const QString& id, QSize image_size)
{
  QImage image;
  if (m_manifest.images.contains(id)) {
    image = m_manifest.images.value(id);

  } else if (m_manifest.rendered_svg_images.contains(id)) {
    image = m_manifest.rendered_svg_images.value(id);

  } else if (m_manifest.svg_images.contains(id)) {
    QSvgRenderer renderer(m_manifest.svg_images.value(id)->path);

    QSize svgSize(renderer.viewBox().size());
    if (svgSize.isValid()) {
      svgSize.scale(image_size, Qt::KeepAspectRatio);
    } else {
      svgSize = image_size;
    }

    image = QImage(svgSize, QImage::Format_ARGB32);
    QPainter painter(&image);
    renderer.render(&painter);
    painter.end();

    m_manifest.rendered_svg_images.insert(id, image);

  } else {
    QLOG_DEBUG(tr("Unable to find image file for id %1").arg(id));
    return QImage();
  }

  return image;
}

QStringList EPubContainer::itemKeys()
{
  return m_manifest.items.keys();
}

SharedManifestItem EPubContainer::item(QString key)
{
  return m_manifest.items.value(key);
}

QString EPubContainer::css(QString key)
{
  return m_manifest.css.value(key);
}

QString EPubContainer::javascript(QString key)
{
  return m_manifest.javascript.value(key);
}

QString EPubContainer::itemDocument(QString key)
{
  return item(key)->document_string;
}

// void setStartCursor(SharedTextCursor start) {
//  m_manif
//}

QStringList EPubContainer::spineKeys()
{
  return m_spine.ordered_items;
}

QStringList EPubContainer::imageKeys()
{
  return m_manifest.images.keys();
}

QStringList EPubContainer::cssKeys()
{
  return m_manifest.css.keys();
}

QStringList EPubContainer::jsKeys()
{
  return m_manifest.javascript.keys();
}

QString EPubContainer::tocAsString()
{
//  TocDisplayDocument* toc_document = new TocDisplayDocument(this);
//  toc_document->setHtml(m_manifest.formatted_toc_string);
//  return toc_document;
  return m_manifest.formatted_toc_string;
}

QStringList EPubContainer::creators()
{
  return m_metadata->creator_list;
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

    m_mimetype = mimetypeFile.readAll();
    if (m_mimetype != MIMETYPE) {
      QLOG_DEBUG(tr("Unexpected mimetype %1").arg(QString(m_mimetype)));
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

  qint64 size = mimetypeFile.write(m_mimetype);
  if (size != m_mimetype.size()) {
    QLOG_DEBUG(tr("Unexpected mimetype size %1 should be %2").arg(size).arg(m_mimetype.size()));
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
    QDomNodeList root_nodes = m_container_document->elementsByTagName("rootfile");
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

void EPubContainer::createChapterAnchorPoints(SharedSpineItem spine_item)
{
  SharedManifestItem manifest_item = m_manifest.items.value(spine_item->idref);
  if (manifest_item) {
    SharedTocItem toc_item = m_manifest.toc_paths.value(manifest_item->href);
    if (toc_item) {
      if (!toc_item->chapter_tag.isEmpty()) {
        // toc has a label added.
        QString tag("<a id=\"%1\"></a>");
        tag = tag.arg(toc_item->chapter_tag);
        QString document = manifest_item->document_string;
        QRegularExpression body_tag("<body[^>]*> ");
        QRegularExpressionMatch match = body_tag.match(document);
        if (match.hasMatch()) {
          //        int start = match.capturedStart();
          int end = match.capturedEnd();
          document.insert(end, tag);
        }
      }

      if (!toc_item->sub_items.isEmpty()) {
      }
    }
  }
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
  QDomNode node;
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
  QDomNodeList metadata_node_list = package_document->elementsByTagName("metadata");
  for (int i = 0; i < metadata_node_list.count(); i++) {
    QDomNodeList metadata_child_list = metadata_node_list.at(i).childNodes();
    for (int j = 0; j < metadata_child_list.count(); j++) {
      parseMetadataItem(metadata_child_list.at(j));
    }
  }

  // Extract current path, for resolving relative paths
  QString content_file_folder;
  int separator_index = full_path.lastIndexOf('/');
  if (separator_index > 0) {
    content_file_folder = full_path.left(separator_index + 1);
  }

  // Parse out all the components/items in the epub
  // should only have one manifest.
  QDomNodeList manifest_node_list = package_document->elementsByTagName("manifest");
  for (int i = 0; i < manifest_node_list.count(); i++) {
    QDomElement manifest_element = manifest_node_list.at(i).toElement();
    node_map = manifest_element.attributes();
    node = node_map.namedItem("id");
    if (!node.isNull()) {
      m_manifest.id = node.nodeValue();
    }
    QDomNodeList manifest_item_list = manifest_element.elementsByTagName("item");

    for (int j = 0; j < manifest_item_list.count(); j++) {
      parseManifestItem(manifest_item_list.at(j), content_file_folder);
    }
  }

  // Parse out the document guide
  // please note that this has been superceded by landmarks in EPUB 3.0
  QDomNodeList spine_node_list = package_document->elementsByTagName("spine");
  for (int i = 0; i < spine_node_list.count(); i++) {
    QDomElement spine_element = spine_node_list.at(i).toElement();
    SharedSpineItem spine_item = SharedSpineItem(new EPubSpineItem());
    node_map = spine_element.attributes();
    node = node_map.namedItem("id");
    if (!node.isNull()) { // optional
      m_spine.id = node.nodeValue();
    }
    node = node_map.namedItem("toc");
    if (!node.isNull()) { // optional
      m_spine.toc = node.nodeValue();
    }
    node = node_map.namedItem("page-progression-dir");
    if (!node.isNull()) { // optional
      m_spine.page_progression_dir = node.nodeValue();
    }

    QDomNodeList spineItemList = spine_element.elementsByTagName("itemref");
    for (int j = 0; j < spineItemList.count(); j++) {
      spine_item = parseSpineItem(spineItemList.at(j), spine_item);
    }

    if (!m_spine.toc.isEmpty()) { // EPUB2.0 toc
      parseToc();
    }

    /*
     * At this point not all books have anchors added for chapter links. Calibre
     * adds in anchors at the end of the previous chapter. For simplicitys sake I
     * am adding them at the start of the new chapter.
     * Also remember that some
     */
    createChapterAnchorPoints(spine_item);
  }

  //  QDomNodeList guide_node_list =
  //  package_document->elementsByTagName("guide"); for (int i = 0; i <
  //  guide_node_list.count(); i++) {
  //    QDomElement spine_element = guide_node_list.at(i).toElement();
  //    SharedGuideItem item = SharedGuideItem(new EPubGuideItem());
  //    node_map = spine_element.attributes();
  //    node = node_map.namedItem("id");
  //    if (!node.isNull()) { // optional
  //      m_spine.id = node.nodeValue();
  //    }
  //    node = node_map.namedItem("toc");
  //    if (!node.isNull()) { // optional
  //      m_spine.toc = node.nodeValue();
  //    }
  //    node = node_map.namedItem("page-progression-dir");
  //    if (!node.isNull()) { // optional
  //      m_spine.page_progression_dir = node.nodeValue();
  //    }

  //    QDomNodeList spineItemList = spine_element.elementsByTagName("itemref");
  //    for (int j = 0; j < spineItemList.count(); j++) {
  //      parseSpineItem(spineItemList.at(j));
  //    }
  //  }

  //    parseGuideItem(); // this has been superceded by landmarks.
  //  parseLandmarkItem();
  //  parseBindingsItem(); // Tis is used for non-epb standard media types.

  return true;
}

void EPubContainer::saveTitles(QDomElement metadata_element)
{
  QString id;
  SharedTitle shared_title;
  QDomElement elem;
  QStringList keys = m_metadata->titles.keys();
  if (keys.size() == 1) {
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
    for (int i = 0; i < keys.size(); i++) {
      QString key = keys.at(i);
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
        elem.setAttribute("display-seq", QString::number(shared_title->sequence));
        metadata_element.appendChild(elem);
      }
    }
  }
}

void EPubContainer::saveCreator(QDomElement metadata_element)
{
  QStringList keys = m_metadata->creators.keys();
  QString role;
  for (int i = 0; i < keys.size(); i++) {
    QString key = keys.at(i);
    SharedCreator shared_creator = m_metadata->creators.value(key);
    QDomElement elem = QDomElement();
    elem.setTagName("dc:creator");
    elem.setNodeValue(shared_creator->name);
    // ALL of these should have an role.
    role = "aut";
    if (!shared_creator->role.isEmpty()) {
      role = shared_creator->role;
    } else if (shared_creator->type) {
      role = EPubCreator::toCreatorString(shared_creator->type);
    } else {
      // TODO ?? maybe nothing
    }
    elem.setAttribute("id", role);
    metadata_element.appendChild(elem);

    elem = QDomElement();
    elem.setTagName("meta");
    elem.setAttribute("refines", "#" + role);
    elem.setAttribute("property", "role");
    elem.setAttribute("scheme", "marc:relators");
    elem.setNodeValue(role);
    metadata_element.appendChild(elem);

    if (!shared_creator->file_as.isEmpty()) {
      elem = QDomElement();
      elem.setTagName("meta");
      elem.setAttribute("refines", "#" + role);
      elem.setAttribute("property", "file-as");
      elem.setNodeValue(shared_creator->file_as);
      metadata_element.appendChild(elem);
    }

    if (!shared_creator->file_as.isEmpty()) {
      elem = QDomElement();
      elem.setTagName("meta");
      elem.setAttribute("refines", "#" + role);
      elem.setAttribute("property", "file-as");
      elem.setNodeValue(shared_creator->file_as);
      metadata_element.appendChild(elem);
    }
  }
}

void EPubContainer::saveIdentifier(QDomElement metadata_element)
{
  QStringList keys = m_metadata->identifiers.keys();
  QString role;
  for (int i = 0; i < keys.size(); i++) {
    QString key = keys.at(i);
    SharedIdentifier shared_identifier = m_metadata->identifiers.value(key);
    QDomElement elem = QDomElement();
    elem.setTagName("dc:identifier");
    elem.setAttribute("id", shared_identifier->name);
    elem.setNodeValue(shared_identifier->uid);
    // ALL of these should have an role.
    elem.setAttribute("id", shared_identifier->uid);
    metadata_element.appendChild(elem);
  }
}

bool EPubContainer::savePackageFile(QString& full_path)
{
  QDomDocument doc(full_path);
  // create the base package tags including optional elements.
  QDomElement root = doc.createElement("package");
  root.setAttribute("xmlns", "http://www.idpf.org/2007/opf");
  root.setAttribute("version", "3.0");
  if (!m_unique_identifier_name.isEmpty()) { // actually this is a required element
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
  SharedIdentifier unigue_identifier = m_metadata->identifiers.value(m_unique_identifier_name);
  elem.setTagName("dc:identifier");
  elem.setAttribute("id", m_unique_identifier_name);
  elem.setNodeValue(unigue_identifier->uid);
  metadata_element.appendChild(elem);

  saveTitles(metadata_element);
  saveCreator(metadata_element);
  saveIdentifier(metadata_element);
  // TODO - the rest of the saves.
  return true;
}

bool EPubContainer::parseMetadataItem(const QDomNode& metadata_node)
{
  QDomElement metadata_element = metadata_node.toElement();
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
              shared_title->type = EPubTitle::fromString(metadata_element.text());
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
                shared_creator->type = EPubCreator::fromCreatorString(metadata_element.text());
                if (shared_creator->role.isEmpty()) {
                  shared_creator->role = EPubCreator::toCreatorString(shared_creator->type);
                }
                if (shared_creator->type == EPubCreator::string_creator_type) {
                  shared_creator->string_creator = metadata_element.text();
                  QLOG_DEBUG(
                    QString("An unexpected role has come up. %1").arg(metadata_element.text()))
                }
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
          parseDateModified(node_map, metadata_element.text());
        } else if (name == "dcterms:source") {
          if (m_metadata->source.isEmpty()) {
            m_metadata->source = metadata_element.text();
          } else {
            // Not certain whether multi sources are allowed?
            QLOG_DEBUG(QString("Problem setting document source : %1 when %2 already set.")
                       .arg(metadata_element.text().arg(m_metadata->source)));
          }
        }
      }
    }
  } else if (metadata_element.prefix() == "dc" || metadata_element.prefix() == "opf") {
    if (tag_name == "creator") {
      parseCreatorMetadata(metadata_element);
    } else if (tag_name == "title") {
      parseTitleMetadata(metadata_element);
    } else if (tag_name == "identifier") {
      parseIdentifierMetadata(metadata_element);
    } else if (tag_name == "language") {
      parseLanguageMetadata(metadata_element);
    } else if (tag_name == "description") {
      m_metadata->description = metadata_element.text();
    } else if (tag_name == "publisher") {
      m_metadata->publisher = metadata_element.text();
    } else if (tag_name == "date") {
      m_metadata->date = QDateTime::fromString(metadata_element.text(), Qt::ISODate);
      QDomNode node = node_map.namedItem("id");
      if (!node.isNull()) {
        m_metadata->modified.id = node.nodeValue();
      }
    } else if (tag_name == "rights") {
      m_metadata->rights = metadata_element.text();
    } else if (tag_name == "contributor") {
      // TODO
    } else if (tag_name == "subject") {
      parseSubjectMetadata(metadata_element);
    } else {
      // TODO
    }
  } else {
    // TODO not a dc element, maybe calibre?
  }

  return true;
}

void EPubContainer::parseTitleMetadata(QDomElement metadata_element)
{
  QDomNamedNodeMap node_map = metadata_element.attributes();
  QDomNode node = node_map.namedItem("id");
  SharedTitle shared_title = SharedTitle(new EPubTitle());
  node = node_map.namedItem("id");
  if (!node.isNull()) { // id element is NOT used in EPUB 2.0
    shared_title->id = node.nodeValue();
  }
  // TODO optional lang and dir attributes.
  shared_title->title = metadata_element.text();
  m_metadata->titles.insert(shared_title->id, shared_title);
}

void EPubContainer::parseCreatorMetadata(QDomElement metadata_element)
{
  QDomNamedNodeMap node_map = metadata_element.attributes();
  SharedCreator shared_creator = SharedCreator(new EPubCreator());
  QDomNode node = node_map.namedItem("id");
  if (!node.isNull()) { // id element is NOT used in EPUB 2.0
    shared_creator->id = node.nodeValue();
  }
  /* Not certain if anybody is using the role information. The EPUB people
   * seem a bit ambivalent about it as well so for the time being I am
   * going to leave it as text and not use it for anything. Also if you
   * look at marc:role the list is enormous, most of which are irrelevant
   * to an ebook.
   */
  node = node_map.namedItem("role");
  if (!node.isNull()) {
    shared_creator->role = node.nodeValue();
    shared_creator->type = EPubCreator::fromCreatorString(node.nodeValue());
    if (shared_creator->type == EPubCreator::string_creator_type) {
      shared_creator->string_creator = node.nodeValue();
      QLOG_DEBUG(QString("An unexpected role has come up. %1").arg(node.nodeValue()))
    }
  }
  node = node_map.namedItem("file-as");
  if (!node.isNull()) {
    shared_creator->file_as = node.nodeValue();
  }
  shared_creator->name = metadata_element.text();
  m_metadata->creators.insert(shared_creator->id, shared_creator);
  m_metadata->creator_list.append(shared_creator->name);
}

void EPubContainer::parseIdentifierMetadata(QDomElement metadata_element)
{
  QDomNamedNodeMap node_map = metadata_element.attributes();
  SharedIdentifier shared_identifier = SharedIdentifier(new EPubIdentifier());
  QDomNode node = node_map.namedItem("id");
  if (!node.isNull()) {
    QString id_value = node.nodeValue();
    shared_identifier->name = id_value;
    shared_identifier->uid = metadata_element.text();
    if (id_value == m_unique_identifier_name) {
      m_unique_identifier = shared_identifier->uid;
    }
  }
  m_metadata->identifiers.insert(shared_identifier->uid, shared_identifier);
}

void EPubContainer::parseLanguageMetadata(QDomElement metadata_element)
{
  QDomNamedNodeMap node_map = metadata_element.attributes();
  SharedLanguage shared_language = SharedLanguage(new EPubLanguage());
  QDomNode node = node_map.namedItem("id");
  if (!node.isNull()) {
    shared_language->id = node.nodeValue();
  }
  shared_language->language = metadata_element.text();
  m_metadata->languages.insert(shared_language->id, shared_language);
}

void EPubContainer::parseSubjectMetadata(QDomElement metadata_element)
{
  QDomNamedNodeMap node_map = metadata_element.attributes();
  QDomNode node = node_map.namedItem("id");
  SharedSubject shared_subject = SharedSubject(new EPubSubject());
  node = node_map.namedItem("id");
  if (!node.isNull()) {
    shared_subject->id = node.nodeValue();
  }
  shared_subject->subject = metadata_element.text();
  m_metadata->subjects.insert(shared_subject->subject, shared_subject);
}

void EPubContainer::parseDateModified(QDomNamedNodeMap node_map, QString text)
{
  QDateTime date_modified = QDateTime::fromString(text, Qt::ISODate);
  m_metadata->modified.date = date_modified;
  QDomNode node = node_map.namedItem("id");
  if (!node.isNull()) {
    m_metadata->modified.id = node.nodeValue();
  }
  if (!m_metadata->date.isValid()) {
    // if no document release date is set then set it to the modified value.
    // this will be overridden if a dc:date meta node is found later.
    m_metadata->date = m_metadata->modified.date;
  }
}

bool EPubContainer::parseManifestItem(const QDomNode& manifest_node, const QString current_folder)
{
  QDomElement metadata_element = manifest_node.toElement();
  QString tag_name = metadata_element.tagName();
  QDomNamedNodeMap node_map = metadata_element.attributes();
  QDomNode node;
  QString name, value;

  if (tag_name == "item") {
    SharedManifestItem item = SharedManifestItem(new EPubManifestItem());
    node = node_map.namedItem("href");
    if (!node.isNull()) {
      value = node.nodeValue();
      QString path = QDir::cleanPath(current_folder + value);
      item->href = value;
      item->path = path;
    } else {
      QLOG_DEBUG(tr("Warning invalid manifest item : no href value"))
    }

    node = node_map.namedItem("id");
    if (!node.isNull()) {
      value = node.nodeValue();
      item->id = value;
    } else {
      QLOG_DEBUG(tr("Warning invalid manifest item : no id value"))
    }

    node = node_map.namedItem("media-type");
    if (!node.isNull()) {
      value = node.nodeValue();
      item->media_type = value.toLatin1();
      if (item->media_type == "image/gif" || item->media_type == "image/jpeg" ||
          item->media_type == "image/png") {

        if (!QImageReader::supportedMimeTypes().contains(item->media_type)) {
          QLOG_DEBUG(QString("Requested image type %1 is an unsupported type")
                     .arg(QString(item->media_type)));
        }

        m_archive->setCurrentFile(item->path);
        QuaZipFile image_file(m_archive);
        image_file.setZip(m_archive);

        if (!image_file.open(QIODevice::ReadOnly)) {
          m_archive->getZipError();
          QLOG_DEBUG(tr("Unable to open image file %1").arg(item->path));
        }

        QByteArray data = image_file.readAll();
        QImage image = QImage::fromData(data);
        m_manifest.images.insert(item->id, image);

      } else if (item->media_type == "application/vnd.ms-opentype" ||
                 item->media_type == "application/font-woff") {
        m_manifest.fonts.insert(item->id, item);

      } else if (item->media_type == "application/xhtml+xml") {
        m_archive->setCurrentFile(item->path);
        QuaZipFile itemFile(m_archive);
        itemFile.setZip(m_archive);

        if (!itemFile.open(QIODevice::ReadOnly)) {
          int error = m_archive->getZipError();
          QLOG_DEBUG(tr("Unable to open container file error %1").arg(error));
          return false;
        }

        QString container(itemFile.readAll());
        item->document_string = container;
        m_manifest.html_items.append(item);
      } else if (item->media_type == "text/css") {
        m_archive->setCurrentFile(item->path);
        QuaZipFile itemFile(m_archive);
        itemFile.setZip(m_archive);

        if (!itemFile.open(QIODevice::ReadOnly)) {
          int error = m_archive->getZipError();
          QLOG_DEBUG(tr("Unable to open css file error %1").arg(error));
          return false;
        }

        QString css_string(itemFile.readAll());
        css_string.replace("@charset \"", "@charset\"");
        m_manifest.css.insert(item->id, css_string);

      } else if (item->media_type == "text/javascript") {
        m_archive->setCurrentFile(item->path);
        QuaZipFile itemFile(m_archive);
        itemFile.setZip(m_archive);

        if (!itemFile.open(QIODevice::ReadOnly)) {
          int error = m_archive->getZipError();
          QLOG_DEBUG(tr("Unable to open javascript file error %1").arg(error));
          return false;
        }

        QString js_string(itemFile.readAll());
        m_manifest.javascript.insert(item->id, js_string);
      }
    } else {
      QLOG_DEBUG(tr("Warning invalid manifest item : no media-type value"))
    }

    node = node_map.namedItem("properties");
    if (!node.isNull()) {
      name = node.nodeName();
      value = node.nodeValue();
      // space separated list
      QStringList properties = value.split(' ', QString::SkipEmptyParts);
      item->properties = properties;

      foreach (QString prop, properties) {
        if (prop == "cover-image") {
          // only one cover-image allowed.
          m_manifest.cover_image = item;
          //          m_manifest.images.insert(item->id, item);
        } else if (prop == "nav") {
          // only one nav allowed.
          m_manifest.nav = item;
        } else if (prop == "svg") {
          m_manifest.svg_images.insert(item->id, item);
          //          m_manifest.images.insert(item->id, item);
        } else if (prop == "switch") {
          m_manifest.switches.insert(item->id, item);
        } else if (prop == "mathml") {
          m_manifest.mathml.insert(item->id, item);
        } else if (prop == "remote-resources") {
          m_manifest.remotes.insert(item->id, item);
        } else if (prop == "scripted") {
          m_manifest.scripted.insert(item->id, item);
        } else {
          // one of the exmples had a data-nav element which is NOT standard.
          // not certain what to do with these.
          item->non_standard_properties.insert(name, value);
        }
      }
    }

    node = node_map.namedItem("fallback");
    if (!node.isNull()) {
      value = node.nodeValue();
      item->fallback = value;
    }

    node = node_map.namedItem("media-overlay");
    if (!node.isNull()) {
      value = node.nodeValue();
      item->media_overlay = value;
      m_manifest.media_overlay.insert(item->id, item);
    }

    m_manifest.items.insert(item->id, item);
  }
  return true;
}

SharedSpineItem EPubContainer::parseSpineItem(const QDomNode& spine_node, SharedSpineItem item)
{
  QDomElement metadata_element = spine_node.toElement();
  QString tag_name = metadata_element.tagName();
  QDomNamedNodeMap node_map = metadata_element.attributes();
  QDomNode node;
  QString name, value;

  if (tag_name == "itemref") {

    // TODO EPUB2 toc element - convert to EPUB3

    node = node_map.namedItem("idref");
    if (!node.isNull()) {
      value = node.nodeValue();
      item->idref = value;
    } else {
      QLOG_DEBUG(tr("Warning invalid manifest itemref : no idref value"))
    }

    node = node_map.namedItem("id");
    if (!node.isNull()) {
      value = node.nodeValue();
      item->id = value;
    }

    node = node_map.namedItem("linear");
    if (!node.isNull()) {
      value = node.nodeValue();
      if (value == "yes" || value == "no") {
        if (value == "yes")
          item->linear = true; // false by default.
      } else {
        QLOG_DEBUG(tr("Warning invalid manifest itemref : linear MUST be "
                      "either yes or no not %1")
                   .arg(value))
      }
    }

    node = node_map.namedItem("properties");
    if (!node.isNull()) {
      name = node.nodeName();
      value = node.nodeValue();
      // space separated list
      QStringList properties = value.split(' ', QString::SkipEmptyParts);

      foreach (QString prop, properties) {
        if (prop == "page-spread-left") {
          item->page_spread_left = true;
        } else if (prop == "page-spread-right") {
          item->page_spread_right = true;
        }
      }
    }

    m_spine.items.insert(item->idref, item);
    m_spine.ordered_items.append(item->idref);
  }
  return item;
}

bool EPubContainer::saveSpineItem()
{
  // TODO save spine manfest section
}

SharedTocItem EPubContainer::parseNavPoint(QDomElement navpoint, QString& formatted_toc_data)
{
  m_toc_chapter_index++;
  SharedTocItem toc_item = SharedTocItem(new EPubTocItem());
  //  QDomNamedNodeMap attributes = navpoint.attributes();
  QString value = navpoint.attribute("class");
  if (!value.isEmpty()) {
    toc_item->tag_class = value;
  } else {
    //            QLOG_DEBUG(tr("Warning invalid manifest itemref : no
    //            idref value"))
  }

  value = navpoint.attribute("id");
  if (!value.isEmpty()) {
    toc_item->id = value;
  } else {
    //            QLOG_DEBUG(tr("Warning invalid manifest itemref : no
    //            idref value"))
  }

  value = navpoint.attribute("playOrder");
  if (!value.isEmpty()) {
    toc_item->playorder = value.toInt();
  } else {
    //            QLOG_DEBUG(tr("Warning invalid manifest itemref : no
    //            idref value"))
  }

  QDomElement navlabel = navpoint.firstChildElement("navLabel");
  if (!navlabel.isNull()) {
    QDomElement text = navlabel.firstChild().toElement();
    if (text.tagName() == "text") {
      toc_item->label = text.text();
    }
  }

  QDomElement content = navpoint.firstChildElement("content");
  value = content.attribute("src");
  if (!value.isEmpty()) {
    // if no chapter fragment then add one in.
    int index = value.indexOf("#");
    if (index <= 0) {
      toc_item->source = value;
      toc_item->chapter_tag.clear();
    } else {
      QString tag;
      tag = QString("#part%1").arg(m_toc_chapter_index);
      value += tag;
      toc_item->chapter_tag = tag;
    }
  } else {
    //            QLOG_DEBUG(tr("Warning invalid manifest itemref : no
    //            idref value"))
  }

  formatted_toc_data += LIST_ITEM.arg(toc_item->source).arg(toc_item->label);

  // parse nested navPoints.
  QDomElement sub_navpoint = navpoint.firstChildElement("navPoint");
  if (!sub_navpoint.isNull()) {
    formatted_toc_data += LIST_START;

    while (!sub_navpoint.isNull()) {
      SharedTocItem sub_item = parseNavPoint(sub_navpoint, formatted_toc_data);

      toc_item->sub_items.insert(sub_item->playorder, sub_item);
      sub_navpoint = sub_navpoint.nextSiblingElement("navPoint");
    }

    formatted_toc_data += LIST_END;
  }

  return toc_item;
}

void EPubContainer::handleSubNavpoints(QDomElement elem, QString& formatted_toc_string)
{
  QDomElement subpoint = elem.firstChildElement("navPoint");
  while (!subpoint.isNull()) {
    SharedTocItem toc_item = parseNavPoint(subpoint, formatted_toc_string);
    m_manifest.toc_items.insert(toc_item->playorder, toc_item);
    m_manifest.toc_paths.insert(toc_item->source, toc_item);

    handleSubNavpoints(subpoint, formatted_toc_string);

    subpoint = elem.nextSiblingElement("navPoint");
  }
}

QString EPubContainer::extractTagText(int anchor_start, QString document_string)
{
  int i = anchor_start;
  bool in_dquotes = false, in_squotes = false, in_tag = false, in_close_tag = false;
  QChar first;
  QString text;
  while (i < document_string.length()) {
    first = document_string.at(i);
    if (first == '<') {
      if (!in_dquotes && !in_squotes) {
        in_tag = true;
      }
    } else if (first == '>') {
      if (!in_dquotes && !in_squotes) {
        if (in_tag) {
          in_tag = false;
        } else if (in_close_tag) {
          in_close_tag = false;
        }
      }
    } else if (first == '\"') {
      if (in_dquotes)
        in_dquotes = false;
      else {
        in_dquotes = true;
      }
    } else if (first == '\'') {
      if (in_squotes)
        in_squotes = false;
      else {
        in_squotes = true;
      }
    } else if (first == '/') {
      if (in_tag) {
        in_close_tag = true;
        in_tag = false;
      }
    } else {
      if (!in_tag && !in_close_tag) {
        // store everything if not in_quotes and not in_tag
        text += first;
      }
      if (in_close_tag) {
        if (first == 'a') { // end of the anchor tag.
          break;
        }
      }
    }
    i++;
  }
  return text;
}

QString EPubContainer::buildTocfromHtml()
{
  QString formatted_toc_string = LIST_START;

  QRegularExpression re_anchor_complete("<a[\\s]+([^>]+)>((?:.(?!\\<\\/a\\>))*.)</a>");
  QRegularExpression re_anchor_tag("<a[^>]*>");
  QRegularExpression re_href("href=\\\"[^\"]*\"");
  int anchor_start, pos = 0;

  foreach (SharedManifestItem item, m_manifest.html_items) {
    QString document_string = item->document_string;
    if (!document_string.isEmpty()) {
      QRegularExpressionMatchIterator i = re_anchor_complete.globalMatch(document_string);
      QRegularExpressionMatch anchor_complete_match, anchor_tag_match, href_match;
      QString anchor_complete, anchor_tag, href_attr;

      while (i.hasNext()) {
        anchor_complete_match = i.next();
        anchor_complete = anchor_complete_match.captured(0);
        anchor_start = anchor_complete_match.capturedStart(0); // start of open anchor tag.

        anchor_tag_match = re_anchor_tag.match(anchor_complete);
        if (anchor_tag_match.hasMatch()) {
          anchor_tag = anchor_tag_match.captured(0);
          anchor_start += anchor_tag_match.capturedLength(0); // end of open anchor tag

          QString text = extractTagText(anchor_start, document_string);

          href_match = re_href.match(anchor_tag);
          if (href_match.hasMatch()) {
            href_attr = href_match.captured(0);
            href_attr = href_attr.mid(6, href_attr.length() - 7);
            QStringList splits = href_attr.split("#", QString::KeepEmptyParts);
            if (splits.length() == 1) {
              QString filename = splits.at(0);
              SharedManifestItemList files = m_manifest.html_items;
              foreach (SharedManifestItem item, files) {
                QString href = item->href;
                if (href == filename) {
                  // TODO add anchor & make anchor tag.
                }
              }
            } else if (!splits.at(0).isEmpty() && !splits.at(1).isEmpty()) {
              // existing file + anchor points exist.
              formatted_toc_string += LIST_BUILD_ITEM.arg(splits.at(0)).arg(splits.at(1)).arg(text);
            } else if (!splits.at(0).isEmpty() && splits.at(1).isEmpty()) {
              // existing file but no anchor point.
              QString pos_tag = LIST_FILEPOS.arg(pos++);
              formatted_toc_string += LIST_BUILD_ITEM.arg(splits.at(0)).arg(pos_tag).arg(text);
              // TODO introduce anchor tag
            } else if (splits.at(0).isEmpty() && !splits.at(1).isEmpty()) {
              // existing anchor tag but no file.
              // TODO find file and add to anchor.
            } else {
              // TODO no existing anchor point ??? not certain we ever get here.
              // may need to move this outside last regex match.
            }
          }
        }
      }
    }
  }

  formatted_toc_string += LIST_END;
  return formatted_toc_string;
}

bool EPubContainer::parseToc()
{
  QString toc_id = m_spine.toc;
  SharedManifestItem toc_item = m_manifest.items.value(toc_id);
  QString toc_path = toc_item->path;

  m_archive->setCurrentFile(toc_path);
  QuaZipFile toc_file(m_archive);
  toc_file.setZip(m_archive);

  if (!toc_file.open(QIODevice::ReadOnly)) {
    m_archive->getZipError();
    QLOG_DEBUG(tr("Unable to open toc file %1").arg(toc_path));
  }

  QByteArray data = toc_file.readAll();
  SharedDomDocument document = SharedDomDocument(new QDomDocument());
  document->setContent(data);
  QString formatted_toc_string;
  QDomElement root = document->documentElement();
  QDomNodeList node_list = document->elementsByTagName("docTitle");

  if (!node_list.isEmpty()) {
    QDomNode title_node = node_list.at(0);
    QDomElement title_text = title_node.firstChild().toElement();
    if (title_text.tagName() == "text") {
      formatted_toc_string += TOC_TITLE.arg(title_text.text());
    }
  }
  formatted_toc_string += LIST_START;

  m_toc_chapter_index = -1;
  node_list = document->elementsByTagName("navMap");
  if (!node_list.isEmpty()) {
    QString name, value;
    QDomNode node = node_list.at(0);
    QDomElement elem = node.toElement();
    QDomElement navpoint = elem.firstChildElement("navPoint");
    while (!navpoint.isNull()) {
      SharedTocItem toc_item = parseNavPoint(navpoint, formatted_toc_string);
      m_manifest.toc_items.insert(toc_item->playorder, toc_item);
      m_manifest.toc_paths.insert(toc_item->source, toc_item);

      handleSubNavpoints(navpoint, formatted_toc_string);

      navpoint = navpoint.nextSiblingElement("navPoint");
    }
  }

  formatted_toc_string += LIST_END;

  m_manifest.formatted_toc_string = formatted_toc_string;
}

bool EPubContainer::parseGuideItem(const QDomNode& guideItem)
{
  // TODO this has been superceded by the landmark in EPUB 3.0
}

bool EPubContainer::parseLandmarksItem(const QDomNode& guideItem)
{
  // TODO load landmarks
}

bool EPubContainer::saveLandmarksItem()
{
  // TODO save landmarks
}

bool EPubContainer::parseBindingsItem(const QDomNode& bindingsItem)
{
}

bool EPubContainer::saveBindingsItem()
{
  // TODO save bindings.
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
