#ifndef EPUBCONTAINER_H
#define EPUBCONTAINER_H

#include <QDomNode>
#include <QHash>
#include <QList>
#include <QMap>
#include <QMimeDatabase>
#include <QObject>
#include <QPair>
#include <QSet>
#include <QSharedPointer>
#include <QStringList>
#include <QStringLiteral>
#include <QTextStream>
#include <QVector>
#include <QtSvg>

#include <quazip5/quazip.h>
#include <quazip5/quazipfile.h>

#include "authors.h"
#include "dcterms.h"
#include "ebookcommon.h"
#include "foaf.h"
#include "library.h"
#include "marcrelator.h"

#include <ebookmetadata.h>

class QXmlStreamReader;

// struct EBookMetadata {

//  EBookMetadata();

//  SharedIdentifierMap identifiers;
//  SharedTitleMap titles_by_id;
//  SharedTitleMap titles_by_name;
//  OrderedTitleMap ordered_titles;
//  SharedCreatorMap creators_by_id;
//  SharedCreatorMap creators_by_name;
//  //  SharedCreatorMap creators_by_name;
//  SharedContributorMap contributors_by_id;
//  SharedContributorMap contributors_by_name;
//  SharedDescription description;
//  CreatorList creator_list;
//  SharedLanguageMap languages;
//  SharedSubjectMap subjects;
//  EPubModified modified;
//  QDateTime date;
//  SharedSource source;
//  SharedPublisher publisher;
//  SharedFormat format;
//  SharedRelation relation;
//  SharedCoverage coverage;
//  SharedRights rights;
//  SharedType type;
//  QMap<QString, QString> extra_metas;
//  Calibre calibre;
//  bool is_foaf;
//};
// typedef QSharedPointer<EBookMetadata> SharedMetadata;

struct EPubManifestItem
{
  QString href;
  QString path;
  //  SharedDomDocument dom_document;
  QString document_string;
  QStringList css_links;
  QString body_class;
  // these should point to the start and end of each chapter block;
  //  SharedTextCursor start, end;
  QTextCursor start_cursor, end_cursor;
  QString id;
  QByteArray media_type;
  QStringList properties;
  QString fallback;
  QString media_overlay;
  QMap<QString, QString> non_standard_properties;
};
typedef QSharedPointer<EPubManifestItem> SharedManifestItem;
typedef QMap<QString, SharedManifestItem> SharedManifestItemMap;
typedef QList<SharedManifestItem> SharedManifestItemList;

class EPubTocItem;
typedef QSharedPointer<EPubTocItem> SharedTocItem;
typedef QMap<int, SharedTocItem> SharedTocItemMap;
typedef QMap<QString, SharedTocItem> SharedTocItemPathMap;
class EPubTocItem
{
public:
  QString id;
  int playorder;
  QString tag_class;
  QString label;
  QString source;
  SharedTocItemMap sub_items;
  QString chapter_tag;
};

class EPubManifest
{
public:
  QString id;
  SharedManifestItem cover_image; // 0 or 1
  SharedManifestItem nav;         // 1
  SharedManifestItemMap items;    // all items
  SharedManifestItemList html_items;
  SharedManifestItemMap mathml;              // subset of items for math markup
  SharedManifestItemMap svg_images;          // subset of items for images
  QMap<QString, QImage> rendered_svg_images; // rendered svg images
  QMap<QString, QImage> images;
  SharedManifestItemMap remotes;
  SharedManifestItemMap scripted;
  SharedManifestItemMap switches;
  QMap<QString, QString> css;          // all items
  QMap<QString, QString> javascript;   // all items
  SharedManifestItemMap fonts;         // all items
  SharedManifestItemMap media_overlay; // all items
  QString formatted_toc_string;
  SharedTocItemMap toc_items;
  SharedTocItemPathMap toc_paths;
};

class EPubSpineItem
{
public:
  EPubSpineItem()
  {
    linear = false;
    page_spread_left = false;
    page_spread_right = false;
  }
  QString id;
  QString idref;
  bool linear;
  bool page_spread_left;
  bool page_spread_right;
};
typedef QSharedPointer<EPubSpineItem> SharedSpineItem;
typedef QMap<QString, SharedSpineItem> SharedSpineItemMap;
typedef QStringList SpineItemList;

class EPubSpine
{
public:
  QString id;
  QString toc;
  QString page_progression_dir;
  SharedSpineItemMap items;
  SpineItemList ordered_items;
};

/*!
 * \brief The EPUB guide section
 *
 * This is only included to support epub2.0 as it has been superceded in epub3.0
 * by the landmarks object.
 */
class EPubGuideItem
{
public:
  enum GuideType
  {
    cover,
    title_page,
    toc,
    index,
    glossary,
    acknowledgements,
    bibliography,
    colophon,
    copyright_page,
    dedication,
    epigraph,
    foreword,
    loi, // ist of illustrations
    lot, // list of tables
    notes,
    preface,
    text,
  };

  GuideType type;
  QString title;
  QString href;

  static GuideType fromString(QString type)
  {
    if (type == "cover") {
      return GuideType::cover;
    } else if (type == "title-page") {
      return GuideType::title_page;
    } else if (type == "toc") {
      return GuideType::toc;
    } else if (type == "index") {
      return GuideType::index;
    } else if (type == "glossary") {
      return GuideType::glossary;
    } else if (type == "acknowledgements") {
      return GuideType::acknowledgements;
    } else if (type == "bibliography") {
      return GuideType::bibliography;
    } else if (type == "colophon") {
      return GuideType::colophon;
    } else if (type == "copyright-page") {
      return GuideType::copyright_page;
    } else if (type == "dedication") {
      return GuideType::dedication;
    } else if (type == "epigraph") {
      return GuideType::epigraph;
    } else if (type == "foreword") {
      return GuideType::foreword;
    } else if (type == "loi") { // list of illustrations
      return GuideType::loi;
    } else if (type == "lot") { // list of tables
      return GuideType::lot;
    } else if (type == "notes") {
      return GuideType::notes;
    } else if (type == "preface") {
      return GuideType::preface;
    } else {
      /*if (type == "text") { */
      return GuideType::text;
    }
  }

