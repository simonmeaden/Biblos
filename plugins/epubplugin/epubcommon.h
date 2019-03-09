#ifndef EPUBMANIFESTITEM_H
#define EPUBMANIFESTITEM_H

#include <QList>
#include <QSharedPointer>

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

#endif // EPUBMANIFESTITEM_H
