#include "ebookmetadata.h"

#include <QXmlStreamWriter>

#include <qlogger/qlogger.h>

using namespace qlogger;

EBookMetadata::EBookMetadata()
  : m_calibre(Calibre(new EBookCalibre()))
  , m_is_foaf(false)
{}

void
EBookMetadata::parse(QDomNodeList metadata_node_list)
{
  for (int i = 0; i < metadata_node_list.count(); i++) { // should only be one.
    QDomNodeList metadata_child_list = metadata_node_list.at(i).childNodes();
    for (int j = 0; j < metadata_child_list.count(); j++) {
      parseMetadataItem(metadata_child_list.at(j));
    }
  }
}

bool
EBookMetadata::write(QXmlStreamWriter* xml_writer)
{

  xml_writer->writeStartElement("metadata");

  if (!m_ordered_titles.isEmpty()) {
    QList<int> keys = m_ordered_titles.keys();
    // This may contain some extra titles added by dcterms:title
    QStringList named_titles = m_titles_by_name.keys();
    foreach (int key, keys) {
      QString title = writeTitleMetadata(xml_writer, key);
      named_titles.removeAll(title);
    }
    // These were probably added from dcterms:title metas.
    foreach (QString title, named_titles) {
      Title shared_title = m_titles_by_name.value(title);
      writeTitle(xml_writer, shared_title);
    }
  }

  if (!m_creators_by_name.isEmpty()) {
    QStringList keys = m_creators_by_name.keys();
    QStringList ids = m_creators_by_id.keys();
    foreach (QString key, keys) {
      QString name = writeCreatorsMetadata(xml_writer, key);
      // remove names that have been handled.
      Creator shared_creator = m_creators_by_name.value(name);
      QString id = shared_creator->id;
      ids.removeAll(id);
    }
    // These were probably added from dcterms:creator metas.
    foreach (QString creator, ids) {
      Creator shared_creator = m_creators_by_id.value(creator);
      writeCreator(xml_writer, shared_creator);
    }
  }

  if (!contributors_by_name.isEmpty()) {
    QStringList keys = contributors_by_name.keys();
    foreach (QString key, keys) {
      writeContributorMetadata(xml_writer, key);
    }
  }

  if (!description.isNull()) {
    writeDescriptionMetadata(xml_writer);
  }

  // actually should NEVER be empty as at least one is required.
  if (!m_languages.isEmpty()) {
    QStringList keys = m_languages.keys();
    if (keys.size() == 1) {
      writeLanguageMetadata(xml_writer, m_languages.keys().at(0), true);
    } else {
      bool first = true;
      foreach (QString key, keys) {
        writeLanguageMetadata(xml_writer, key, first);
        first = false;
      }
    }
  }

  if (!subjects.isEmpty()) {
    QStringList keys = subjects.keys();
    foreach (QString key, keys) {
      writeSubjectMetadata(xml_writer, key);
    }
  }

  if (!m_identifiers.isEmpty()) {
    QList<EBookIdentifierScheme::IdentifierScheme> keys = m_identifiers.keys();
    foreach (EBookIdentifierScheme::IdentifierScheme key, keys) {
      writeIdentifierMetadata(xml_writer, key);
    }
  }

  if (!date.isNull()) {
    xml_writer->writeStartElement("date");
    xml_writer->writeCharacters(date.toString(Qt::ISODate));
    xml_writer->writeEndElement();
  }

  if (!source.isNull()) {
    writeSourceMetadata(xml_writer);
  }

  if (!publisher.isNull()) {
    writePublisherMetadata(xml_writer);
  }

  if (!format.isNull()) {
    writeFormatMetadata(xml_writer);
  }

  if (!relation.isNull()) {
    writeRelationMetadata(xml_writer);
  }

  if (!rights.isNull()) {
    writeRightsMetadata(xml_writer);
  }

  if (!coverage.isNull()) {
    writeCoverageMetadata(xml_writer);
  }

  xml_writer->writeEndElement();

  return true;
}

QString
EBookMetadata::uniqueIdentifierName() const
{
  return m_package_unique_identifier_name;
}

void
EBookMetadata::setUniqueIdentifierName(
  const QString& package_unique_identifier_name)
{
  m_package_unique_identifier_name = package_unique_identifier_name;
}

bool
EBookMetadata::isFoaf() const
{
  return m_is_foaf;
}

void
EBookMetadata::setIsFoaf(bool value)
{
  m_is_foaf = value;
}

QStringList
EBookMetadata::creatorList() const
{
  return m_creator_list;
}

void
EBookMetadata::setCreatorList(const QStringList& creator_list)
{
  m_creator_list = creator_list;
}

OrderedTitleMap
EBookMetadata::orderedTitles() const
{
  return m_ordered_titles;
}