  static QString toString(GuideType type)
  {
    switch (type) {
      case cover:
        return "cover";
      case title_page:
        return "title-page";
      case toc:
        return "toc";
      case index:
        return "index";
      case glossary:
        return "glossary";
      case acknowledgements:
        return "acknowledgements";
      case bibliography:
        return "bibliography";
      case colophon:
        return "colophon";
      case copyright_page:
        return "copyright-page";
      case dedication:
        return "dedication";
      case epigraph:
        return "epigraph";
      case foreword:
        return "foreword";
      case loi:
        return "loi";
      case lot:
        return "lot";
      case notes:
        return "notes";
      case preface:
        return "preface";
      case text:
        return "text";
    }
  }
};
typedef QSharedPointer<EPubGuideItem> SharedGuideItem;
typedef QMap<QString, SharedGuideItem> SharedGuideItemMap;
typedef QStringList GuideItemList;

class EPubContainer : public QObject
{
  Q_OBJECT
public:
  explicit EPubContainer(QObject* parent = nullptr);
  ~EPubContainer();

  bool loadFile(const QString path);
  QString filename();
  void setFilename(QString filename);
  bool saveFile(const QString& filepath = QString());
  bool closeFile();
  //  QByteArray epubItem(const QString& id) const;
  //  QSharedPointer<QuaZipFile> zipFile(const QString& path);
  QImage image(const QString& id, QSize image_size = QSize());
  // metadata is stored in a QMultiHash to allow multiple values
  // of a key. eg. there might be more than one "creator" tag.
  QStringList itemKeys();
  SharedManifestItem item(QString key);
  QString css(QString key);
  QString javascript(QString key);
  QString itemDocument(QString key);
  QStringList spineKeys();
  QStringList imageKeys();
  QStringList cssKeys();
  QStringList jsKeys();
  QString tocAsString();
  QStringList creators();

  Metadata metadata();
  EPubManifest manifest();

  QString buildTocfromHtml();

signals:
  void errorHappened(const QString& error);

public slots:

protected:
  // Base epub data
  int m_version;
  QString m_package_xmlns;
  QString m_package_language;
  QString m_package_prefix;
  QString
    m_package_direction; // text direction - overridden  by Undicode values.
  QString m_package_id;  // unique identifier.

  // only one container per epub.
  QString m_container_version;
  QString m_container_xmlns;
  QString m_container_fullpath;
  QString m_container_mediatype;
  LibraryDB* m_library;
  EBookAuthorsDB* m_authors;

  bool parseMimetype();
  bool writeMimetype(QuaZip* save_zip);
  bool parseContainer();
  bool writeContainer(QuaZip* save_zip);
  bool parsePackageFile(QString& full_path);
  bool writePackageFile(QuaZip* save_zip);

  bool parseManifestItem(const QDomNode& manifest_node,
                         const QString current_folder);
  void extractHeadInformationFromHtmlFile(SharedManifestItem item,
                                          QString container);

  SharedSpineItem parseSpineItem(const QDomNode& metadata_element,
                                 SharedSpineItem item);
  bool saveSpineItem();
  bool parseTocFile();
  bool parseGuideItem(const QDomNode& guideItem);
  bool parseLandmarksItem(const QDomNode& guideItem);
  bool saveLandmarksItem();
  bool parseBindingsItem(const QDomNode& bindingsItem);
  bool saveBindingsItem();

  const QuaZip* getFile(const QString& path);

  QuaZip* m_archive = nullptr;
  QString m_filename;
  QStringList m_files;

  // metadata/manifest/spine etc.
  QByteArray m_mimetype;
  Metadata m_metadata;
  EPubManifest m_manifest;
  EPubSpine m_spine;

  // might be more than one root content file.
  QMap<QString, QMap<QString, DomDocument>> m_rootfiles;
  QMap<QString, DomDocument> m_current_rootfile;
  // a map of dom elements within the  that might be modified.
  QMap<QString, QDomElement*> m_metadata_nodes;
  int m_toc_chapter_index;

  SharedTocItem parseNavPoint(QDomElement navpoint,
                              QString& formatted_toc_data);

  void createAnchorPointForChapter(SharedTocItem toc_item,
                                   SharedManifestItem manifest_item);
  //  void createChapterAnchorPoints(SharedSpineItem spine_item);
  void handleSubNavpoints(QDomElement navpoint, QString& formatted_toc_string);
  QString extractTagText(int anchor_start, QString document_string);

  static const QString MIMETYPE_FILE;
  static const QByteArray MIMETYPE;
  static const QString METADATA_FOLDER;
  static const QString CONTAINER_FILE;
  static const QString TOC_FILE;

  static const QString TITLE;
  static const QString CREATOR;
  static const QString IDENTIFIER;
  static const QString LANGUAGE;

  static const QString TOC_TITLE;
  static const QString LIST_START;
  static const QString LIST_END;
  static const QString LIST_ITEM;
  static const QString LIST_BUILD_ITEM;
  static const QString LIST_FILEPOS;

  static const QString HTML_DOCTYPE;
  static const QString XML_HEADER;
  static const QString HTML_XMLNS;
  //  static const QString HEAD_META_CONTENT;
};

#endif // EPUBCONTAINER_H
