#include "epubdocument_p.h"

#include <csvsplitter/csvsplitter.h>
#include <qlogger/qlogger.h>
using namespace qlogger;

const QString EPubDocumentPrivate::METADATA_FOLDER = "META-INF";
const QString EPubDocumentPrivate::MIMETYPE_FILE = "mimetype";
const QByteArray EPubDocumentPrivate::MIMETYPE = "application/epub+zip";
const QString EPubDocumentPrivate::CONTAINER_FILE = "META-INF/container.xml";
const QString EPubDocumentPrivate::TOC_FILE = "toc.ncx";

const QString EPubDocumentPrivate::TITLE = "title";
const QString EPubDocumentPrivate::CREATOR = "creator";
const QString EPubDocumentPrivate::IDENTIFIER = "identifier";
const QString EPubDocumentPrivate::LANGUAGE = "language";

const QString EPubDocumentPrivate::TOC_TITLE = "<h2>%1</h2>";
const QString EPubDocumentPrivate::LIST_START = "<html><body><ul>";
const QString EPubDocumentPrivate::LIST_END = "</ul></body></html>";
const QString EPubDocumentPrivate::LIST_ITEM = "<li><a href=\"%1\">%2</li>";
const QString EPubDocumentPrivate::LIST_BUILD_ITEM =
  "<li><a href=\"%1#%2\">%3</li>";
const QString EPubDocumentPrivate::LIST_FILEPOS = "position%1";
const QString EPubDocumentPrivate::HTML_DOCTYPE =
  "<!DOCTYPE html PUBLIC "
  "\"-//W3C//DTD XHTML 1.1//EN\" "
  "\"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">";
const QString EPubDocumentPrivate::XML_HEADER =
  "<?xml version='1.0' encoding='utf-8'?>";
const QString EPubDocumentPrivate::HTML_XMLNS = "http://www.w3.org/1999/xhtml";

EPubDocumentPrivate::EPubDocumentPrivate(EPubDocument* parent)
  : q_ptr(parent)
  , m_loaded(false)
{
  m_metadata = Metadata(new EBookMetadata());
  m_parser = new HtmlParser(q_ptr);
}

EPubDocumentPrivate::~EPubDocumentPrivate() {}

bool
EPubDocumentPrivate::loaded()
{
  return m_loaded;
}

void
EPubDocumentPrivate::openDocument(const QString& path)
{
  Q_Q(EPubDocument);
  m_filename = path;
  loadDocument();
}

void
EPubDocumentPrivate::closeDocument(const QString& path)
{
  if (m_modified) {
    m_filename = path;
    saveDocument();
  }
}

QString
EPubDocumentPrivate::filename()
{
  //  Q_Q(EPubDocument);
  return m_filename;
}

QString
EPubDocumentPrivate::buildTocFromFiles()
{
  return buildTocfromHtml();
}

QString
EPubDocumentPrivate::buildTocfromHtml()
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

QString
EPubDocumentPrivate::extractTagText(int anchor_start, QString document_string)
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

bool
EPubDocumentPrivate::isModified() const
{
  return m_modified;
}

