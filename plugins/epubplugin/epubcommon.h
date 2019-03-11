#ifndef EPUBMANIFESTITEM_H
#define EPUBMANIFESTITEM_H

#include <QList>
#include <QSharedPointer>

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
typedef QSharedPointer<EPubGuideItem> GuideItem;
typedef QMap<QString, GuideItem> GuideItemMap;
typedef QStringList GuideItemList;

#endif // EPUBMANIFESTITEM_H
