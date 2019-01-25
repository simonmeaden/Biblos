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
#include <QVector>
#include <QtSvg>

#include <quazip5/quazip.h>
#include <quazip5/quazipfile.h>

#include "ebookcommon.h"
#include "marcrelator.h"

class QXmlStreamReader;

typedef QSharedPointer<QDomDocument> SharedDomDocument;
typedef QSharedPointer<QImage> SharedImage;
typedef QMap<QString, SharedImage> SharedImageMap;
typedef QSharedPointer<QTextCursor> SharedTextCursor;

enum EPubType {
  EPUB_2_0,
  EPUB_3_0,
};

class EPubModified
{
public:
  QDateTime date;
  QString id;
};

class EPubBaseMetadata
{
  EPubModified modified;
};

class EPubTitle : public EPubBaseMetadata
{
public:
  enum TitleType {
    no_type,
    main,
    subtitle,
    short_title,
    collection,
    edition,
    expanded,
  };
  EPubTitle()
  {
    type = no_type; // default
    sequence = 0;
  }
  //  QString title_type;
  QString id;
  QString title;
  int sequence = -1;
  TitleType type;
  QString direction;
  QString language;
  QString alternative_rep;
  QString alternative_rep_lang;
  QString file_as;

  static TitleType fromString(QString type)
  {
    if (type == "main") {
      return TitleType::main;
    } else if (type == "subtitle") {
      return TitleType::subtitle;
    } else if (type == "short") {
      return TitleType::short_title;
    } else if (type == "collection") {
      return TitleType::collection;
    } else if (type == "edition") {
      return TitleType::edition;
    } else {
      /*if (type == "expanded") { */
      return TitleType::expanded;
    }
  }
  static QString toString(TitleType type)
  {
    switch (type) {
    case main:
      return "maintitle";
    case subtitle:
      return "subtitle";
    case short_title:
      return "short";
    case collection:
      return "collection";
    case edition:
      return "edition";
    case expanded:
      return "expanded";
    default:
      return QString();
    }
  }
};
typedef QSharedPointer<EPubTitle> SharedTitle;
typedef QMap<QString, SharedTitle> SharedTitleMap;

class EPubIdentifier
{
public:
  QString name;
  QString uid;
  QString scheme;
};
typedef QSharedPointer<EPubIdentifier> SharedIdentifier;
typedef QMap<QString, SharedIdentifier> SharedIdentifierMap;

class EPubLanguage
{
public:
  QString id;
  QString language;
};
typedef QSharedPointer<EPubLanguage> SharedLanguage;
typedef QMap<QString, SharedLanguage> SharedLanguageMap;

class EPubSubject
{
public:
  QString id;
  QString subject;
  QString authority;
  QString term;
  QString lang;
  QString dir;
};
typedef QSharedPointer<EPubSubject> SharedSubject;
typedef QMap<QString, SharedSubject> SharedSubjectMap;

class EPubCreator : public EPubBaseMetadata
{
public:
  enum SchemeType {
    dcterms,
    marc,
    media,
    onyx,
    xsd,
    string_scheme_type,
  };
  EPubCreator()
  {
  }

  QString name;
  QString role;
  QString file_as;
  QString scheme;
  QString id;
  QString alternative_script;
  QString alternative_language;

  MarcRelator relator;
  QString string_creator;
  QString string_scheme;

  static SchemeType fromSchemeString(QString type)
  {
    if (type == "dcterms") {
      return SchemeType::dcterms;
    } else if (type == "marc") {
      return SchemeType::marc;
    } else if (type == "media") {
      return SchemeType::media;
    } else if (type == "onyx") {
      return SchemeType::onyx;
    } else if (type == "xsd") {
      return SchemeType::xsd;
    } else {
      return SchemeType::string_scheme_type;
    }
  }
};

typedef QSharedPointer<EPubCreator> SharedCreator;
typedef QMultiMap<QString, SharedCreator> SharedCreatorMap;
typedef QList<QString> CreatorList;

struct EPubDescription {
  QString id;
  QString text;
  QString direction;
  QString language;
};
typedef QSharedPointer<EPubDescription> SharedDescription;
typedef QMultiMap<QString, SharedDescription> SharedDescriptionMap;

struct Calibre {
  QString series_name;
  QString series_index;
  QString title_sort;
  QString author_link_map;
  QString timestamp;
};

