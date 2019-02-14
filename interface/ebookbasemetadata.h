#ifndef EBOOKBASEMETADATA_H
#define EBOOKBASEMETADATA_H

#include <QSharedPointer>
#include <QImage>
#include <QTextCursor>
#include <QDateTime>
#include <QDomDocument>

#include "foaf.h"
#include "dcterms.h"
#include "marcrelator.h"

typedef QSharedPointer<QDomDocument> SharedDomDocument;
typedef QSharedPointer<QImage> SharedImage;
typedef QMap<QString, SharedImage> SharedImageMap;
typedef QSharedPointer<QTextCursor> SharedTextCursor;

struct EBookModified {
  QDateTime date;
  QString id;
};

struct EBookAltRep {
  QString name;
  QString lang;
};
typedef QSharedPointer<EBookAltRep> AltRep;
typedef QList<AltRep> AltRepList;

struct EBookFileAs {
  QString name;
  QString lang;
};
typedef QSharedPointer<EBookFileAs> FileAs;
typedef QList<FileAs> FileAsList;

struct EBookBaseMetadata {
  EBookBaseMetadata();
};
typedef QSharedPointer<EBookBaseMetadata> BaseMetadata;

struct EBookTitle : public EBookBaseMetadata {
  EBookTitle()
  {
  }
  //  QString title_type;
  QString id;
  QString title;
  QString dir;
  QString lang;
  AltRepList alt_rep_list;
  FileAsList file_as_list;
  QDateTime date;
};
typedef QSharedPointer<EBookTitle> Title;
typedef QMap<QString, Title> TitleMap;
typedef QMap<int, Title> OrderedTitleMap;

class EBookLanguage
{
public:
  QString id;
  QString language;
};
typedef QSharedPointer<EBookLanguage> Language;
typedef QMap<QString, Language> LanguageMap;

class EBookSubject
{
public:
  QString id;
  QString subject;
  QString authority;
  QString term;
  QString lang;
  QString dir;
};
typedef QSharedPointer<EBookSubject> Subject;
typedef QMap<QString, Subject> SubjectMap;

class EBookIdentifierScheme
{
public:
  enum IdentifierScheme {
    DOI,
    ISBN,
    JDCN,
    UUID,
    AMAZON,
    CALIBRE,
    UNKNOWN_SCHEME,
  };

  EBookIdentifierScheme()
  {
    scheme = IdentifierScheme::UNKNOWN_SCHEME;
  }

  IdentifierScheme scheme;

  static EBookIdentifierScheme fromString(QString scheme_type)
  {
    QString type = scheme_type.toLower();
    EBookIdentifierScheme scheme;
    if (type == "doi") {
      scheme.scheme = DOI;
    } else if (type == "isbn") {
      scheme.scheme = ISBN;
    } else if (type == "jdcn") {
      scheme.scheme = JDCN;
    } else if (type == "uuid") {
      scheme.scheme = UUID;
    } else if (type == "mobi-asin") {
      scheme.scheme = AMAZON;
    } else if (type == "calibre") {
      scheme.scheme = CALIBRE;
    } else {
      scheme.scheme = UNKNOWN_SCHEME;
    }
    return scheme;
  }
  static QString toString(EBookIdentifierScheme::IdentifierScheme scheme)
  {
    switch (scheme) {
    case DOI:
      return "doi";
    case ISBN:
      return "isbn";
    case JDCN:
      return "jdcn";
    case UUID:
      return "uuid";
    case AMAZON:
      return "mobi-asin";
    case CALIBRE:
      return "calibre";
    default:
      return QString();
    }
  }
};

class EBookIdentifier
{
public:
  QString name;
  QString id;
  EBookIdentifierScheme identifier;
};
typedef QSharedPointer<EBookIdentifier> Identifier;
typedef QMap<EBookIdentifierScheme::IdentifierScheme, Identifier> IdentifierMap;

class EPubSource
{
public:
  QString source;
  QString id;
  EBookIdentifierScheme scheme;
};
typedef QSharedPointer<EPubSource> Source;
typedef QMap<QString, Source> SourceMap;

struct EPubPublisher {
  QString name;
  FileAs file_as;
  QString dir;
  QString id;
  AltRep alt_rep;
  QString lang;
};
typedef QSharedPointer<EPubPublisher> Publisher;

struct EBookRelation {
  QString name;
  QString dir;
  QString id;
  QString lang;
};
typedef QSharedPointer<EBookRelation> Relation;

struct EBookRights {
  QString name;
  QString dir;
  QString id;
  QString lang;
};
typedef QSharedPointer<EBookRights> Rights;

struct EBookCoverage {
  QString name;
  QString dir;
  QString id;
  QString lang;
};
typedef QSharedPointer<EBookCoverage> Coverage;

struct EBookFormat {
  QString name;
  QString id;
};
typedef QSharedPointer<EBookFormat> Format;

struct EBookType {
  QString name;
  QString id;
};
typedef QSharedPointer<EBookType> BookType;

class EBookCreator : public EBookBaseMetadata
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
  EBookCreator()
  {
  }

  QString name;
  FileAsList file_as_list;
  QString id;
  AltRepList alt_rep_list;
  QDateTime date;

  MarcRelator relator;
  QString string_creator;
  QString string_scheme;
  QList<Foaf> foaf_list;

  static SchemeType fromSchemeString(QString type)
  {
    if (type == "dcterms") {
      return SchemeType::dcterms;
    } else if (type == "marc") { // TODO - The only one supported at the present
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

class EBookContributor : public EBookCreator
{
  // Actually identical - convenience class
};
typedef QSharedPointer<EBookCreator> Creator;
typedef QMultiMap<QString, Creator> CreatorMap;
typedef QList<Creator> CreatorList;
typedef QSharedPointer<EBookContributor> Contributor;
typedef QMultiMap<QString, Contributor> ContributorMap;
typedef QList<Contributor> ContributorList;

struct EBookDescription {
  QString id;
  QString text;
  QString dir;
  QString language;
};
typedef QSharedPointer<EBookDescription> Description;
typedef QMultiMap<QString, Description> DescriptionMap;

struct Calibre {
  QString series_name;
  QString series_index;
  QString title_sort;
  QString author_link_map;
  QString timestamp;
  QString rating;
  QString publication_type;
  QString user_metadata;
  QString user_categories;
  QString custom_metadata;
};

#endif // EBOOKBASEMETADATA_H