bool
EPubDocumentPrivate::loadDocument()
{
  Q_Q(EPubDocument);

  // open the epub as a zip file
  m_archive = new QuaZip(q->filename());
  m_filename = q->filename(); // stored against modification;
  if (!m_archive->open(QuaZip::mdUnzip)) {
    QLOG_DEBUG(q->tr("Failed to open %1").arg(q->filename()));
    return false;
  }

  // get list of filenames from zip file
  m_files = m_archive->getFileNameList();
  if (m_files.isEmpty()) {
    QLOG_DEBUG(q->tr("Failed to read %1").arg(q->filename()));
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

  foreach (SharedManifestItem item, m_manifest.html_items) {
    QString doc_string = item->document_string;
    if (m_parser->parse(item->id, doc_string)) {
      // TODO use data?
    }
  }

  emit q->loadCompleted();
  return true;
}

bool
EPubDocumentPrivate::saveDocument()
{
  QFileInfo info;
  if (m_filename.isEmpty())
    info = QFileInfo(m_filename);
  else
    info = QFileInfo(m_filename);
  QString path = info.path();
  QString name = info.fileName();
  //  path = path + QDir::separator() + "temp";
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
  m_filename = m_filename;

  temp_file.close();

  // TODO backup/rename etc.
  // TODO remove temp file/dir - maybe on application close/cleanup

  return true;
}

bool
EPubDocumentPrivate::parseMimetype()
{
  Q_Q(EPubDocument);

  if (m_files.contains(MIMETYPE_FILE)) {
    m_archive->setCurrentFile(MIMETYPE_FILE);
    QuaZipFile mimetypeFile(m_archive);

    if (!mimetypeFile.open(QIODevice::ReadOnly)) {
      int error = m_archive->getZipError();
      QLOG_DEBUG(q->tr("Unable to open mimetype file : error %1").arg(error));
      return false;
    }

    m_mimetype = mimetypeFile.readAll();
    if (m_mimetype != MIMETYPE) {
      QLOG_DEBUG(q->tr("Unexpected mimetype %1").arg(QString(m_mimetype)));
    }
  } else {
    QLOG_DEBUG(q->tr("Unable to find mimetype in file"));
    return false;
  }
  return true;
}

bool
EPubDocumentPrivate::writeMimetype(QuaZip* save_zip)
{
  Q_Q(EPubDocument);

  QuaZipFile mimetype_file(save_zip);

  if (!mimetype_file.open(QIODevice::WriteOnly,
                          QuaZipNewInfo(MIMETYPE_FILE, MIMETYPE_FILE))) {
    int error = save_zip->getZipError();
    QLOG_DEBUG(q->tr("Unable to write mimetype file : error %1").arg(error));
    return false;
  }

  qint64 size = mimetype_file.write(MIMETYPE);
  if (size != m_mimetype.size()) {
    QLOG_DEBUG(q->tr("Unexpected mimetype size %1 should be %2")
                 .arg(size)
                 .arg(m_mimetype.size()));
    return false;
  }
  return true;
}

bool
EPubDocumentPrivate::parseContainer()
{
  Q_Q(EPubDocument);

  if (m_files.contains(CONTAINER_FILE)) {
    m_archive->setCurrentFile(CONTAINER_FILE);
    QuaZipFile containerFile(m_archive);
    containerFile.setZip(m_archive);

    if (!containerFile.open(QIODevice::ReadOnly)) {
      int error = m_archive->getZipError();
      QLOG_DEBUG(q->tr("Unable to open container file error %1").arg(error));
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
          QLOG_WARN(q->tr("Invalid root file entry"));
          continue;
        }
        if (parsePackageFile(m_container_fullpath)) {
          return true;
        }
      }
    }

  } else {
    QLOG_DEBUG(q->tr("Unable to find container information"));
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

  QLOG_DEBUG(q->tr("Unable to find and use any content files"));
  return false;
}