void
EBookMetadata::setOrderedTitles(const OrderedTitleMap& ordered_titles)
{
  m_ordered_titles = ordered_titles;
}

Calibre
EBookMetadata::calibre() const
{
  return m_calibre;
}

void
EBookMetadata::writeCreator(QXmlStreamWriter* xml_writer,
                            Creator shared_creator)
{
  writeCreatorContributor("dc:creator", xml_writer, shared_creator);
}

void
EBookMetadata::writeContributor(QXmlStreamWriter* xml_writer,
                                Creator shared_creator)
{
  writeCreatorContributor("dc:contributor", xml_writer, shared_creator);
}

void
EBookMetadata::parseSourceMetadata(const QDomElement& metadata_element)
{
  QDomNamedNodeMap node_map = metadata_element.attributes();
  QDomNode node = node_map.namedItem("id");
  Source shared_source = Source(new EPubSource());
  node = node_map.namedItem("id");
  if (!node.isNull()) { // id element is NOT used in EPUB 2.0
    shared_source->id = node.nodeValue();
  }
  node = node_map.namedItem("scheme");
  if (!node.isNull()) { // id element is NOT used in EPUB 2.0
    shared_source->scheme = EBookIdentifierScheme::fromString(node.nodeValue());
  }
  shared_source->source = metadata_element.text();
  source = shared_source;
}

void
EBookMetadata::parsePublisherMetadata(const QDomElement& metadata_element)
{
  QDomNamedNodeMap node_map = metadata_element.attributes();
  Publisher shared_publisher = Publisher(new EPubPublisher());
  QDomNode node = node_map.namedItem("id");
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
    shared_publisher->alt_rep->name = node.nodeValue();
  }
  node = node_map.namedItem("alt-rep-lang");
  if (!node.isNull()) { // alt-rep-lang element is NOT used in EPUB 2.0
    shared_publisher->alt_rep->lang = node.nodeValue();
  }
  node = node_map.namedItem("file-as");
  if (!node.isNull()) { // file-as element is NOT used in EPUB 2.0
    shared_publisher->file_as->name = node.nodeValue();
  }
  shared_publisher->name = metadata_element.text();
  publisher = shared_publisher;
}

void
EBookMetadata::parseFormatMetadata(const QDomElement& metadata_element)
{
  Format shared_format = Format(new EBookFormat());
  QDomNamedNodeMap node_map = metadata_element.attributes();
  QDomNode node = node_map.namedItem("id");
  node = node_map.namedItem("id");
  if (!node.isNull()) { // id element is NOT used in EPUB 2.0
    shared_format->id = node.nodeValue();
  }
  shared_format->name = metadata_element.text();
  format = shared_format;
}

void
EBookMetadata::parseTypeMetadata(const QDomElement& metadata_element)
{
  BookType shared_type = BookType(new EBookType());
  QDomNamedNodeMap node_map = metadata_element.attributes();
  QDomNode node = node_map.namedItem("id");
  node = node_map.namedItem("id");
  if (!node.isNull()) { // id element is NOT used in EPUB 2.0
    shared_type->id = node.nodeValue();
  }
  shared_type->name = metadata_element.text();
  m_type = shared_type;
}

void
EBookMetadata::parseRelationMetadata(const QDomElement& metadata_element)
{
  Relation shared_relation = Relation(new EBookRelation());
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
  relation = shared_relation;
}

void
EBookMetadata::parseCoverageMetadata(const QDomElement& metadata_element)
{
  Coverage shared_coverage = Coverage(new EBookCoverage());
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
  coverage = shared_coverage;
}

void
EBookMetadata::parseRightsMetadata(const QDomElement& metadata_element)
{
  Rights shared_rights = Rights(new EBookRights());
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
  rights = shared_rights;
}

