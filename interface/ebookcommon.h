#ifndef EBOOKCOMMON_H
#define EBOOKCOMMON_H

#include <QDateTime>
#include <QFont>
#include <QHash>
#include <QImage>
#include <QMap>
#include <QObject>
#include <QPair>
#include <QPoint>
#include <QSharedPointer>
#include <QTextCursor>
#include <QTextDocument>
#include <QtPlugin>

struct EBookNavPoint
{
  EBookNavPoint(QString classname, QString id, QString label, QString src)
  {
    this->classname = classname;
    this->id = id;
    this->label = label;
    this->src = src;
  }
  QString classname;
  QString id;
  QString label;
  QString src;
};
typedef QSharedPointer<EBookNavPoint> NavPoint;

enum DocumentType
{
  PLAINTEXT,
  HTML
};
enum EBookDocumentType
{
  UNSUPPORTED_TYPE,
  EPUB,
  MOBI,
  AZW,
  PDF,
};

struct EBookManifestItem
{
  QString href;
  QString path;
  QString document_string;
  QStringList css_links;
  QString body_class;
  QString id;
  QByteArray media_type;
  QStringList properties;
  QString fallback;
  QString media_overlay;
  QMap<QString, QString> non_standard_properties;
};
typedef QSharedPointer<EBookManifestItem> ManifestItem;
typedef QMap<QString, ManifestItem> ManifestItemMap;
typedef QList<ManifestItem> ManifestItemList;

class EBookTocItem;
typedef QSharedPointer<EBookTocItem> TocItem;
typedef QMap<int, TocItem> TocItemMap;
typedef QMap<QString, TocItem> TocItemPathMap;
class EBookTocItem
{
public:
  QString id;
  int playorder;
  QString tag_class;
  QString label;
  QString source;
  TocItemMap sub_items;
  QString chapter_tag;
};

typedef QSharedPointer<QMap<QString, QString>> CSSMap;
class EBookManifest
{
public:
  EBookManifest();
  ~EBookManifest();
  QString id;
  ManifestItem cover_image;      // 0 or 1
  ManifestItem nav;              // 1
  ManifestItemMap items_by_id;   // all items
  ManifestItemMap items_by_href; // all items
  ManifestItemList html_items;
  ManifestItemMap mathml;                    // subset of items for math markup
  ManifestItemMap svg_images;                // subset of items for images
  QMap<QString, QImage> rendered_svg_images; // rendered svg images
  QMap<QString, QImage> images;
  ManifestItemMap remotes;
  ManifestItemMap scripted;
  ManifestItemMap switches;
  //  QMap<QString, QString> id_by_file;          // all items
  CSSMap css;                        // all items
  QMap<QString, QString> javascript; // all items
  ManifestItemMap fonts;             // all items
  ManifestItemMap media_overlay;     // all items
  QString formatted_toc_string;
  TocItemMap toc_items;
  TocItemPathMap toc_paths;
};

class EBookSpineItem
{
public:
  EBookSpineItem()
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
typedef QSharedPointer<EBookSpineItem> SpineItem;
typedef QMap<QString, SpineItem> SpineItemMap;
typedef QStringList SpineItemList;

class EBookSpine
{
public:
  QString id;
  QString toc;
  QString page_progression_dir;
  SpineItemMap items;
  SpineItemList ordered_items;
};

#endif // EBOOKCOMMON_H