bool
EPubDocumentPrivate::writeContainer(QuaZip* save_zip)
{
  Q_Q(EPubDocument);

  QuaZipFile container_file(save_zip);

  if (!container_file.open(QIODevice::WriteOnly,
                           QuaZipNewInfo(CONTAINER_FILE, CONTAINER_FILE))) {
    int error = save_zip->getZipError();
    QLOG_DEBUG(q->tr("Unable to write container file : error %1").arg(error));
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

  xml_writer.writeEndDocument();
  container_file.close();

  writePackageFile(save_zip);

  foreach (SharedManifestItem item, m_manifest.html_items) {
    QuaZipFile item_file(save_zip);
    item_file.setFileName(item->path);

    if (!item_file.open(QIODevice::WriteOnly,
                        QuaZipNewInfo(item->path, item->path))) {
      int error = save_zip->getZipError();
      QLOG_DEBUG(
        q->tr("Unable to write html/xhtml file : error %1").arg(error));
      return false;
    }

    /* We have to use QTextStream rather than QXmlStreamWriter because the xml
     * stream escapes '<', '>' and several other characters. */
    QTextStream out(&item_file);
    out << QStringLiteral("<html xmlns=\"http://www.w3.org/1999/xhtml\">\n");
    out << HTML_DOCTYPE << "\n";
    out << QStringLiteral("<head>\n");
    out << QStringLiteral("<title>");
    Title shared_title = m_metadata->orderedTitles().first();
    if (shared_title) {
      out << shared_title->title;
    }
    out << QStringLiteral("</title>\n");
    out << QStringLiteral("<meta http-equiv=\"Content-Type\" "
                          "content=\"text/html; charset=utf-8\"/>\n");
    foreach (QString href, item->css_links) {
      out << QString(
               "<link href=\"%1\" rel=\"stylesheet\" type=\"text/css\"/>\n")
               .arg(href);
    }
    out << QStringLiteral("</head>\n");
    out << QStringLiteral("<body");
    if (!item->body_class.isEmpty()) {
      out << QString(" class=\"%1\">\n").arg(item->body_class);
    } else {
      out << QStringLiteral(">\n");
    }
    out << item->document_string;
    out << QStringLiteral("</body>\n");
    out << QStringLiteral("</html>\n");
    out.flush();

    item_file.close();
  }
  return true;
}

bool
EPubDocumentPrivate::parsePackageFile(QString& full_path)
{
  Q_Q(EPubDocument);

  m_archive->setCurrentFile(full_path);
  QuaZipFile contentFile(m_archive);
  contentFile.setZip(m_archive);

  if (!contentFile.open(QIODevice::ReadOnly)) {
    QLOG_DEBUG(q->tr("Malformed content file, unable to get content metadata"));
    return false;
  }

  QMap<QString, DomDocument> map;
  m_current_rootfile = map;
  m_rootfiles.insert(full_path, m_current_rootfile);

  QString content(contentFile.readAll());

  DomDocument package_document(new QDomDocument());
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
      m_metadata->setUniqueIdentifierName(value);
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
      m_metadata->setIsFoaf(true);
    }
  }

  // parse metadata.
  QDomNodeList metadata_node_list =
    package_document->elementsByTagName("metadata");
  m_metadata->parse(metadata_node_list);

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

