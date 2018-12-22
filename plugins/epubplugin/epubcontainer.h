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

#include <quazip5/quazip.h>
#include <quazip5/quazipfile.h>

#include "ebookcommon.h"

class QXmlStreamReader;

typedef QSharedPointer<QDomDocument> SharedDomDocument;

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
    main,
    subtitle,
    short_title,
    collection,
    edition,
    expanded,
  };
  //  QString title_type;
  QString id;
  QString title;
  int sequence = -1;
  TitleType type;
  QString direction;
  QString language;

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
};
typedef QSharedPointer<EPubSubject> SharedSubject;
typedef QMap<QString, SharedSubject> SharedSubjectMap;

class EPubCreator : public EPubBaseMetadata
{
public:
  enum CreatorType {
    author,
    illustrator,
    editor,
    annotator,
    markup_editor,
    string_creator_type,
  };
  enum SchemeType {
    dcterms,
    marc,
    media,
    onyx,
    xsd,
    string_scheme_type,
  };

  QString name;
  QString role;
  QString file_as;
  QString id;
  QString alternative_script;

  CreatorType type;
  QString string_creator;
  QString string_scheme;

  static CreatorType fromCreatorString(QString type)
  {
    if (type == "aut") {
      return CreatorType::author;
    } else if (type == "ill") {
      return CreatorType::illustrator;
    } else if (type == "edt") {
      return CreatorType::editor;
    } else if (type == "ann") {
      return CreatorType::annotator;
    } else if (type == "mrk") {
      return CreatorType::markup_editor;
    } else {
      return CreatorType::string_creator_type;
    }
  }
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

  static QString toCreatorString(CreatorType type)
  {
    switch (type) {
    case author:
      return "aut";
    case illustrator:
      return "ill";
    case annotator:
      return "ann";
    case markup_editor:
      return "mrk";
    case editor:
      return "edt";
    default:
      return QString();
    }
  }
};
typedef QSharedPointer<EPubCreator> SharedCreator;
typedef QMap<QString, SharedCreator> SharedCreatorMap;

class EPubMetadata
{
public:
  SharedIdentifierMap identifiers;
  SharedTitleMap titles;
  SharedCreatorMap creators;
  SharedLanguageMap languages;
  SharedLanguageMap subjects;
  EPubModified modified;
  QDateTime date;
  QString source;
  QString description;
  QString publisher;
  QString rights;
};
typedef QSharedPointer<EPubMetadata> SharedMetadata;

class EPubManifestItem
{
public:
  QString href;
  QString id;
  QString media_type;
  QStringList properties;
  QString fallback;
  QString media_overlay;
  QMap<QString, QString> non_standard_properties;
};
typedef QSharedPointer<EPubManifestItem> SharedManifestItem;
typedef QMap<QString, SharedManifestItem> SharedManifestItemMap;

class EPubManifest
{
public:
  QString id;
  SharedManifestItem cover_image; // 0 or 1
  SharedManifestItem nav;         // 1
  SharedManifestItemMap items;    // all items
  SharedManifestItemMap mathml;   // subset of items for math markup
  SharedManifestItemMap svg;      // subset of items for images
  SharedManifestItemMap images;   // subset of items for images
  SharedManifestItemMap remotes;
  SharedManifestItemMap scripted;
  SharedManifestItemMap switches;
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

class EPubSpine
{
public:
  QString id;
  QString toc;
  QString page_progression_dir;
  SharedSpineItemMap items;
};

class EPubContainer : public QObject
{
  Q_OBJECT
public:
  explicit EPubContainer(QObject* parent = Q_NULLPTR);
  ~EPubContainer();

  bool loadFile(const QString path);
  bool saveFile(const QString path);
  bool closeFile();
  EBookItem epubItem(const QString& id) const;
  QSharedPointer<QuaZipFile> zipFile(const QString& path);
  QImage image(const QString& id);
  // metadata is stored in a QMultiHash to allow multiple values
  // of a key. eg. there might be more than one "creator" tag.
  QStringList items();
  QString standardPage(EPubPageReference::StandardType type);

signals:
  void errorHappened(const QString& error);

public slots:

protected:
  int m_version;
  QString m_unique_identifier_name;
  QString m_unique_identifier;
  QString m_language;
  QString m_prefix;
  QString m_dir; // text direction - overridden  by Undicode values.
  QString m_id;  // unique identifier.

  bool parseMimetype();
  bool saveMimetype();
  bool parseContainer();
  bool saveContainer();
  bool parsePackageFile(QString& full_path);
  bool savePackageFile(QString& full_path);
  bool parseMetadataItem(const QDomNode& metadata_node);
  bool parseManifestItem(const QDomNode& manifest_node,
                         const QString current_folder);
  bool parseSpineItem(const QDomNode& metadata_element);
  bool saveSpineItem();
  bool parseGuideItem(const QDomNode& guideItem);
  bool parseLandmarksItem(const QDomNode& guideItem);
  bool saveLandmarksItem();
  bool parseBindingsItem(const QDomNode& bindingsItem);
  bool saveBindingsItem();

  const QuaZip* getFile(const QString& path);

  QuaZip* m_archive = Q_NULLPTR;
  QString m_filename;
  QStringList m_files;

  SharedMetadata m_metadata;
  EPubManifest m_manifest;
  EPubSpine m_spine;

  // only one container per epub.
  SharedDomDocument m_container_document;
  // might be more than one root content file.
  QMap<QString, QMap<QString, SharedDomDocument>> m_rootfiles;
  QMap<QString, SharedDomDocument> m_current_rootfile;
  // a map of dom elements within the  that might be modified.
  QMap<QString, QDomElement*> m_metadata_nodes;

  //  QMultiHash<QString, QString> metadata_old;
  //  QMultiHash<QString, QMap<QString, QString>> m_tag_attributes;
  //  ebooktoc_t m_toc;
  //  QString m_root_version;
  //  //    QHash<QString, QString> m_root_node;

  //  QHash<QString, EBookItem> m_items;
  //  QStringList m_orderedItems;
  //  QSet<QString> m_unorderedItems;

  //  QHash<EPubPageReference::StandardType, EPubPageReference>
  //      m_standardReferences;
  //  QHash<QString, EPubPageReference> m_otherReferences;
  QMimeDatabase m_mimeDatabase;
  void parseTitleMetadata(QDomElement metadata_element);
  void parseCreatorMetadata(QDomElement metadata_element);
  void parseIdentifierMetadata(QDomElement metadata_element);
  void parseLanguageMetadata(QDomElement metadata_element);
  void parseSubjectMetadata(QDomElement metadata_element);

  static const QString MIMETYPE_FILE;
  static const QByteArray MIMETYPE;
  static const QString METADATA_FOLDER;
  static const QString CONTAINER_FILE;
  static const QString TOC_FILE;

  static const QString TITLE;
  static const QString CREATOR;
  static const QString IDENTIFIER;
  static const QString LANGUAGE;
  void parseDateModified(QDomNamedNodeMap node_map, QString text);
  void saveTitles(QDomElement metadata_element);
  void saveCreator(QDomElement metadata_element);
  void saveIdentifier(QDomElement metadata_element);
};

#endif // EPUBCONTAINER_H