class EPubMetadata
{
public:
  SharedIdentifierMap identifiers;
  SharedTitleMap titles;
  SharedCreatorMap creators;
  SharedCreatorMap contributors;
  SharedDescription description;
  CreatorList creator_list;
  SharedLanguageMap languages;
  SharedSubjectMap subjects;
  EPubModified modified;
  QDateTime date;
  QString source;
  QString publisher;
  QString rights;
  QMap<QString, QString> extra_metadata;
  Calibre calibre;
};
typedef QSharedPointer<EPubMetadata> SharedMetadata;

class EPubManifestItem
{
public:
  QString href;
  QString path;
  //  SharedDomDocument dom_document;
  QString document_string;
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
  enum GuideType {
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
  explicit EPubContainer(QObject* parent = Q_NULLPTR);
  ~EPubContainer();

  bool loadFile(const QString path);
  bool saveFile();
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

  SharedMetadata metadata()
  {
    return m_metadata;
  }
  EPubManifest manifest()
  {
    return m_manifest;
  }

  QString buildTocfromHtml();

signals:
  void errorHappened(const QString& error);

public slots:

protected:
  // Base epub data
  int m_version;
  QString m_package_unique_identifier_name;
  QString m_package_unique_identifier;
  QString m_package_xmlns;
  QString m_package_language;
  QString m_package_prefix;
  QString m_package_direction; // text direction - overridden  by Undicode values.
  QString m_package_id;        // unique identifier.
  EPubType m_load_type;

  // only one container per epub.
  QString m_container_version;
  QString m_container_xmlns;
  QString m_container_fullpath;
  QString m_container_mediatype;

  bool parseMimetype();
  bool writeMimetype(QuaZip* save_zip);
  bool parseContainer();
  bool writeContainer(QuaZip* save_zip);
  bool parsePackageFile(QString& full_path);
  bool writePackageFile(QuaZip* save_zip);
  bool writeMetadata(QXmlStreamWriter* xml_writer);
  bool parseMetadataItem(const QDomNode& metadata_node);
  bool parseManifestItem(const QDomNode& manifest_node, const QString current_folder);
  SharedSpineItem parseSpineItem(const QDomNode& metadata_element, SharedSpineItem item);
  bool saveSpineItem();
  bool parseTocFile();
  bool parseGuideItem(const QDomNode& guideItem);
  bool parseLandmarksItem(const QDomNode& guideItem);
  bool saveLandmarksItem();
  bool parseBindingsItem(const QDomNode& bindingsItem);
  bool saveBindingsItem();

  const QuaZip* getFile(const QString& path);

  QuaZip* m_archive = Q_NULLPTR;
  QString m_filename;
  QStringList m_files;

  // metadata/manifest/spine etc.
  QByteArray m_mimetype;
  SharedMetadata m_metadata;
  EPubManifest m_manifest;
  EPubSpine m_spine;

  // might be more than one root content file.
  QMap<QString, QMap<QString, SharedDomDocument>> m_rootfiles;
  QMap<QString, SharedDomDocument> m_current_rootfile;
  // a map of dom elements within the  that might be modified.
  QMap<QString, QDomElement*> m_metadata_nodes;
  int m_toc_chapter_index;

  void parseTitleMetadata(QDomElement metadata_element);
  void parseCreatorMetadata(QDomElement metadata_element);
  void parseContributorMetadata(QDomElement metadata_element);
  void parseDescriptionMetadata(QDomElement metadata_element);
  void parseIdentifierMetadata(QDomElement metadata_element);
  void parseLanguageMetadata(QDomElement metadata_element);
  void parseSubjectMetadata(QDomElement metadata_element);
  void parseDateModified(QDomNamedNodeMap node_map, QString text);
  void saveTitles(QDomElement metadata_element);
  void saveCreator(QDomElement metadata_element);
  void saveIdentifier(QDomElement metadata_element);
  SharedTocItem parseNavPoint(QDomElement navpoint, QString& formatted_toc_data);
  void createAnchorPointForChapter(SharedTocItem toc_item, SharedManifestItem manifest_item);
  //  void createChapterAnchorPoints(SharedSpineItem spine_item);
  void handleSubNavpoints(QDomElement navpoint, QString& formatted_toc_string);
  QString extractTagText(int anchor_start, QString document_string);
  void writeTitleMetadata(QXmlStreamWriter* xml_writer, QString key);
  void writeDescriptionMetadata(QXmlStreamWriter* xml_writer);
  void writeCreatorsMetadata(QXmlStreamWriter* xml_writer, QString key);
  void writeContributorMetadata(QXmlStreamWriter* xml_writer, QString key);
  void writeLanguageMetadata(QXmlStreamWriter* xml_writer, QString key, bool first=false);
  void writeSubjectMetadata(QXmlStreamWriter* xml_writer, QString key);

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
};

#endif // EPUBCONTAINER_H