bool
EPubDocumentPrivate::writePackageFile(QuaZip* save_zip)
{
  Q_Q(EPubDocument);

  QuaZipFile package_file(save_zip);

  if (!package_file.open(
        QIODevice::WriteOnly,
        QuaZipNewInfo(m_container_fullpath, m_container_fullpath))) {
    int error = save_zip->getZipError();
    QLOG_DEBUG(q->tr("Unable to write container file : error %1").arg(error));
    return false;
  }

  QXmlStreamWriter xml_writer(&package_file);
  xml_writer.setAutoFormatting(true);
  xml_writer.writeStartDocument("1.0");

  xml_writer.writeStartElement("package");
  xml_writer.writeAttribute("version", "3.0");
  xml_writer.writeAttribute("unique-identifier",
                            m_metadata->uniqueIdentifierName());
  xml_writer.writeAttribute("xmlns", m_package_xmlns);
  if (m_metadata->isFoaf()) {
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

  m_metadata->write(&xml_writer);

  xml_writer.writeEndElement();
  xml_writer.writeEndDocument();

  package_file.close();

  // TODO - the rest of the saves.
  return true;
}

bool
EPubDocumentPrivate::parseManifestItem(const QDomNode& manifest_node,
                                       const QString current_folder)
{
  Q_Q(EPubDocument);

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
      QLOG_DEBUG(q->tr("Warning invalid manifest item : no href value"))
    }

    node = node_map.namedItem("id");
    if (!node.isNull()) {
      value = node.nodeValue();
      item->id = value;
    } else {
      QLOG_DEBUG(q->tr("Warning invalid manifest item : no id value"))
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
          QLOG_DEBUG(q->tr("Unable to open image file %1").arg(item->path));
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
          QLOG_DEBUG(
            q->tr("Unable to open container file error %1").arg(error));
          return false;
        }

        QString container(itemFile.readAll());
        // remove xml header string. This will be reinserted by QXmlStreamWriter
        int length = container.trimmed().length();
        if (container.trimmed().startsWith(XML_HEADER)) {
          length -= XML_HEADER.length();
          container = container.right(length).trimmed();
        }
        // Remove DOCTYPE if exists. Again this will be reinserted later.
        if (container.trimmed().startsWith(HTML_DOCTYPE)) {
          length = container.length();
          length -= HTML_DOCTYPE.length();
          container = container.right(length).trimmed();
        }

        extractHeadInformationFromHtmlFile(item, container);

        QString in_body;
        QRegularExpression regex("<body[^>]*>((.|[\n\r])*)<\\/body>");
        QRegularExpressionMatch match = regex.match(container);
        int start, end;
        if (match.isValid()) {
          in_body = match.captured(0);
          regex = QRegularExpression("<body[^>]*>");
          match = regex.match(in_body);
          if (match.isValid()) {
            start = match.capturedEnd(0);
            regex = QRegularExpression("<\\/body>");
            match = regex.match(in_body);
            if (match.isValid()) {
              end = match.capturedStart(0);
              in_body = in_body.mid(start, end - start);
            }
          }
        }

        item->document_string = in_body.trimmed();
        m_manifest.html_items.append(item);
      } else if (item->media_type == "text/css") {
        m_archive->setCurrentFile(item->path);
        QuaZipFile itemFile(m_archive);
        itemFile.setZip(m_archive);

        if (!itemFile.open(QIODevice::ReadOnly)) {
          int error = m_archive->getZipError();
          QLOG_DEBUG(q->tr("Unable to open css file error %1").arg(error));
          return false;
        }

        QString css_string(itemFile.readAll());
        css_string.replace("@charset \"", "@charset\"");
        m_manifest.css.insert(item->href, css_string);

      } else if (item->media_type == "text/javascript") {
        m_archive->setCurrentFile(item->path);
        QuaZipFile itemFile(m_archive);
        itemFile.setZip(m_archive);

        if (!itemFile.open(QIODevice::ReadOnly)) {
          int error = m_archive->getZipError();
          QLOG_DEBUG(
            q->tr("Unable to open javascript file error %1").arg(error));
          return false;
        }

        QString js_string(itemFile.readAll());
        m_manifest.javascript.insert(item->id, js_string);
      }
    } else {
      QLOG_DEBUG(q->tr("Warning invalid manifest item : no media-type value"))
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

void
EPubDocumentPrivate::extractHeadInformationFromHtmlFile(SharedManifestItem item,
                                                        QString container)
{
  QDomDocument doc;
  doc.setContent(container);

  QDomNodeList node_list = doc.elementsByTagName(QLatin1String("head"));

  if (!node_list.isEmpty()) {
    QDomNode head_node = node_list.at(0); // should only be one head element
    QDomElement head_elem = head_node.toElement();
    QDomNodeList link_list = head_elem.elementsByTagName(QLatin1String("link"));
    for (int i = 0; i < link_list.length(); i++) {
      QDomNode link = link_list.at(i);
      QDomElement link_elem = link.toElement();
      QString rel = link_elem.attribute(QLatin1String("rel"));
      if (!rel.isEmpty()) {
        if (rel == QLatin1String("stylesheet")) {
          QString type = link_elem.attribute(QLatin1String("type"));
          if (!type.isEmpty()) {
            if (type == QLatin1String("text/css")) {
              QString href = link_elem.attribute(QLatin1String("href"));
              if (!href.isEmpty()) {
                item->css_links.append(href);
              }
            }
          }
        }
      }
    }
  }

  node_list = doc.elementsByTagName(QLatin1String("body"));

  if (!node_list.isEmpty()) {
    QDomNode body_node = node_list.item(0); // should only be one
    QDomElement body_elem = body_node.toElement();
    QString att = body_elem.attribute(QLatin1String("class"));
    if (!att.isEmpty()) {
      item->body_class = att;
    }
  }
}

SharedSpineItem
EPubDocumentPrivate::parseSpineItem(const QDomNode& spine_node,
                                    SharedSpineItem item)
{
  Q_Q(EPubDocument);

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
      QLOG_DEBUG(q->tr("Warning invalid manifest itemref : no idref value"))
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
        QLOG_DEBUG(q->tr("Warning invalid manifest itemref : linear MUST be "
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

bool
EPubDocumentPrivate::saveSpineItem()
{
  // TODO save spine manfest section
}

bool
EPubDocumentPrivate::parseBindingsItem(const QDomNode& bindingsItem)
{
  // TODO parse bindings
}

bool
EPubDocumentPrivate::saveBindingsItem()
{
  // TODO save bindings.
}

bool
EPubDocumentPrivate::parseTocFile()
{
  Q_Q(EPubDocument);

  QString toc_id = m_spine.toc;
  SharedManifestItem toc_item = m_manifest.items.value(toc_id);
  QString toc_path = toc_item->path;

  m_archive->setCurrentFile(toc_path);
  QuaZipFile toc_file(m_archive);
  toc_file.setZip(m_archive);

  if (!toc_file.open(QIODevice::ReadOnly)) {
    m_archive->getZipError();
    QLOG_DEBUG(q->tr("Unable to open toc file %1").arg(toc_path));
  }

  QByteArray data = toc_file.readAll();
  DomDocument document = DomDocument(new QDomDocument());
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

      handleNestedNavpoints(navpoint, formatted_toc_string);

      navpoint = navpoint.nextSiblingElement("navPoint");
    }
  }

  formatted_toc_string += LIST_END;

  m_manifest.formatted_toc_string = formatted_toc_string;
}

bool
EPubDocumentPrivate::parseGuideItem(const QDomNode& guideItem)
{
  // TODO this has been superceded by the landmark in EPUB 3.0
}

bool
EPubDocumentPrivate::parseLandmarksItem(const QDomNode& guideItem)
{
  // TODO load landmarks
}

bool
EPubDocumentPrivate::saveLandmarksItem()
{
  // TODO save landmarks
}

SharedTocItem
EPubDocumentPrivate::parseNavPoint(QDomElement navpoint,
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

void
EPubDocumentPrivate::handleNestedNavpoints(QDomElement elem,
                                           QString& formatted_toc_string)
{
  QDomElement subpoint = elem.firstChildElement("navPoint");
  while (!subpoint.isNull()) {
    SharedTocItem toc_item = parseNavPoint(subpoint, formatted_toc_string);
    m_manifest.toc_items.insert(toc_item->playorder, toc_item);
    m_manifest.toc_paths.insert(toc_item->source, toc_item);

    handleNestedNavpoints(subpoint, formatted_toc_string);

    subpoint = elem.nextSiblingElement("navPoint");
  }
}

QString
EPubDocumentPrivate::toc()
{
  return tocAsString();
}

QString
EPubDocumentPrivate::tocAsString()
{
  //  TocDisplayDocument* toc_document = new TocDisplayDocument(this);
  //  toc_document->setHtml(m_manifest.formatted_toc_string);
  //  return toc_document;
  return m_manifest.formatted_toc_string;
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

EPubContents*
EPubDocumentPrivate::cloneData()
{
  //  EPubContents* contents = new EPubContents();
  //  contents->m_loaded = m_loaded;

  //  return contents;
}

void
EPubDocumentPrivate::setClonedData(EPubContents* /*clone*/)
{
  //  Q_Q(EPubDocument);

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

QStringList
EPubDocumentPrivate::creators()
{
  return m_metadata->creatorList();
}

QString
EPubDocumentPrivate::title()
{
  Title first = m_metadata->orderedTitles().first();
  if (!first.isNull())
    return first->title;
  else {
    return QString();
  }
}

void
EPubDocumentPrivate::setTitle(QString title)
{
  Title first = m_metadata->orderedTitles().first();
  if (!first.isNull()) {
    first->title = title;
    m_modified = true;
  } else {
    first = Title(new EBookTitle());
    first->title = title;
    m_metadata->orderedTitles().insert(1, first);
    m_modified = true;
  }
}

QString
EPubDocumentPrivate::css(QString key)
{
  return m_manifest.css.value(key);
}

QString
EPubDocumentPrivate::javascript(QString key)
{
  return m_manifest.javascript.value(key);
}
/*!
 * \brief The ordered list of Spine keys.
 *
 * \return a QStringList of spine keys;
 */
QStringList
EPubDocumentPrivate::spineKeys()
{
  return m_spine.ordered_items;
}

QStringList
EPubDocumentPrivate::imageKeys()
{
  return m_manifest.images.keys();
}

QStringList
EPubDocumentPrivate::cssKeys()
{
  return m_manifest.css.keys();
}

QStringList
EPubDocumentPrivate::jsKeys()
{
  return m_manifest.javascript.keys();
}

QStringList
EPubDocumentPrivate::spine()
{
  return m_spine.ordered_items;
}

QMap<QString, QString>
EPubDocumentPrivate::pages()
{
  return m_parser->htmlDocumentsById();
}

Metadata
EPubDocumentPrivate::metadata()
{
  return m_metadata;
}
