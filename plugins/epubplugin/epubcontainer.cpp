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
  , m_archive(Q_NULLPTR)
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

SharedMetadata EPubContainer::metadata()
{
  return m_metadata;
}

EPubManifest EPubContainer::manifest()
{
  return m_manifest;
}

bool EPubContainer::parseMimetype()
{
  if (m_files.contains(MIMETYPE_FILE)) {
    m_archive->setCurrentFile(MIMETYPE_FILE);
    QuaZipFile mimetypeFile(m_archive);

    if (!mimetypeFile.open(QIODevice::ReadOnly)) {
      int error = m_archive->getZipError();
      QLOG_DEBUG(tr("Unable to open mimetype file : error %1").arg(error));
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
    QDomDocument container_document;
    container_document.setContent(container);
    QDomElement root_elem = container_document.documentElement();
    QDomNamedNodeMap att_map = root_elem.attributes();
    m_container_version = root_elem.attribute("version");
    m_container_xmlns = root_elem.attribute("xmlns");

    QDomNodeList root_files = root_elem.elementsByTagName("rootfiles");
    if (root_files.size() > 0) {
      QDomElement rootfiles_elem = root_files.at(0).toElement();
      QDomNodeList root_nodes = root_elem.elementsByTagName("rootfile");
      for (int i = 0; i < root_nodes.count(); i++) {
        QDomElement root_element = root_nodes.at(i).toElement();
        m_container_fullpath = root_element.attribute("full-path");
        m_container_mediatype = root_element.attribute("media-type");
        if (m_container_fullpath.isEmpty()) {
          QLOG_WARN(tr("Invalid root file entry"));
          continue;
        }
        if (parsePackageFile(m_container_fullpath)) {
          return true;
        }
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

// void EPubContainer::createChapterAnchorPoints(SharedSpineItem spine_item)
//{
//  SharedManifestItem manifest_item =
//  m_manifest.items.value(spine_item->idref); if (manifest_item) {
//    SharedTocItem toc_item = m_manifest.toc_paths.value(manifest_item->href);
//    if (toc_item) {
//      if (!toc_item->chapter_tag.isEmpty()) {
//        // toc has a label added.
//        QString tag("<a id=\"%1\"></a>");
//        tag = tag.arg(toc_item->chapter_tag);
//        QString document = manifest_item->document_string;
//        QRegularExpression body_tag("<body[^>]*> ");
//        QRegularExpressionMatch match = body_tag.match(document);
//        if (match.hasMatch()) {
//          //        int start = match.capturedStart();
//          int end = match.capturedEnd();
//          document.insert(end, tag);
//        }
//      }

//      if (!toc_item->sub_items.isEmpty()) {
//      }
//    }
//  }
//}

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
    QString value = node.nodeValue().toLower();
    // parse package attributes.
    if (name == "version") {
      if (value == "2.0") {
        m_version = 2;
      } else if (value == "3.0") {
        m_version = 3;
      }
    } else if (name == "xmlns") {
      m_package_xmlns = value;
    } else if (name == "unique-identifier") {
      m_package_unique_identifier_name = value;
    } else if (name == "xml:lang") {
      m_package_language = value;
    } else if (name == "prefix") { // Only 3.0
      // TODO - handle prefix mapping - may not need this, just store value.
      m_package_prefix = value;
    } else if (name == "dir") { // Only 3.0
      m_package_direction = value;
    } else if (name == "id") { // Only 3.0
      m_package_id = value;
    } else if (name == "prefix") {
      m_metadata->is_foaf = true;
    }
  }

  // parse metadata.
  QDomNodeList metadata_node_list =
    package_document->elementsByTagName("metadata");
  for (int i = 0; i < metadata_node_list.count(); i++) { // should only be one.
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
  QDomNodeList manifest_node_list =
    package_document->elementsByTagName("manifest");
  for (int i = 0; i < manifest_node_list.count(); i++) {
    QDomElement manifest_element = manifest_node_list.at(i).toElement();
    node_map = manifest_element.attributes();
    node = node_map.namedItem("id");
    if (!node.isNull()) {
      m_manifest.id = node.nodeValue();
    }
    QDomNodeList manifest_item_list =
      manifest_element.elementsByTagName("item");

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
      parseTocFile();
    }

    //    /*
    //     * At this point not all books have anchors added for chapter links.
    //     Calibre
    //     * adds in anchors at the end of the previous chapter. For
    //     simplicities sake I
    //     * am adding them at the start of the new chapter.
    //     * Also remember that some
    //     */
    //    createChapterAnchorPoints(spine_item);
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

int EPubContainer::getNextTitleIndex()
{
  int highest = 0;
  foreach (int i, m_metadata->ordered_titles.keys()) {
    if (i > highest)
      highest = i;
  }
  highest++;
  return highest;
}

void EPubContainer::parseSourceMetadata(const QDomElement& metadata_element)
{
  QDomNamedNodeMap node_map = metadata_element.attributes();
  QDomNode node = node_map.namedItem("id");
  SharedSource shared_source = SharedSource(new EPubSource());
  node = node_map.namedItem("id");
  if (!node.isNull()) { // id element is NOT used in EPUB 2.0
    shared_source->id = node.nodeValue();
  }
  node = node_map.namedItem("scheme");
  if (!node.isNull()) { // id element is NOT used in EPUB 2.0
    shared_source->scheme = EPubIdentifierScheme::fromString(node.nodeValue());
  }
  shared_source->source = metadata_element.text();
  m_metadata->source = shared_source;
}

void EPubContainer::parsePublisherMetadata(const QDomElement& metadata_element)
{
  QDomNamedNodeMap node_map = metadata_element.attributes();
  QDomNode node = node_map.namedItem("id");
  SharedPublisher shared_publisher = SharedPublisher(new EPubPublisher());
  node = node_map.namedItem("id");
  if (!node.isNull()) {
    shared_publisher->id = node.nodeValue();
  }
  node = node_map.namedItem("lang");
  if (!node.isNull()) {
    shared_publisher->lang = node.nodeValue();
  }
  node = node_map.namedItem("dir");
  if (!node.isNull()) {
    shared_publisher->dir = node.nodeValue();
  }
  node = node_map.namedItem("alt-rep");
  if (!node.isNull()) { // alt-rep element is NOT used in EPUB 2.0
    shared_publisher->alt_rep->alt_rep = node.nodeValue();
  }
  node = node_map.namedItem("alt-rep-lang");
  if (!node.isNull()) { // alt-rep-lang element is NOT used in EPUB 2.0
    shared_publisher->alt_rep->alt_rep_lang = node.nodeValue();
  }
  node = node_map.namedItem("file-as");
  if (!node.isNull()) { // file-as element is NOT used in EPUB 2.0
    shared_publisher->file_as->file_as = node.nodeValue();
  }
  shared_publisher->name = metadata_element.text();
  m_metadata->publisher = shared_publisher;
}

void EPubContainer::parseFormatMetadata(const QDomElement& metadata_element)
{
  SharedFormat shared_format = SharedFormat(new EPubFormat());
  QDomNamedNodeMap node_map = metadata_element.attributes();
  QDomNode node = node_map.namedItem("id");
  node = node_map.namedItem("id");
  if (!node.isNull()) { // id element is NOT used in EPUB 2.0
    shared_format->id = node.nodeValue();
  }
  shared_format->name = metadata_element.text();
  m_metadata->format = shared_format;
}

void EPubContainer::parseTypeMetadata(const QDomElement& metadata_element)
{
  SharedType shared_type = SharedType(new EPubType());
  QDomNamedNodeMap node_map = metadata_element.attributes();
  QDomNode node = node_map.namedItem("id");
  node = node_map.namedItem("id");
  if (!node.isNull()) { // id element is NOT used in EPUB 2.0
    shared_type->id = node.nodeValue();
  }
  shared_type->name = metadata_element.text();
  m_metadata->type = shared_type;
}

void EPubContainer::parseRelationMetadata(const QDomElement& metadata_element)
{
  SharedRelation shared_relation = SharedRelation(new EPubRelation());
  QDomNamedNodeMap node_map = metadata_element.attributes();
  QDomNode node = node_map.namedItem("id");
  node = node_map.namedItem("id");
  if (!node.isNull()) { // id element is NOT used in EPUB 2.0
    shared_relation->id = node.nodeValue();
  }
  node = node_map.namedItem("lang");
  if (!node.isNull()) {
    shared_relation->lang = node.nodeValue();
  }
  node = node_map.namedItem("dir");
  if (!node.isNull()) {
    shared_relation->dir = node.nodeValue();
  }
  shared_relation->name = metadata_element.text();
  m_metadata->relation = shared_relation;
}

void EPubContainer::parseCoverageMetadata(const QDomElement& metadata_element)
{
  SharedCoverage shared_coverage = SharedCoverage(new EPubCoverage());
  QDomNamedNodeMap node_map = metadata_element.attributes();
  QDomNode node = node_map.namedItem("id");
  node = node_map.namedItem("id");
  if (!node.isNull()) { // id element is NOT used in EPUB 2.0
    shared_coverage->id = node.nodeValue();
  }
  node = node_map.namedItem("lang");
  if (!node.isNull()) {
    shared_coverage->lang = node.nodeValue();
  }
  node = node_map.namedItem("dir");
  if (!node.isNull()) {
    shared_coverage->dir = node.nodeValue();
  }
  shared_coverage->name = metadata_element.text();
  m_metadata->coverage = shared_coverage;
}

void EPubContainer::parseRightsMetadata(const QDomElement& metadata_element)
{
  SharedRights shared_rights = SharedRights(new EPubRights());
  QDomNamedNodeMap node_map = metadata_element.attributes();
  QDomNode node = node_map.namedItem("id");
  node = node_map.namedItem("id");
  if (!node.isNull()) { // id element is NOT used in EPUB 2.0
    shared_rights->id = node.nodeValue();
  }
  node = node_map.namedItem("lang");
  if (!node.isNull()) {
    shared_rights->lang = node.nodeValue();
  }
  node = node_map.namedItem("dir");
  if (!node.isNull()) {
    shared_rights->dir = node.nodeValue();
  }
  shared_rights->name = metadata_element.text();
  m_metadata->rights = shared_rights;
}

void EPubContainer::parseDublinCoreMeta(QString tag_name,
                                        QDomElement& metadata_element,
                                        QDomNamedNodeMap& node_map)
{
  if (tag_name == "title") {
    parseTitleMetadata(metadata_element);
  } else if (tag_name == "creator") {
    parseCreatorMetadata(metadata_element);
  } else if (tag_name == "contributor") {
    parseContributorMetadata(metadata_element);
  } else if (tag_name == "description") {
    parseDescriptionMetadata(metadata_element);
  } else if (tag_name == "identifier") {
    parseIdentifierMetadata(metadata_element);
  } else if (tag_name == "language") {
    parseLanguageMetadata(metadata_element);
  } else if (tag_name == "description") {
    parseDescriptionMetadata(metadata_element);
  } else if (tag_name == "publisher") {
    parsePublisherMetadata(metadata_element);
  } else if (tag_name == "date") {
    QDomNode id_node = node_map.namedItem("id");
    m_metadata->date =
      QDateTime::fromString(metadata_element.text(), Qt::ISODate);
    if (!id_node.isNull()) {
      m_metadata->modified.id = id_node.nodeValue();
    }
  } else if (tag_name == "rights") {
    parseRightsMetadata(metadata_element);
  } else if (tag_name == "format") {
    parseFormatMetadata(metadata_element);
  } else if (tag_name == "relation") {
    parseRelationMetadata(metadata_element);
  } else if (tag_name == "coverage") {
    parseCoverageMetadata(metadata_element);
  } else if (tag_name == "source") {
    parseSourceMetadata(metadata_element);
  } else if (tag_name == "subject") {
    parseSubjectMetadata(metadata_element);
  } else if (tag_name == "type") {
    parseTypeMetadata(metadata_element);
  } else {
    // TODO
  }
}

void EPubContainer::parseOpfMeta(QString tag_name,
                                 QDomElement& metadata_element,
                                 QDomNamedNodeMap& /*node_map*/)
{
  QLOG_WARN(QString("Unknown OPF <meta> detected : TagName=%1, NodeName=%2, "
                    "NodeValue=%3, TagValue=%4")
            .arg(tag_name)
            .arg(metadata_element.nodeName())
            .arg(metadata_element.nodeValue())
            .arg(metadata_element.text()));
}

void EPubContainer::parseCalibreMetas(QString id, QDomNode& node)
{
  if (id == "calibre:series")
    m_metadata->calibre.series_name = node.nodeValue();
  else if (id == "calibre:series_index")
    m_metadata->calibre.series_index = node.nodeValue();
  else if (id == "calibre:title_sort")
    m_metadata->calibre.title_sort = node.nodeValue();
  else if (id == "calibre:author_link_map")
    m_metadata->calibre.author_link_map = node.nodeValue();
  else if (id == "calibre:timestamp")
    m_metadata->calibre.timestamp = node.nodeValue();
  else if (id == "calibre:rating")
    m_metadata->calibre.rating = node.nodeValue();
  else if (id == "calibre:publication_type")
    m_metadata->calibre.publication_type = node.nodeValue();
  else if (id == "calibre:user_metadata")
    m_metadata->calibre.user_metadata = node.nodeValue();
  else if (id == "calibre:user_categories")
    m_metadata->calibre.user_categories = node.nodeValue();
  else if (id == "calibre:custom_metadata")
    m_metadata->calibre.custom_metadata = node.nodeValue();
}

void EPubContainer::parseTitleDateRefines(SharedTitle shared_title,
    QDomElement& metadata_element)
{
  shared_title->date =
    QDateTime::fromString(metadata_element.text(), Qt::ISODate);
}

void EPubContainer::parseCreatorContributorRefines(
  SharedCreator shared_creator, QDomElement& metadata_element, QString id,
  QString property, QDomNamedNodeMap node_map)
{
  QDomNode node = node_map.namedItem("scheme");
  if (!node.isNull()) {
    QString scheme = node.nodeValue().toLower();
    if (scheme == "marc:relators") {
      //            shared_creator->scheme = id;
      shared_creator->relator =
        MarcRelator::fromString(metadata_element.text());
      if (shared_creator->relator.type() == MarcRelator::NO_TYPE) {
        shared_creator->string_creator = metadata_element.text();
        QLOG_DEBUG(QString("An unexpected role has come up. %1")
                   .arg(metadata_element.text()))
      }
    } else {
      // TODO treat as a string if not a recognised scheme type;
      shared_creator->string_scheme = metadata_element.text();
    }
  } else if (property == "alternate-script") {
    SharedAltRep alt_rep = SharedAltRep(new AltRep());
    alt_rep->alt_rep = metadata_element.text();
    node = node_map.namedItem("lang");
    if (!node.isNull()) {
      alt_rep->alt_rep_lang = node.nodeValue();
    }
    shared_creator->alt_rep_list.append(alt_rep);
  } else if (property == "file-as") {
    SharedFileAs file_as = SharedFileAs(new FileAs());
    file_as->file_as = node.nodeValue();
    node = node_map.namedItem("lang");
    if (!node.isNull()) {
      file_as->lang = node.nodeValue();
    }
    shared_creator->file_as_list.append(file_as);
  } else if (property.startsWith("foaf:")) {
    if (m_metadata->creators_by_id.contains(id)) {
      SharedCreator shared_creator = m_metadata->creators_by_id.value(id);
      shared_creator->foaf.append(Foaf::fromString(property));
    } else if (m_metadata->contributors_by_id.contains(id)) {
      SharedContributor shared_contributor =
        m_metadata->contributors_by_id.value(id);
      shared_contributor->foaf.append(Foaf::fromString(property));
    }
  }
}

void EPubContainer::parseRefineMetas(QDomElement& metadata_element,
                                     QDomNode& node, QDomNamedNodeMap& node_map)
{
  QString id = node.nodeValue().toLower();
  if (id.startsWith("#")) {
    id = id.right(id.length() - 1);
    if (m_metadata->titles_by_id.contains(id)) {
      // is it a title?
      SharedTitle shared_title = m_metadata->titles_by_id.value(id);
      node = node_map.namedItem("property");
      if (!node.isNull()) {
        QString property = node.nodeValue().toLower();
        if (property == "title-type") {
          // "title-type" is not used in 3.1 so ignore that node.
        } else if (property == "display-seq") {
          // refines/display-seq has been superceded in 3.1. Titles should
          // appear in the order required so reorder them in display-seq
          // order if these items appear in the metadata.
          int new_seq = metadata_element.text().trimmed().toInt();
          int old_seq = m_metadata->ordered_titles.key(shared_title);
          if (new_seq != old_seq) { // dont change if key is correct.
            if (m_metadata->ordered_titles.contains(
                  new_seq)) { // bugger key already exists
              SharedTitle existing_title =
                m_metadata->ordered_titles.value(new_seq);
              m_metadata->ordered_titles.remove(new_seq);
              m_metadata->ordered_titles.remove(old_seq);
              m_metadata->ordered_titles.insert(new_seq, shared_title);
              m_metadata->ordered_titles.insert(getNextTitleIndex(),
                                                existing_title);
            } else { // otherwise it is a free key
              m_metadata->ordered_titles.remove(old_seq);
              m_metadata->ordered_titles.insert(new_seq, shared_title);
            }
          }
        } else if (property == "alternate-script") {
          SharedAltRep alt_rep = SharedAltRep(new AltRep());
          alt_rep->alt_rep = metadata_element.text();
          node = node_map.namedItem("lang");
          if (!node.isNull()) {
            alt_rep->alt_rep_lang = node.nodeValue();
          }
          shared_title->alt_rep_list.append(alt_rep);
        } else if (property == "file-as") {
          SharedFileAs file_as = SharedFileAs(new FileAs());
          file_as->file_as = node.nodeValue();
          node = node_map.namedItem("lang");
          if (!node.isNull()) {
            file_as->lang = node.nodeValue();
          }
          shared_title->file_as_list.append(file_as);
        } else if (property.startsWith("dcterms:")) {
          if (property == "dcterms:date") {
            node = node_map.namedItem("scheme");
            parseTitleDateRefines(shared_title, metadata_element);
          }
        }
      }
    }
  } else if (m_metadata->languages.contains(id)) {
    // is it a language?
    SharedLanguage shared_language = m_metadata->languages.value(id);
    node = node_map.namedItem("property");
    if (!node.isNull()) {
      id = node.nodeValue().toLower();
      if (id == "language")
        shared_language->language = metadata_element.text();
    }
  } else if (m_metadata->creators_by_id.contains(id)) {
    // is it a creator
    SharedCreator shared_creator = m_metadata->creators_by_id.value(id);
    node = node_map.namedItem("property");
    if (!node.isNull()) {
      QString property = node.nodeValue().toLower();
      if (property == "role") {
        parseCreatorContributorRefines(shared_creator, metadata_element, id,
                                       property, node_map);
      }
    }
  } else if (m_metadata->contributors_by_name.contains(id)) {
    // is it a creator
    SharedCreator shared_contributor = m_metadata->contributors_by_name.value(id);
    node = node_map.namedItem("property");
    if (!node.isNull()) {
      QString property = node.nodeValue().toLower();
      if (property == "role") {
        parseCreatorContributorRefines(shared_contributor, metadata_element, id,
                                       property, node_map);
      }
    }
  }
}

bool EPubContainer::parseMetadataItem(const QDomNode& metadata_node)
{
  QDomElement metadata_element = metadata_node.toElement();
  QString tag_name = metadata_element.tagName();
  QDomNamedNodeMap node_map = metadata_element.attributes();
  QDomNode node;
  QString id;

  if (tag_name == "meta") {
    // refines are now deprecated however store their data and we will
    // try to convert them to 3.1
    node = node_map.namedItem("refines");
    if (!node.isNull()) { // # refines earler values title etc.
      parseRefineMetas(metadata_element, node, node_map);
      return true;
    }

    // metadata elements that do not refine other elements.
    node = node_map.namedItem("property");
    if (!node.isNull()) {
      id = node.nodeValue().toLower();
      DCTerms dcterms = DCTerms::fromString(id);
      if (dcterms.isDCTerm()) {
        if (dcterms.term() == DCTerms::DATE) {
          parseDateModified(node_map, metadata_element.text());
        } else if (dcterms.term() == DCTerms::SOURCE) {
          if (!m_metadata->source.isNull()) {
            m_metadata->source->source = metadata_element.text();
          } else {
            SharedSource shared_source = SharedSource(new EPubSource());
            shared_source->source = metadata_element.text();
            m_metadata->source = shared_source;
          }
        } else if (dcterms.term() == DCTerms::MODIFIED) {
          m_metadata->modified.date =
            QDateTime::fromString(metadata_element.text(), Qt::ISODate);
        } else if (dcterms.term() == DCTerms::IDENTIFIER) {
          // TODO
        } else if (dcterms.term() == DCTerms::RIGHTS) {
          if (!m_metadata->rights.isNull()) {
            m_metadata->rights->name = metadata_element.text();
          } else {
            SharedRights shared_rights = SharedRights(new EPubRights());
            shared_rights->name = metadata_element.text();
            m_metadata->rights = shared_rights;
          }
        } else if (dcterms.term() == DCTerms::SUBJECT) {
          // TODO
        } else if (dcterms.term() == DCTerms::CREATOR) {
          QString creator = metadata_element.text();
          QStringList keys = m_metadata->contributors_by_name.keys();
          if (!keys.contains(creator, Qt::CaseInsensitive)) {
            // This might cause a contributor to be duplicated if the spelling
            // is different. I think that that will have to be edited manually
            // by the user.
            SharedCreator shared_creator = SharedCreator(new EPubCreator());
            shared_creator->name = creator;
            m_metadata->creators_by_name.insert(creator, shared_creator);
          }
        } else if (dcterms.term() == DCTerms::CREATED) {
          // TODO
        } else if (dcterms.term() == DCTerms::LANGUAGE) {
          // TODO
        } else if (dcterms.term() == DCTerms::DESCRIPTION) {
          // TODO
        } else if (dcterms.term() == DCTerms::TITLE) {
          QString title = metadata_element.text();
          QStringList keys = m_metadata->titles_by_name.keys();
          if (keys.contains(title, Qt::CaseInsensitive)) {
            SharedTitle shared_title = SharedTitle(new EPubTitle());
            shared_title->title = title;
            m_metadata->titles_by_name.insert(title, shared_title);
          }
        } else if (dcterms.term() == DCTerms::PUBLISHER) {
          if (m_metadata->publisher.isNull()) {
            // TODO - maybe more than one publisher?
            m_metadata->publisher = SharedPublisher(new EPubPublisher());
            m_metadata->publisher->name = metadata_element.text();
          }
        } else if (dcterms.term() == DCTerms::CONTRIBUTOR) {
          QString contributor = metadata_element.text();
          QStringList keys = m_metadata->contributors_by_name.keys();
          if (!keys.contains(contributor, Qt::CaseInsensitive)) {
            // This might cause a contributor to be duplicated if the spelling
            // is different. I think that that will have to be edited manually
            // by the user.
            SharedContributor shared_contributor =
              SharedContributor(new EPubContributor());
            shared_contributor->name = contributor;
            m_metadata->contributors_by_name.insert(contributor,
                                                    shared_contributor);
          }
        } else if (dcterms.term() == DCTerms::DATE) {
        } else if (dcterms.term() == DCTerms::DATE_COPYRIGHTED) {
          // TODO
        } else if (dcterms.term() == DCTerms::DATE_ACCEPTED) {
          // TODO
        } else if (dcterms.term() == DCTerms::DATE_SUBMITTED) {
          // TODO
        } else if (dcterms.term() == DCTerms::IS_PART_OF) {
          // TODO
        } else if (dcterms.term() == DCTerms::ISSUED) {
          // TODO
        } else if (dcterms.term() == DCTerms::DATE) {
          m_metadata->date =
            QDateTime::fromString(metadata_element.text(), Qt::ISODate);
        } else if (dcterms.term() == DCTerms::LICENSE) {
          // TODO
        } else {
          // TODO other dcterms.
          QLOG_WARN(QString("Unknown DCTerms object : %1").arg(dcterms.code()));
        }
      }
    }

    // handle <meta> tags
    node = node_map.namedItem("name");
    if (!node.isNull()) {
      id = node.nodeValue().toLower();
      node = node_map.namedItem("content");
      if (!node.isNull()) {
        // Mostly calibre tags - maintain these
        if (id.startsWith("calibre:")) {
          parseCalibreMetas(id, node);
        } else // a store for unknown stuff.
          m_metadata->extra_metas.insert(id, node.nodeValue());
      }
    }
  } else if (metadata_element.prefix() == "dc") {
    parseDublinCoreMeta(tag_name, metadata_element, node_map);
  } else if (metadata_element.prefix() == "opf") {
    // might not actually be any of these.
    // just throws a log warning at the moment.
    parseOpfMeta(tag_name, metadata_element, node_map);
  } else {
    // TODO not a dc  or OPF element, maybe calibre?
    QLOG_WARN(QString("Unknown <meta> detected : TagName=%1, NodeName=%2, "
                      "NodeValue=%3, TagValue=%4")
              .arg(tag_name)
              .arg(metadata_element.nodeName())
              .arg(metadata_element.nodeValue())
              .arg(metadata_element.text()));
  }

  return true;
}

void EPubContainer::parseTitleMetadata(QDomElement metadata_element)
{
  SharedTitle shared_title = SharedTitle(new EPubTitle());
  QDomNamedNodeMap node_map = metadata_element.attributes();
  QDomNode node = node_map.namedItem("id");
  node = node_map.namedItem("id");
  if (!node.isNull()) { // id element is NOT used in EPUB 2.0
    shared_title->id = node.nodeValue();
  }
  node = node_map.namedItem("dir");
  if (!node.isNull()) { // dir element is NOT used in EPUB 2.0
    shared_title->dir = node.nodeValue();
  }
  node = node_map.namedItem("lang");
  if (!node.isNull()) { // lang element is NOT used in EPUB 2.0
    shared_title->lang = node.nodeValue();
  }
  node = node_map.namedItem("alt-rep");
  if (!node.isNull()) { // alt-rep element is NOT used in EPUB 2.0
    SharedAltRep shared_alt_rep = SharedAltRep(new AltRep());
    shared_alt_rep->alt_rep = node.nodeValue();
    node = node_map.namedItem("alt-rep-lang");
    if (!node.isNull()) { // alt-rep-lang element is NOT used in EPUB 2.0
      shared_alt_rep->alt_rep_lang = node.nodeValue();
    }
    shared_title->alt_rep_list.append(shared_alt_rep);
  }
  node = node_map.namedItem("file-as");
  if (!node.isNull()) { // file-as element is NOT used in EPUB 2.0
    SharedFileAs file_as = SharedFileAs(new FileAs());
    file_as->file_as = node.nodeValue();
    shared_title->file_as_list.append(file_as);
  }

  shared_title->title = metadata_element.text();
  m_metadata->titles_by_id.insert(shared_title->id, shared_title);
  m_metadata->titles_by_name.insert(shared_title->title, shared_title);
  m_metadata->ordered_titles.insert(getNextTitleIndex(), shared_title);
}

void EPubContainer::parseCreatorMetadata(QDomElement metadata_element)
{
  QDomNamedNodeMap node_map = metadata_element.attributes();
  SharedCreator shared_creator = SharedCreator(new EPubCreator());
  QDomNode node = node_map.namedItem("id");
  if (!node.isNull()) { //  EPUB 3.0 only
    shared_creator->id = node.nodeValue();
  }
  /* Not certain if anybody is using the role information. The EPUB people
   * seem a bit ambivalent about it as well so for the time being I am
   * going to leave it as text and not use it for anything. Also if you
   * look at marc:role the list is enormous, most of which are irrelevant
   * to an ebook.
   */
  node = node_map.namedItem("role"); // 2.0 & 3.0
  if (!node.isNull()) {
    shared_creator->relator = MarcRelator::fromString(node.nodeValue());
    if (shared_creator->relator.type() == MarcRelator::NO_TYPE) {
      shared_creator->string_creator = node.nodeValue();
      QLOG_DEBUG(
        QString("An unexpected role has come up. %1").arg(node.nodeValue()))
    }
  }
  node = node_map.namedItem("file-as"); // 2.0 & 3.0
  if (!node.isNull()) {
    SharedFileAs file_as = SharedFileAs(new FileAs());
    file_as->file_as = node.nodeValue();
    shared_creator->file_as_list.append(file_as);
  }
  node = node_map.namedItem("scheme"); // 3.0
  if (!node.isNull()) {
    QDomElement elem = node.toElement();
    if (!elem.isNull()) {
      QString scheme = elem.attribute("scheme");
      if (!scheme.isEmpty()) {
        if (scheme == "marc:relators") {
          if (shared_creator->relator.type() != MarcRelator::NO_TYPE) {
            MarcRelator relator =
              MarcRelator::fromString(metadata_element.text());
            if (relator.type() != MarcRelator::NO_TYPE) {
              shared_creator->relator = relator;
            }
          }
        }
      }
    }
  }
  node = node_map.namedItem("alt-rep"); // 3.0
  if (!node.isNull()) {
    SharedAltRep alt_rep = SharedAltRep(new AltRep());
    alt_rep->alt_rep = node.nodeValue();
    node = node_map.namedItem("alt-rep-lang"); // 3.0
    if (!node.isNull()) {
      alt_rep->alt_rep_lang = node.nodeValue();
    }
    shared_creator->alt_rep_list.append(alt_rep);
  }

  shared_creator->name = metadata_element.text();
  m_metadata->creators_by_id.insert(shared_creator->id, shared_creator);
  m_metadata->creators_by_name.insert(shared_creator->name, shared_creator);
  m_metadata->creator_list.append(shared_creator->name);
}

void EPubContainer::parseContributorMetadata(QDomElement metadata_element)
{
  /* Please note that according to the 3.0 spec Contributors data are
   * identical to Creators data. */
  QDomNamedNodeMap node_map = metadata_element.attributes();
  SharedContributor shared_contributor =
    SharedContributor(new EPubContributor());
  QDomNode node = node_map.namedItem("id");
  if (!node.isNull()) { //  EPUB 3.0 only
    shared_contributor->id = node.nodeValue();
  }

  node = node_map.namedItem("role"); // 2.0 & 3.0
  if (!node.isNull()) {
    shared_contributor->relator = MarcRelator::fromString(node.nodeValue());
    if (shared_contributor->relator.type() == MarcRelator::NO_TYPE) {
      shared_contributor->string_creator = node.nodeValue();
      QLOG_DEBUG(
        QString("An unexpected role has come up. %1").arg(node.nodeValue()))
    }
  }
  if (!node.isNull()) {
    SharedFileAs file_as = SharedFileAs(new FileAs());
    file_as->file_as = node.nodeValue();
    shared_contributor->file_as_list.append(file_as);
  }
  node = node_map.namedItem("scheme"); // 3.0
  if (!node.isNull()) {
    QDomElement elem = node.toElement();
    if (!elem.isNull()) {
      QString scheme = elem.attribute("scheme");
      if (!scheme.isEmpty()) {
        if (scheme == "marc:relators") {
          if (shared_contributor->relator.type() != MarcRelator::NO_TYPE) {
            MarcRelator relator =
              MarcRelator::fromString(metadata_element.text());
            if (relator.type() != MarcRelator::NO_TYPE) {
              shared_contributor->relator = relator;
            }
          }
        }
      }
    }
  }
  node = node_map.namedItem("alt-rep"); // 3.0
  if (!node.isNull()) {
    SharedAltRep alt_rep = SharedAltRep(new AltRep());
    alt_rep->alt_rep = node.nodeValue();
    node = node_map.namedItem("alt-rep-lang"); // 3.0
    if (!node.isNull()) {
      alt_rep->alt_rep_lang = node.nodeValue();
    }
    shared_contributor->alt_rep_list.append(alt_rep);
  }

  shared_contributor->name = metadata_element.text();
  if (!shared_contributor->id.isEmpty()) {
    m_metadata->contributors_by_id.insert(shared_contributor->id,
                                          shared_contributor);
  }
  m_metadata->contributors_by_name.insert(shared_contributor->name,
                                          shared_contributor);
  //  m_metadata->creator_list.append(shared_creator->name);
}

void EPubContainer::parseDescriptionMetadata(QDomElement metadata_element)
{
  /* Please note that according to the 3.0 spec Contributors data are
   * identical to Creators data. */
  QDomNamedNodeMap node_map = metadata_element.attributes();
  m_metadata->description = SharedDescription(new EPubDescription());
  QDomNode node = node_map.namedItem("id");
  if (!node.isNull()) { //  EPUB 3.0 only
    m_metadata->description->id = node.nodeValue();
  }
  node = node_map.namedItem("dir");
  if (!node.isNull()) { // dir element is NOT used in EPUB 2.0
    m_metadata->description->dir = node.nodeValue();
  }
  node = node_map.namedItem("lang");
  if (!node.isNull()) { // lang element is NOT used in EPUB 2.0
    m_metadata->description->language = node.nodeValue();
  }

  m_metadata->description->text = metadata_element.text();
}

void EPubContainer::parseIdentifierMetadata(QDomElement metadata_element)
{
  QDomNamedNodeMap node_map = metadata_element.attributes();
  SharedIdentifier shared_identifier = SharedIdentifier(new EPubIdentifier());
  QDomNode node = node_map.namedItem("id");
  if (!node.isNull()) { // EPUB 3.0
    QString id_value = node.nodeValue().toLower();
    shared_identifier->id = id_value;
    if (id_value == m_package_unique_identifier_name.toLower()) {
      m_package_unique_identifier = shared_identifier->id;
    }
  }
  node = node_map.namedItem("scheme");
  if (!node.isNull()) { // id element is NOT used in EPUB 2.0
    shared_identifier->identifier =
      EPubIdentifierScheme::fromString(node.nodeValue());
  }

  shared_identifier->name = metadata_element.text();

  m_metadata->identifiers.insert(shared_identifier->identifier.scheme,
                                 shared_identifier);
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
  node = node_map.namedItem("authority");
  if (!node.isNull()) {
    shared_subject->authority = node.nodeValue();
  }
  node = node_map.namedItem("term");
  if (!node.isNull()) {
    shared_subject->term = node.nodeValue();
  }
  node = node_map.namedItem("lang");
  if (!node.isNull()) {
    shared_subject->lang = node.nodeValue();
  }
  node = node_map.namedItem("dir");
  if (!node.isNull()) {
    shared_subject->dir = node.nodeValue();
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

bool EPubContainer::parseManifestItem(const QDomNode& manifest_node,
                                      const QString current_folder)
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
          //          m_archive->getZipError();
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

SharedSpineItem EPubContainer::parseSpineItem(const QDomNode& spine_node,
    SharedSpineItem item)
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

SharedTocItem EPubContainer::parseNavPoint(QDomElement navpoint,
    QString& formatted_toc_data)
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

void EPubContainer::handleSubNavpoints(QDomElement elem,
                                       QString& formatted_toc_string)
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
  bool in_dquotes = false, in_squotes = false, in_tag = false,
       in_close_tag = false;
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

  QRegularExpression re_anchor_complete(
    "<a[\\s]+([^>]+)>((?:.(?!\\<\\/a\\>))*.)</a>");
  QRegularExpression re_anchor_tag("<a[^>]*>");
  QRegularExpression re_href("href=\\\"[^\"]*\"");
  int anchor_start, pos = 0;

  foreach (SharedManifestItem item, m_manifest.html_items) {
    QString document_string = item->document_string;
    if (!document_string.isEmpty()) {
      QRegularExpressionMatchIterator i =
        re_anchor_complete.globalMatch(document_string);
      QRegularExpressionMatch anchor_complete_match, anchor_tag_match,
                              href_match;
      QString anchor_complete, anchor_tag, href_attr;

      while (i.hasNext()) {
        anchor_complete_match = i.next();
        anchor_complete = anchor_complete_match.captured(0);
        anchor_start =
          anchor_complete_match.capturedStart(0); // start of open anchor tag.

        anchor_tag_match = re_anchor_tag.match(anchor_complete);
        if (anchor_tag_match.hasMatch()) {
          anchor_tag = anchor_tag_match.captured(0);
          anchor_start +=
            anchor_tag_match.capturedLength(0); // end of open anchor tag

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
              formatted_toc_string +=
                LIST_BUILD_ITEM.arg(splits.at(0)).arg(splits.at(1)).arg(text);
            } else if (!splits.at(0).isEmpty() && splits.at(1).isEmpty()) {
              // existing file but no anchor point.
              QString pos_tag = LIST_FILEPOS.arg(pos++);
              formatted_toc_string +=
                LIST_BUILD_ITEM.arg(splits.at(0)).arg(pos_tag).arg(text);
              // TODO introduce anchor tag
            } else if (splits.at(0).isEmpty() && !splits.at(1).isEmpty()) {
              // existing anchor tag but no file.
              // TODO find file and add to anchor.
            } else {
              // TODO no existing anchor point ??? not certain we ever get
              // here. may need to move this outside last regex match.
            }
          }
        }
      }
    }
  }

  formatted_toc_string += LIST_END;
  return formatted_toc_string;
}

void EPubContainer::writeCreator(QXmlStreamWriter* xml_writer,
                                 SharedCreator shared_creator)
{
  writeCreatorContributor("dc:creator", xml_writer, shared_creator);
}

void EPubContainer::writeContributor(QXmlStreamWriter* xml_writer,
                                     SharedCreator shared_creator)
{
  writeCreatorContributor("dc:contributor", xml_writer, shared_creator);
}

bool EPubContainer::parseTocFile()
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

bool EPubContainer::saveFile()
{
  QFileInfo info(m_filename);
  QString path = info.path();
  QString name = info.fileName();
  path = path + QDir::separator() + "temp";
  QDir dir;
  dir.mkpath(path);

  QuaZip temp_file(path + QDir::separator() + name);
  if (temp_file.open(QuaZip::mdAdd)) {

    if (!writeMimetype(&temp_file)) {
      return false;
    }
    if (!writeContainer(&temp_file)) {
      return false;
    }
    // TODO the rest
  }

  temp_file.close();

  // TODO backup/rename etc.
  // TODO remove temp file/dir - maybe on application close/cleanup

  return true;
}

bool EPubContainer::writeMimetype(QuaZip* save_zip)
{
  QuaZipFile mimetype_file(save_zip);

  if (!mimetype_file.open(QIODevice::WriteOnly,
                          QuaZipNewInfo(MIMETYPE_FILE, MIMETYPE_FILE))) {
    int error = save_zip->getZipError();
    QLOG_DEBUG(tr("Unable to write mimetype file : error %1").arg(error));
    return false;
  }

  qint64 size = mimetype_file.write(MIMETYPE);
  if (size != m_mimetype.size()) {
    QLOG_DEBUG(tr("Unexpected mimetype size %1 should be %2")
               .arg(size)
               .arg(m_mimetype.size()));
    return false;
  }
  return true;
}

bool EPubContainer::writeContainer(QuaZip* save_zip)
{
  QuaZipFile container_file(save_zip);

  if (!container_file.open(QIODevice::WriteOnly,
                           QuaZipNewInfo(CONTAINER_FILE, CONTAINER_FILE))) {
    int error = save_zip->getZipError();
    QLOG_DEBUG(tr("Unable to write container file : error %1").arg(error));
    return false;
  }

  QXmlStreamWriter xml_writer(&container_file);
  xml_writer.setAutoFormatting(true);
  xml_writer.writeStartDocument("1.0");

  xml_writer.writeStartElement("container");
  xml_writer.writeAttribute("version", m_container_version);
  xml_writer.writeAttribute("xmlns", m_container_xmlns);

  xml_writer.writeStartElement("rootfiles");
  xml_writer.writeStartElement("rootfile");
  xml_writer.writeAttribute("full-path", m_container_fullpath);
  xml_writer.writeAttribute("media-type", m_container_mediatype);
  xml_writer.writeEndElement();

  xml_writer.writeEndElement();
  xml_writer.writeEndElement();

  container_file.close();

  writePackageFile(save_zip);
  return true;
}

bool EPubContainer::writePackageFile(QuaZip* save_zip)
{
  QuaZipFile package_file(save_zip);

  if (!package_file.open(
        QIODevice::WriteOnly,
        QuaZipNewInfo(m_container_fullpath, m_container_fullpath))) {
    int error = save_zip->getZipError();
    QLOG_DEBUG(tr("Unable to write container file : error %1").arg(error));
    return false;
  }

  QXmlStreamWriter xml_writer(&package_file);
  xml_writer.setAutoFormatting(true);
  xml_writer.writeStartDocument("1.0");

  xml_writer.writeStartElement("package");
  xml_writer.writeAttribute("version", "3.0");
  xml_writer.writeAttribute("unique-identifier",
                            m_package_unique_identifier_name);
  xml_writer.writeAttribute("xmlns", m_package_xmlns);
  if (m_metadata->is_foaf) {
    xml_writer.writeAttribute("prefix", Foaf::prefix());
  }
  if (!m_package_language.isEmpty()) { // Optional in EPUB 2.0
    xml_writer.writeAttribute("xml:lang", m_package_language);
  }
  if (!m_package_prefix.isEmpty()) {
    xml_writer.writeAttribute("prefix", m_package_prefix);
  }
  if (!m_package_direction.isEmpty()) {
    xml_writer.writeAttribute("dir", m_package_direction);
  }
  if (!m_package_id.isEmpty()) {
    xml_writer.writeAttribute("id", m_package_id);
  }

  writeMetadata(&xml_writer);

  xml_writer.writeEndElement();

  package_file.close();

  // TODO - the rest of the saves.
  return true;
}

void EPubContainer::writeIdAttribute(QXmlStreamWriter* xml_writer, QString id)
{
  if (!id.isEmpty()) { // 3.0.
    xml_writer->writeAttribute("id", id);
  }
}

void EPubContainer::writeDirAttribute(QXmlStreamWriter* xml_writer, QString dir)
{
  if (!dir.isEmpty()) { // 3.0.
    xml_writer->writeAttribute("dir", dir);
  }
}

void EPubContainer::writeLangAttribute(QXmlStreamWriter* xml_writer,
                                       QString lang)
{
  if (!lang.isEmpty()) { // 3.0.
    xml_writer->writeAttribute("xml:lang", lang);
  }
}

void EPubContainer::writeAuthorityAttribute(QXmlStreamWriter* xml_writer,
    QString authority, QString term)
{
  if (!authority.isEmpty()) { // 3.0.
    xml_writer->writeAttribute("opf:authority", authority);
  }

  if (!authority.isEmpty() && !term.isEmpty()) { // 2.0 and 3.0 with id set.
    xml_writer->writeAttribute("opf:term", term);
  }
}

void EPubContainer::writeAltRepAttribute(QXmlStreamWriter* xml_writer,
    QString alt_rep, QString alt_rep_lang)
{
  if (!alt_rep.isEmpty()) { // 3.0
    xml_writer->writeAttribute("opf:alt-rep", alt_rep);
  }
  if (!alt_rep_lang.isEmpty()) { // 3.0
    xml_writer->writeAttribute("opf:alt-rep-lang", alt_rep_lang);
  }
}

void EPubContainer::writeFileAsAttribute(QXmlStreamWriter* xml_writer,
    QString file_as, QString lang)
{
  if (!file_as.isEmpty()) { // 3.0
    xml_writer->writeAttribute("opf:file-as", file_as);
  }
  writeLangAttribute(xml_writer, lang);
}

QString EPubContainer::writeCreatorsMetadata(QXmlStreamWriter* xml_writer,
    QString key)
{
  return writeCreatorContibutor("dc:creator", xml_writer, key);
}

QString EPubContainer::writeContributorMetadata(QXmlStreamWriter* xml_writer,
    QString key)
{
  return writeCreatorContibutor("dc:contributor", xml_writer, key);
}

void EPubContainer::writeRoleAttribute(QXmlStreamWriter* xml_writer,
                                       MarcRelator relator)
{
  if (relator.type() != MarcRelator::NO_TYPE) {
    xml_writer->writeAttribute("opf:role", relator.code());
  }
}

void EPubContainer::writeSchemeAttribute(QXmlStreamWriter* xml_writer,
    EPubIdentifierScheme scheme)
{
  if (scheme.scheme != EPubIdentifierScheme::UNKNOWN_SCHEME) {
    xml_writer->writeAttribute("opf:scheme",
                               EPubIdentifierScheme::toString(scheme.scheme));
  }
}

void EPubContainer::writeCreatorContributor(QString tag_name,
    QXmlStreamWriter* xml_writer,
    SharedCreator shared_object)
{
  if (!shared_object.isNull()) {
    QString id = shared_object->id;
    QString name = shared_object->name;

    if (!shared_object->name.isEmpty()) {
      xml_writer->writeStartElement(tag_name);

      writeIdAttribute(xml_writer, shared_object->id);
      foreach (SharedFileAs file_as, shared_object->file_as_list) {
        writeFileAsAttribute(xml_writer, file_as->file_as, file_as->lang);
      }
      foreach (SharedAltRep alt_rep, shared_object->alt_rep_list) {
        writeAltRepAttribute(xml_writer, alt_rep->alt_rep,
                             alt_rep->alt_rep_lang);
      }
      writeRoleAttribute(xml_writer, shared_object->relator);

      xml_writer->writeCharacters(shared_object->name);
      xml_writer->writeEndElement();
    }
  }
}

QString EPubContainer::writeCreatorContibutor(QString tag_name,
    QXmlStreamWriter* xml_writer,
    QString key)
{
  // Basically contributors and creators are identical in form if not in
  // definition, contributors are lesser ctreators effectively.
  SharedCreator shared_object;
  QString name;

  // As EPubContributor is basically an unchanged EPubCreator we only need to
  // do this bit to make certain we get the correct data store.
  SharedCreator shared_creator =
    m_metadata->creators_by_id.value(key).dynamicCast<EPubCreator>();
  if (shared_creator) {
    shared_object = shared_creator;
  } else {
    SharedContributor shared_contributor =
      m_metadata->contributors_by_name.value(key)
      .dynamicCast<EPubContributor>();
    shared_object = shared_contributor;
  }

  writeCreatorContributor(tag_name, xml_writer, shared_object);

  return name;
}

void EPubContainer::writeLanguageMetadata(QXmlStreamWriter* xml_writer,
    QString key, bool first)
{
  /*
   * There must be at least one language element in 2.0 & 3.0.
   * refines metas will be ignored by a 2.0 reader so for backwards
   * compatability set the first language in both 2.0 & 3.0 form.
   * 2.0 and 3.0 with no id set.
   *  <dc:language>en-US</language>
   * 3.1 but readable by both as id will be ignored by 2.0.
   *  <dc:language id="lang1">en-US</language>
   */
  SharedLanguage shared_language = m_metadata->languages.value(key);
  if (!shared_language.isNull()) {
    QString id = shared_language->id;

    if (!shared_language->language.isEmpty()) {
      xml_writer->writeStartElement("dc:language");

      if (!id.isEmpty()) { // 2.0 and 3.0 with id set.
        xml_writer->writeAttribute("id", id);
        if (first) {
          xml_writer->writeCharacters(shared_language->language.trimmed());
        }
      } else { // 2.0 and 3.0 without id set
        xml_writer->writeCharacters(shared_language->language.trimmed());
      }
      xml_writer->writeEndElement();

      // refines is superceded in 3.1
      //      if (!id.isEmpty()) { // 3.0 with id set
      //        xml_writer->writeStartElement("meta");
      //        xml_writer->writeAttribute("refines", "#" + id);
      //        xml_writer->writeAttribute("property", "language");
      //        xml_writer->writeCharacters(shared_language->language.trimmed());
      //        xml_writer->writeEndElement();
      //      }
    }
  }
}

void EPubContainer::writeSubjectMetadata(QXmlStreamWriter* xml_writer,
    QString key)
{
  SharedSubject shared_subject = m_metadata->subjects.value(key);

  if (!shared_subject.isNull()) {
    if (!shared_subject->subject.isEmpty()) {
      xml_writer->writeStartElement("dc:subject");

      writeIdAttribute(xml_writer, shared_subject->id);
      writeDirAttribute(xml_writer, shared_subject->dir);
      writeLangAttribute(xml_writer, shared_subject->lang);
      writeAuthorityAttribute(xml_writer, shared_subject->authority,
                              shared_subject->term);

      xml_writer->writeCharacters(shared_subject->subject);
      xml_writer->writeEndElement();
    }
  }
}

void EPubContainer::writeIdentifierMetadata(
  QXmlStreamWriter* xml_writer, EPubIdentifierScheme::IdentifierScheme key)
{
  SharedIdentifier shared_identifier = m_metadata->identifiers.value(key);
  if (!shared_identifier->name.isEmpty()) {
    xml_writer->writeStartElement("dc:identifier");
    writeIdAttribute(xml_writer, shared_identifier->id);
    writeSchemeAttribute(xml_writer, shared_identifier->identifier);

    xml_writer->writeCharacters(shared_identifier->name);
    xml_writer->writeEndElement();
  }
}

void EPubContainer::writetitle(QXmlStreamWriter* xml_writer,
                               SharedTitle shared_title)
{
  QString id = shared_title->id;

  if (!shared_title->title.isEmpty()) {
    xml_writer->writeStartElement("dc:title");

    if (!id.isEmpty()) { // 3.0
      xml_writer->writeAttribute("id", id);
    }

    // these are optional attributes.
    writeDirAttribute(xml_writer, shared_title->dir);
    writeLangAttribute(xml_writer, shared_title->lang);
    SharedAltRepList alt_reps = shared_title->alt_rep_list;
    foreach (SharedAltRep alt_rep, alt_reps) {
      writeAltRepAttribute(xml_writer, alt_rep->alt_rep, alt_rep->alt_rep_lang);
    }
    SharedFileAsList file_as_list = shared_title->file_as_list;
    foreach (SharedFileAs file_as, file_as_list) {
      writeFileAsAttribute(xml_writer, file_as->file_as, file_as->lang);
    }

    xml_writer->writeCharacters(shared_title->title);
    xml_writer->writeEndElement();
  }
}

QString EPubContainer::writeTitleMetadata(QXmlStreamWriter* xml_writer, int key)
{
  SharedTitle shared_title = m_metadata->ordered_titles.value(key);
  QString title;
  if (!shared_title.isNull()) {
    title = shared_title->title;
    writetitle(xml_writer, shared_title);
  }
  return title;
}

void EPubContainer::writeDescriptionMetadata(QXmlStreamWriter* xml_writer)
{
  if (!m_metadata->description->text.isEmpty()) {
    xml_writer->writeStartElement("dc:description");

    writeIdAttribute(xml_writer, m_metadata->description->id);
    writeDirAttribute(xml_writer, m_metadata->description->dir);
    writeLangAttribute(xml_writer, m_metadata->description->language);

    xml_writer->writeCharacters(m_metadata->description->text);
    xml_writer->writeEndElement();
  }
}

void EPubContainer::writeSourceMetadata(QXmlStreamWriter* xml_writer)
{
  SharedSource shared_source = m_metadata->source;
  if (!shared_source->source.isEmpty()) {
    xml_writer->writeStartElement("dc:source");
    writeIdAttribute(xml_writer, shared_source->id);
    writeSchemeAttribute(xml_writer, shared_source->scheme);
    xml_writer->writeCharacters(shared_source->source);
    xml_writer->writeEndElement();
  }
}

void EPubContainer::writePublisherMetadata(QXmlStreamWriter* xml_writer)
{
  SharedPublisher shared_publisher = m_metadata->publisher;
  if (!shared_publisher->name.isEmpty()) {

    xml_writer->writeStartElement("dc:publisher");

    writeIdAttribute(xml_writer, shared_publisher->id);
    writeFileAsAttribute(xml_writer, shared_publisher->file_as->file_as,
                         shared_publisher->file_as->lang);
    writeDirAttribute(xml_writer, shared_publisher->dir);
    writeLangAttribute(xml_writer, shared_publisher->lang);
    writeAltRepAttribute(xml_writer, shared_publisher->alt_rep->alt_rep,
                         shared_publisher->alt_rep->alt_rep_lang);

    xml_writer->writeCharacters(shared_publisher->name);
    xml_writer->writeEndElement();
  }
}

void EPubContainer::writeFormatMetadata(QXmlStreamWriter* xml_writer)
{
  SharedFormat shared_format = m_metadata->format;
  if (!shared_format->name.isEmpty()) {
    xml_writer->writeStartElement("dc:format");

    writeIdAttribute(xml_writer, shared_format->id);

    xml_writer->writeCharacters(shared_format->name);
    xml_writer->writeEndElement();
  }
}

void EPubContainer::writeRelationMetadata(QXmlStreamWriter* xml_writer)
{
  SharedRelation shared_publisher = m_metadata->relation;
  if (!shared_publisher->name.isEmpty()) {
    xml_writer->writeStartElement("dc:relation");

    writeIdAttribute(xml_writer, shared_publisher->id);
    writeDirAttribute(xml_writer, shared_publisher->dir);
    writeLangAttribute(xml_writer, shared_publisher->lang);

    xml_writer->writeCharacters(shared_publisher->name);
    xml_writer->writeEndElement();
  }
}

void EPubContainer::writeRightsMetadata(QXmlStreamWriter* xml_writer)
{
  SharedRights shared_rights = m_metadata->rights;
  if (!shared_rights->name.isEmpty()) {
    xml_writer->writeStartElement("dc:rights");

    writeIdAttribute(xml_writer, shared_rights->id);
    writeDirAttribute(xml_writer, shared_rights->dir);
    writeLangAttribute(xml_writer, shared_rights->lang);

    xml_writer->writeCharacters(shared_rights->name);
    xml_writer->writeEndElement();
  }
}

void EPubContainer::writeCoverageMetadata(QXmlStreamWriter* xml_writer)
{
  SharedCoverage shared_coverage = m_metadata->coverage;
  if (!shared_coverage->name.isEmpty()) {
    xml_writer->writeStartElement("dc:coverage");

    writeIdAttribute(xml_writer, shared_coverage->id);
    writeDirAttribute(xml_writer, shared_coverage->dir);
    writeLangAttribute(xml_writer, shared_coverage->lang);

    xml_writer->writeCharacters(shared_coverage->name);
    xml_writer->writeEndElement();
  }
}

bool EPubContainer::writeMetadata(QXmlStreamWriter* xml_writer)
{

  xml_writer->writeStartElement("metadata");

  if (!metadata()->ordered_titles.isEmpty()) {
    QList<int> keys = m_metadata->ordered_titles.keys();
    // This may contain some extra titles added by dcterms:title
    QStringList named_titles = m_metadata->titles_by_name.keys();
    foreach (int key, keys) {
      QString title = writeTitleMetadata(xml_writer, key);
      named_titles.removeAll(title);
    }
    // These were probably added from dcterms:title metas.
    foreach (QString title, named_titles) {
      SharedTitle shared_title = m_metadata->titles_by_name.value(title);
      writetitle(xml_writer, shared_title);
    }
  }

  if (!m_metadata->creators_by_id.isEmpty()) {
    QStringList keys = m_metadata->creators_by_id.keys();
    QStringList names = m_metadata->creators_by_name.keys();
    foreach (QString key, keys) {
      QString name = writeCreatorsMetadata(xml_writer, key);
      // remove names that have been handled.
      names.removeAll(name);
    }
    // These were probably added from dcterms:creator metas.
    foreach (QString creator, names) {
      SharedCreator shared_creator =
        m_metadata->creators_by_name.value(creator);
      writeCreator(xml_writer, shared_creator);
    }
  }

  if (!m_metadata->contributors_by_name.isEmpty()) {
    QStringList keys = m_metadata->contributors_by_name.keys();
    foreach (QString key, keys) {
      writeContributorMetadata(xml_writer, key);
    }
  }

  if (!m_metadata->description.isNull()) {
    writeDescriptionMetadata(xml_writer);
  }

  // actually should NEVER be empty as at least one is required.
  if (!metadata()->languages.isEmpty()) {
    QStringList keys = m_metadata->languages.keys();
    if (keys.size() == 1) {
      writeLanguageMetadata(xml_writer, m_metadata->languages.keys().at(0),
                            true);
    } else {
      bool first = true;
      foreach (QString key, keys) {
        writeLanguageMetadata(xml_writer, key, first);
        first = false;
      }
    }
  }

  if (!m_metadata->subjects.isEmpty()) {
    QStringList keys = m_metadata->subjects.keys();
    foreach (QString key, keys) {
      writeSubjectMetadata(xml_writer, key);
    }
  }

  if (!m_metadata->identifiers.isEmpty()) {
    QList<EPubIdentifierScheme::IdentifierScheme> keys =
      m_metadata->identifiers.keys();
    foreach (EPubIdentifierScheme::IdentifierScheme key, keys) {
      writeIdentifierMetadata(xml_writer, key);
    }
  }

  if (!m_metadata->date.isNull()) {
    xml_writer->writeStartElement("date");
    xml_writer->writeCharacters(m_metadata->date.toString(Qt::ISODate));
    xml_writer->writeEndElement();
  }

  if (!m_metadata->source.isNull()) {
    writeSourceMetadata(xml_writer);
  }

  if (!m_metadata->publisher.isNull()) {
    writePublisherMetadata(xml_writer);
  }

  if (!m_metadata->format.isNull()) {
    writeFormatMetadata(xml_writer);
  }

  if (!m_metadata->relation.isNull()) {
    writeRelationMetadata(xml_writer);
  }

  if (!m_metadata->rights.isNull()) {
    writeRightsMetadata(xml_writer);
  }

  if (!m_metadata->coverage.isNull()) {
    writeCoverageMetadata(xml_writer);
  }

  xml_writer->writeEndElement();

  return true;
}