void
EBookMetadata::parseDublinCoreMeta(QString tag_name,
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
    date = QDateTime::fromString(metadata_element.text(), Qt::ISODate);
    if (!id_node.isNull()) {
      modified.id = id_node.nodeValue();
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

void
EBookMetadata::parseOpfMeta(QString tag_name,
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

void
EBookMetadata::parseCalibreMetas(QString id, QDomNode& node)
{
  if (id == "calibre:series")
    m_calibre->setSeriesName(node.nodeValue());
  else if (id == "calibre:series_index")
    m_calibre->setSeriesIndex(node.nodeValue());
  else if (id == "calibre:title_sort")
    m_calibre->setTitleSort(node.nodeValue());
  else if (id == "calibre:author_link_map")
    m_calibre->setAuthorLinkMap(node.nodeValue());
  else if (id == "calibre:timestamp")
    m_calibre->setTimestamp(node.nodeValue());
  else if (id == "calibre:rating")
    m_calibre->setRating(node.nodeValue());
  else if (id == "calibre:publication_type")
    m_calibre->setPublicationType(node.nodeValue());
  else if (id == "calibre:user_metadata")
    m_calibre->setUserMetadata(node.nodeValue());
  else if (id == "calibre:user_categories")
    m_calibre->setUserCategories(node.nodeValue());
  else if (id == "calibre:custom_metadata")
    m_calibre->setCustomMetadata(node.nodeValue());
  m_calibre->setModified(false);
}

void
EBookMetadata::parseTitleDateRefines(Title shared_title,
                                     QDomElement& metadata_element)
{
  shared_title->date =
    QDateTime::fromString(metadata_element.text(), Qt::ISODate);
}

Foaf
EBookMetadata::parseCreatorContributorFoafAttributes(QString property,
                                                     QDomNamedNodeMap node_map)
{
  /* Please note that these are just values I have found in examples
   * of EPUB 3.0 and 3.1 ebooks. */
  Foaf foaf = Foaf::fromString(property);
  QDomNode foaf_node = node_map.namedItem("scheme");
  if (!foaf_node.isNull()) {
    foaf.setScheme(foaf_node.nodeValue().toLower());
  }
  foaf_node = node_map.namedItem("lang");
  if (!foaf_node.isNull()) {
    foaf.setLang(foaf_node.nodeValue().toLower());
  }
  foaf_node = node_map.namedItem("id");
  if (!foaf_node.isNull()) {
    foaf.setId(foaf_node.nodeValue().toLower());
  }
  foaf_node = node_map.namedItem("content");
  if (!foaf_node.isNull()) {
    foaf.setContent(foaf_node.nodeValue().toLower());
  }
  foaf_node = node_map.namedItem("about");
  if (!foaf_node.isNull()) {
    foaf.setAbout(foaf_node.nodeValue().toLower());
  }
  return foaf;
}

void
EBookMetadata::parseCreatorContributorRefines(Creator shared_creator,
                                              QDomElement& metadata_element,
                                              QString id,
                                              QDomNamedNodeMap node_map)
{
  QDomNode node = node_map.namedItem("property");
  if (!node.isNull()) {
    QString property = node.nodeValue().toLower();
    if (property == "role") {
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
      }
    } else if (property == "alternate-script") {
      AltRep alt_rep = AltRep(new EBookAltRep());
      alt_rep->name = metadata_element.text();
      node = node_map.namedItem("lang");
      if (!node.isNull()) {
        alt_rep->lang = node.nodeValue();
      }
      shared_creator->alt_rep_list.append(alt_rep);
    } else if (property == "file-as") {
      FileAs file_as = FileAs(new EBookFileAs());
      file_as->name = node.nodeValue();
      node = node_map.namedItem("lang");
      if (!node.isNull()) {
        file_as->lang = node.nodeValue();
      }
      shared_creator->file_as_list.append(file_as);
    } else if (property.startsWith("foaf:")) {
      if (m_creators_by_id.contains(id)) {
        Creator shared_creator = m_creators_by_id.value(id);
        Foaf foaf = parseCreatorContributorFoafAttributes(property, node_map);
        shared_creator->foaf_list.append(foaf);
      } else if (contributors_by_id.contains(id)) {
        Contributor shared_contributor = contributors_by_id.value(id);
        Foaf foaf = parseCreatorContributorFoafAttributes(property, node_map);
        shared_contributor->foaf_list.append(foaf);
      }
    }
  }
}

void
EBookMetadata::parseTitleRefines(QString id,
                                 QDomElement metadata_element,
                                 QDomNamedNodeMap node_map)
{
  Title shared_title = m_titles_by_id.value(id);
  QDomNode node = node_map.namedItem("property");
  if (!node.isNull()) {
    QString property = node.nodeValue().toLower();
    if (property == "title-type") {
      // "title-type" is not used in 3.1 so ignore that node.
    } else if (property == "display-seq") {
      // refines/display-seq has been superceded in 3.1. Titles should
      // appear in the order required so reorder them in display-seq
      // order if these items appear in the metadata.
      int new_seq = metadata_element.text().trimmed().toInt();
      int old_seq = m_ordered_titles.key(shared_title);
      if (new_seq != old_seq) { // dont change if key is correct.
        if (m_ordered_titles.contains(new_seq)) { // bugger key already exists
          Title existing_title = m_ordered_titles.value(new_seq);
          m_ordered_titles.remove(new_seq);
          m_ordered_titles.remove(old_seq);
          m_ordered_titles.insert(new_seq, shared_title);
          m_ordered_titles.insert(getNextTitleIndex(), existing_title);
        } else { // otherwise it is a free key
          m_ordered_titles.remove(old_seq);
          m_ordered_titles.insert(new_seq, shared_title);
        }
      }
    } else if (property == "alternate-script") {
      AltRep alt_rep = AltRep(new EBookAltRep());
      alt_rep->name = metadata_element.text();
      node = node_map.namedItem("lang");
      if (!node.isNull()) {
        alt_rep->lang = node.nodeValue();
      }
      shared_title->alt_rep_list.append(alt_rep);
    } else if (property == "file-as") {
      FileAs file_as = FileAs(new EBookFileAs());
      file_as->name = node.nodeValue();
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

void
EBookMetadata::parseRefineMetas(QDomElement& metadata_element,
                                QDomNode& node,
                                QDomNamedNodeMap& node_map)
{
  QString id = node.nodeValue().toLower();
  if (id.startsWith("#")) {
    id = id.right(id.length() - 1);
    if (m_titles_by_id.contains(id)) {
      // is it a title?
      parseTitleRefines(id, metadata_element, node_map);
    }
  } else if (m_languages.contains(id)) {
    // is it a language?
    Language shared_language = m_languages.value(id);
    node = node_map.namedItem("property");
    if (!node.isNull()) {
      id = node.nodeValue().toLower();
      if (id == "language")
        shared_language->language = metadata_element.text();
    }
  } else if (m_creators_by_id.contains(id)) {
    // is it a creator
    Creator shared_creator = m_creators_by_id.value(id);
    parseCreatorContributorRefines(
      shared_creator, metadata_element, id, node_map);
  } else if (contributors_by_name.contains(id)) {
    // is it a creator
    Creator shared_contributor = contributors_by_name.value(id);
    parseCreatorContributorRefines(
      shared_contributor, metadata_element, id, node_map);
  }
}

bool
EBookMetadata::parseMetadataItem(const QDomNode& metadata_node)
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
          if (!source.isNull()) {
            source->source = metadata_element.text();
          } else {
            Source shared_source = Source(new EPubSource());
            shared_source->source = metadata_element.text();
            source = shared_source;
          }
        } else if (dcterms.term() == DCTerms::MODIFIED) {
          modified.date =
            QDateTime::fromString(metadata_element.text(), Qt::ISODate);
        } else if (dcterms.term() == DCTerms::IDENTIFIER) {
          // TODO
        } else if (dcterms.term() == DCTerms::RIGHTS) {
          if (!rights.isNull()) {
            rights->name = metadata_element.text();
          } else {
            Rights shared_rights = Rights(new EBookRights());
            shared_rights->name = metadata_element.text();
            rights = shared_rights;
          }
        } else if (dcterms.term() == DCTerms::SUBJECT) {
          // TODO
        } else if (dcterms.term() == DCTerms::CREATOR) {
          QString creator = metadata_element.text();
          QStringList keys = contributors_by_name.keys();
          if (!keys.contains(creator, Qt::CaseInsensitive)) {
            // This might cause a contributor to be duplicated if the spelling
            // is different. I think that that will have to be edited manually
            // by the user.
            Creator shared_creator = Creator(new EBookCreator());
            shared_creator->name = creator;
            m_creators_by_name.insert(creator, shared_creator);
          }
        } else if (dcterms.term() == DCTerms::CREATED) {
          // TODO
        } else if (dcterms.term() == DCTerms::LANGUAGE) {
          // TODO
        } else if (dcterms.term() == DCTerms::DESCRIPTION) {
          // TODO
        } else if (dcterms.term() == DCTerms::TITLE) {
          QString title = metadata_element.text();
          QStringList keys = m_titles_by_name.keys();
          if (keys.contains(title, Qt::CaseInsensitive)) {
            Title shared_title = Title(new EBookTitle());
            shared_title->title = title;
            m_titles_by_name.insert(title, shared_title);
          }
        } else if (dcterms.term() == DCTerms::PUBLISHER) {
          if (publisher.isNull()) {
            // TODO - maybe more than one publisher?
            publisher = Publisher(new EPubPublisher());
            publisher->name = metadata_element.text();
          }
        } else if (dcterms.term() == DCTerms::CONTRIBUTOR) {
          QString contributor = metadata_element.text();
          QStringList keys = contributors_by_name.keys();
          if (!keys.contains(contributor, Qt::CaseInsensitive)) {
            // This might cause a contributor to be duplicated if the spelling
            // is different. I think that that will have to be edited manually
            // by the user.
            Contributor shared_contributor =
              Contributor(new EBookContributor());
            shared_contributor->name = contributor;
            contributors_by_name.insert(contributor, shared_contributor);
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
          date = QDateTime::fromString(metadata_element.text(), Qt::ISODate);
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
          m_extra_metas.insert(id, node.nodeValue());
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

void
EBookMetadata::parseTitleMetadata(QDomElement metadata_element)
{
  Title shared_title = Title(new EBookTitle());
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
    AltRep shared_alt_rep = AltRep(new EBookAltRep());
    shared_alt_rep->name = node.nodeValue();
    node = node_map.namedItem("alt-rep-lang");
    if (!node.isNull()) { // alt-rep-lang element is NOT used in EPUB 2.0
      shared_alt_rep->lang = node.nodeValue();
    }
    shared_title->alt_rep_list.append(shared_alt_rep);
  }
  node = node_map.namedItem("file-as");
  if (!node.isNull()) { // file-as element is NOT used in EPUB 2.0
    FileAs file_as = FileAs(new EBookFileAs());
    file_as->name = node.nodeValue();
    shared_title->file_as_list.append(file_as);
  }

  shared_title->title = metadata_element.text();
  m_titles_by_id.insert(shared_title->id, shared_title);
  m_titles_by_name.insert(shared_title->title, shared_title);
  m_ordered_titles.insert(getNextTitleIndex(), shared_title);
}

void
EBookMetadata::parseCreatorMetadata(QDomElement metadata_element)
{
  QDomNamedNodeMap node_map = metadata_element.attributes();
  Creator creator = Creator(new EBookCreator());
  QDomNode node = node_map.namedItem("id");
  if (!node.isNull()) { //  EPUB 3.0 only
    creator->id = node.nodeValue();
  }
  /* Not certain if anybody is using the role information. The EPUB people
   * seem a bit ambivalent about it as well so for the time being I am
   * going to leave it as text and not use it for anything. Also if you
   * look at marc:role the list is enormous, most of which are irrelevant
   * to an ebook.
   */
  node = node_map.namedItem("role"); // 2.0 & 3.0
  if (!node.isNull()) {
    creator->relator = MarcRelator::fromString(node.nodeValue());
    if (creator->relator.type() == MarcRelator::NO_TYPE) {
      creator->string_creator = node.nodeValue();
      QLOG_DEBUG(
        QString("An unexpected role has come up. %1").arg(node.nodeValue()))
    }
  }
  node = node_map.namedItem("file-as"); // 2.0 & 3.0
  if (!node.isNull()) {
    FileAs file_as = FileAs(new EBookFileAs());
    file_as->name = node.nodeValue();
    creator->file_as_list.append(file_as);
  }
  node = node_map.namedItem("scheme"); // 3.0
  if (!node.isNull()) {
    QDomElement elem = node.toElement();
    if (!elem.isNull()) {
      QString scheme = elem.attribute("scheme");
      if (!scheme.isEmpty()) {
        if (scheme == "marc:relators") {
          if (creator->relator.type() != MarcRelator::NO_TYPE) {
            MarcRelator relator =
              MarcRelator::fromString(metadata_element.text());
            if (relator.type() != MarcRelator::NO_TYPE) {
              creator->relator = relator;
            }
          }
        }
      }
    }
  }
  node = node_map.namedItem("alt-rep"); // 3.0
  if (!node.isNull()) {
    AltRep alt_rep = AltRep(new EBookAltRep());
    alt_rep->name = node.nodeValue();
    node = node_map.namedItem("alt-rep-lang"); // 3.0
    if (!node.isNull()) {
      alt_rep->lang = node.nodeValue();
    }
    creator->alt_rep_list.append(alt_rep);
  }

  creator->name = metadata_element.text();
  m_creators_by_id.insert(creator->id, creator);
  m_creators_by_name.insert(creator->name, creator);
  m_creator_list.append(creator->name);
}

void
EBookMetadata::parseContributorMetadata(QDomElement metadata_element)
{
  /* Please note that according to the 3.0 spec Contributors data are
   * identical to Creators data. */
  QDomNamedNodeMap node_map = metadata_element.attributes();
  Contributor shared_contributor = Contributor(new EBookContributor());
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
    FileAs file_as = FileAs(new EBookFileAs());
    file_as->name = node.nodeValue();
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
    AltRep alt_rep = AltRep(new EBookAltRep());
    alt_rep->name = node.nodeValue();
    node = node_map.namedItem("alt-rep-lang"); // 3.0
    if (!node.isNull()) {
      alt_rep->lang = node.nodeValue();
    }
    shared_contributor->alt_rep_list.append(alt_rep);
  }

  shared_contributor->name = metadata_element.text();
  if (!shared_contributor->id.isEmpty()) {
    contributors_by_id.insert(shared_contributor->id, shared_contributor);
  }
  contributors_by_name.insert(shared_contributor->name, shared_contributor);
  //  creator_list.append(shared_creator->name);
}

void
EBookMetadata::parseDescriptionMetadata(QDomElement metadata_element)
{
  /* Please note that according to the 3.0 spec Contributors data are
   * identical to Creators data. */
  QDomNamedNodeMap node_map = metadata_element.attributes();
  description = Description(new EBookDescription());
  QDomNode node = node_map.namedItem("id");
  if (!node.isNull()) { //  EPUB 3.0 only
    description->id = node.nodeValue();
  }
  node = node_map.namedItem("dir");
  if (!node.isNull()) { // dir element is NOT used in EPUB 2.0
    description->dir = node.nodeValue();
  }
  node = node_map.namedItem("lang");
  if (!node.isNull()) { // lang element is NOT used in EPUB 2.0
    description->language = node.nodeValue();
  }

  description->text = metadata_element.text();
}

void
EBookMetadata::parseIdentifierMetadata(QDomElement metadata_element)
{
  QDomNamedNodeMap node_map = metadata_element.attributes();
  Identifier shared_identifier = Identifier(new EBookIdentifier());
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
      EBookIdentifierScheme::fromString(node.nodeValue());
  }

  shared_identifier->name = metadata_element.text();

  m_identifiers.insert(shared_identifier->identifier.scheme, shared_identifier);
}

void
EBookMetadata::parseLanguageMetadata(QDomElement metadata_element)
{
  QDomNamedNodeMap node_map = metadata_element.attributes();
  Language shared_language = Language(new EBookLanguage());
  QDomNode node = node_map.namedItem("id");
  if (!node.isNull()) {
    shared_language->id = node.nodeValue();
  }
  shared_language->language = metadata_element.text();
  m_languages.insert(shared_language->id, shared_language);
}

void
EBookMetadata::parseSubjectMetadata(QDomElement metadata_element)
{
  QDomNamedNodeMap node_map = metadata_element.attributes();
  QDomNode node = node_map.namedItem("id");
  Subject shared_subject = Subject(new EBookSubject());
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
  subjects.insert(shared_subject->subject, shared_subject);
}

void
EBookMetadata::parseDateModified(QDomNamedNodeMap node_map, QString text)
{
  QDateTime date_modified = QDateTime::fromString(text, Qt::ISODate);
  modified.date = date_modified;
  QDomNode node = node_map.namedItem("id");
  if (!node.isNull()) {
    modified.id = node.nodeValue();
  }
  if (!date.isValid()) {
    // if no document release date is set then set it to the modified value.
    // this will be overridden if a dc:date meta node is found later.
    date = modified.date;
  }
}

/*!
 * \brief Writes a creator or contributors metadata.
 *
 * Firsts decides whether we are writing a creator or a contributor, then
 * passes the object to the actual write method.
 *
 * \param tag_name
 * \param xml_writer
 * \param key
 * \return
 */
QString
EBookMetadata::writeCreatorContibutor(QString tag_name,
                                      QXmlStreamWriter* xml_writer,
                                      QString key)
{
  // Basically contributors and creators are identical in form if not in
  // definition, contributors are lesser ctreators effectively.
  Creator shared_object;
  QString name;

  // As EPubContributor is basically an unchanged EPubCreator we only need to
  // do this bit to make certain we get the correct data store.
  Creator shared_creator =
    m_creators_by_name.value(key).dynamicCast<EBookCreator>();
  if (shared_creator) {
    shared_object = shared_creator;
  } else {
    Contributor shared_contributor =
      contributors_by_name.value(key).dynamicCast<EBookContributor>();
    shared_object = shared_contributor;
  }

  name = writeCreatorContributor(tag_name, xml_writer, shared_object);

  return name;
}

void
EBookMetadata::writeLanguageMetadata(QXmlStreamWriter* xml_writer,
                                     QString key,
                                     bool first)
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
  Language shared_language = m_languages.value(key);
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

void
EBookMetadata::writeSubjectMetadata(QXmlStreamWriter* xml_writer, QString key)
{
  Subject shared_subject = subjects.value(key);

  if (!shared_subject.isNull()) {
    if (!shared_subject->subject.isEmpty()) {
      xml_writer->writeStartElement("dc:subject");

      writeIdAttribute(xml_writer, shared_subject->id);
      writeDirAttribute(xml_writer, shared_subject->dir);
      writeLangAttribute(xml_writer, shared_subject->lang);
      writeAuthorityAttribute(
        xml_writer, shared_subject->authority, shared_subject->term);

      xml_writer->writeCharacters(shared_subject->subject);
      xml_writer->writeEndElement();
    }
  }
}

void
EBookMetadata::writeIdentifierMetadata(
  QXmlStreamWriter* xml_writer,
  EBookIdentifierScheme::IdentifierScheme key)
{
  Identifier shared_identifier = m_identifiers.value(key);
  if (!shared_identifier->name.isEmpty()) {
    xml_writer->writeStartElement("dc:identifier");
    writeIdAttribute(xml_writer, shared_identifier->id);
    writeSchemeAttribute(xml_writer, shared_identifier->identifier);

    xml_writer->writeCharacters(shared_identifier->name);
    xml_writer->writeEndElement();
  }
}

void
EBookMetadata::writeTitle(QXmlStreamWriter* xml_writer, Title shared_title)
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
    AltRepList alt_reps = shared_title->alt_rep_list;
    foreach (AltRep alt_rep, alt_reps) {
      writeAltRepAttribute(xml_writer, alt_rep);
    }
    FileAsList file_as_list = shared_title->file_as_list;
    foreach (FileAs file_as, file_as_list) {
      writeFileAsAttribute(xml_writer, file_as);
    }

    xml_writer->writeCharacters(shared_title->title);
    xml_writer->writeEndElement();
  }
}

QString
EBookMetadata::writeTitleMetadata(QXmlStreamWriter* xml_writer, int key)
{
  Title shared_title = m_ordered_titles.value(key);
  QString title;
  if (!shared_title.isNull()) {
    title = shared_title->title;
    writeTitle(xml_writer, shared_title);
  }
  return title;
}

void
EBookMetadata::writeDescriptionMetadata(QXmlStreamWriter* xml_writer)
{
  if (!description->text.isEmpty()) {
    xml_writer->writeStartElement("dc:description");

    writeIdAttribute(xml_writer, description->id);
    writeDirAttribute(xml_writer, description->dir);
    writeLangAttribute(xml_writer, description->language);

    xml_writer->writeCharacters(description->text);
    xml_writer->writeEndElement();
  }
}

void
EBookMetadata::writeSourceMetadata(QXmlStreamWriter* xml_writer)
{
  Source shared_source = source;
  if (!shared_source->source.isEmpty()) {
    xml_writer->writeStartElement("dc:source");
    writeIdAttribute(xml_writer, shared_source->id);
    writeSchemeAttribute(xml_writer, shared_source->scheme);
    xml_writer->writeCharacters(shared_source->source);
    xml_writer->writeEndElement();
  }
}

void
EBookMetadata::writePublisherMetadata(QXmlStreamWriter* xml_writer)
{
  Publisher shared_publisher = publisher;
  if (!shared_publisher->name.isEmpty()) {

    xml_writer->writeStartElement("dc:publisher");

    writeIdAttribute(xml_writer, shared_publisher->id);
    writeFileAsAttribute(xml_writer, shared_publisher->file_as);
    writeDirAttribute(xml_writer, shared_publisher->dir);
    writeLangAttribute(xml_writer, shared_publisher->lang);
    writeAltRepAttribute(xml_writer, shared_publisher->alt_rep);

    xml_writer->writeCharacters(shared_publisher->name);
    xml_writer->writeEndElement();
  }
}

void
EBookMetadata::writeFormatMetadata(QXmlStreamWriter* xml_writer)
{
  Format shared_format = format;
  if (!shared_format->name.isEmpty()) {
    xml_writer->writeStartElement("dc:format");

    writeIdAttribute(xml_writer, shared_format->id);

    xml_writer->writeCharacters(shared_format->name);
    xml_writer->writeEndElement();
  }
}

void
EBookMetadata::writeRelationMetadata(QXmlStreamWriter* xml_writer)
{
  Relation shared_publisher = relation;
  if (!shared_publisher->name.isEmpty()) {
    xml_writer->writeStartElement("dc:relation");

    writeIdAttribute(xml_writer, shared_publisher->id);
    writeDirAttribute(xml_writer, shared_publisher->dir);
    writeLangAttribute(xml_writer, shared_publisher->lang);

    xml_writer->writeCharacters(shared_publisher->name);
    xml_writer->writeEndElement();
  }
}

void
EBookMetadata::writeRightsMetadata(QXmlStreamWriter* xml_writer)
{
  Rights shared_rights = rights;
  if (!shared_rights->name.isEmpty()) {
    xml_writer->writeStartElement("dc:rights");

    writeIdAttribute(xml_writer, shared_rights->id);
    writeDirAttribute(xml_writer, shared_rights->dir);
    writeLangAttribute(xml_writer, shared_rights->lang);

    xml_writer->writeCharacters(shared_rights->name);
    xml_writer->writeEndElement();
  }
}

void
EBookMetadata::writeCoverageMetadata(QXmlStreamWriter* xml_writer)
{
  Coverage shared_coverage = coverage;
  if (!shared_coverage->name.isEmpty()) {
    xml_writer->writeStartElement("dc:coverage");

    writeIdAttribute(xml_writer, shared_coverage->id);
    writeDirAttribute(xml_writer, shared_coverage->dir);
    writeLangAttribute(xml_writer, shared_coverage->lang);

    xml_writer->writeCharacters(shared_coverage->name);
    xml_writer->writeEndElement();
  }
}

void
EBookMetadata::writeIdAttribute(QXmlStreamWriter* xml_writer, QString id)
{
  if (!id.isEmpty()) { // 3.0.
    xml_writer->writeAttribute("id", id);
  }
}

void
EBookMetadata::writeDirAttribute(QXmlStreamWriter* xml_writer, QString dir)
{
  if (!dir.isEmpty()) { // 3.0.
    xml_writer->writeAttribute("dir", dir);
  }
}

void
EBookMetadata::writeLangAttribute(QXmlStreamWriter* xml_writer, QString lang)
{
  if (!lang.isEmpty()) { // 3.0.
    xml_writer->writeAttribute("xml:lang", lang);
  }
}

void
EBookMetadata::writeAuthorityAttribute(QXmlStreamWriter* xml_writer,
                                       QString authority,
                                       QString term)
{
  if (!authority.isEmpty()) { // 3.0.
    xml_writer->writeAttribute("opf:authority", authority);
  }

  if (!authority.isEmpty() && !term.isEmpty()) { // 2.0 and 3.0 with id set.
    xml_writer->writeAttribute("opf:term", term);
  }
}

void
EBookMetadata::writeAltRepAttribute(QXmlStreamWriter* xml_writer,
                                    AltRep alt_rep)
{
  if (alt_rep) {
    if (!alt_rep->name.isEmpty()) { // 3.0
      xml_writer->writeAttribute("opf:alt-rep", alt_rep->name);
    }
    if (!alt_rep->lang.isEmpty()) { // 3.0
      xml_writer->writeAttribute("opf:alt-rep-lang", alt_rep->lang);
    }
  }
}

void
EBookMetadata::writeFileAsAttribute(QXmlStreamWriter* xml_writer,
                                    FileAs file_as)
{
  if (file_as) { // 3.0
    if (file_as->name.isEmpty())
      xml_writer->writeAttribute("opf:file-as", file_as->name);
    writeLangAttribute(xml_writer, file_as->lang);
  }
}

QString
EBookMetadata::writeCreatorsMetadata(QXmlStreamWriter* xml_writer, QString key)
{
  return writeCreatorContibutor("dc:creator", xml_writer, key);
}

QString
EBookMetadata::writeContributorMetadata(QXmlStreamWriter* xml_writer,
                                        QString key)
{
  return writeCreatorContibutor("dc:contributor", xml_writer, key);
}

void
EBookMetadata::writeRoleAttribute(QXmlStreamWriter* xml_writer,
                                  MarcRelator relator)
{
  if (relator.type() != MarcRelator::NO_TYPE) {
    xml_writer->writeAttribute("opf:role", relator.code());
  }
}

void
EBookMetadata::writeSchemeAttribute(QXmlStreamWriter* xml_writer,
                                    EBookIdentifierScheme scheme)
{
  writeSchemeAttribute(xml_writer,
                       EBookIdentifierScheme::toString(scheme.scheme));
}

void
EBookMetadata::writeSchemeAttribute(QXmlStreamWriter* xml_writer,
                                    QString scheme)
{
  if (!scheme.isEmpty()) {
    xml_writer->writeAttribute("opf:scheme", scheme);
  }
}

/*!
 * \brief Writes the creator or contributors metadata.
 *
 * \param tag_name
 * \param xml_writer
 * \param shared_object
 */
QString
EBookMetadata::writeCreatorContributor(QString tag_name,
                                       QXmlStreamWriter* xml_writer,
                                       Creator shared_object)
{
  QString name;
  if (!shared_object.isNull()) {
    QString id = shared_object->id;
    name = shared_object->name;

    if (!shared_object->name.isEmpty()) {
      xml_writer->writeStartElement(tag_name);

      writeIdAttribute(xml_writer, shared_object->id);
      foreach (FileAs file_as, shared_object->file_as_list) {
        writeFileAsAttribute(xml_writer, file_as);
      }
      foreach (AltRep alt_rep, shared_object->alt_rep_list) {
        writeAltRepAttribute(xml_writer, alt_rep);
      }
      writeRoleAttribute(xml_writer, shared_object->relator);

      xml_writer->writeCharacters(shared_object->name);
      xml_writer->writeEndElement();

      foreach (Foaf foaf, shared_object->foaf_list) {
        xml_writer->writeStartElement("meta");
        xml_writer->writeAttribute("property", foaf.asString());
        writeIdAttribute(xml_writer, foaf.id());
        writeLangAttribute(xml_writer, foaf.lang());
        writeSchemeAttribute(xml_writer, foaf.scheme());
        xml_writer->writeCharacters(foaf.content());
        xml_writer->writeEndElement();
      }
    }
  }
  return name;
}

int
EBookMetadata::getNextTitleIndex()
{
  int highest = 0;
  foreach (int i, m_ordered_titles.keys()) {
    if (i > highest)
      highest = i;
  }
  highest++;
  return highest;
}
