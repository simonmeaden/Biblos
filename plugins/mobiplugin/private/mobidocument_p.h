#ifndef MOBIDOCUMENT_P_H
#define MOBIDOCUMENT_P_H

#include <QDataStream>
#include <QFile>

#include <mobi.h>

#include "mobidocument.h"

enum ExthBinaryType {
  DRMSERVER = 1,
  DRMCOMMERCE = 2,
  DRMEBOOKBASE = 3,
  TITLE = 99,           /**< <dc:title> */
  AUTHOR = 100,         /**< <dc:creator> */
  PUBLISHER = 101,      /**< <dc:publisher> */
  IMPRINT = 102,        /**< <imprint> */
  DESCRIPTION = 103,    /**< <dc:description> */
  ISBN = 104,           /**< <dc:identifier opf:scheme="ISBN"> */
  SUBJECT = 105,        // Could appear multiple times /**< <dc:subject> */
  PUBLISHINGDATE = 106, /**< <dc:date> */
  REVIEW = 107,         /**< <review> */
  CONTRIBUTOR = 108,    /**< <dc:contributor> */
  RIGHTS = 109,         /**< <dc:rights> */
  SUBJECTCODE = 110,    /**< <dc:subject BASICCode="subjectcode"> */
  TYPE = 111,           /**< <dc:type> */
  SOURCE = 112,         /**< <dc:source> */
  ASIN = 113, // Kindle Paperwhite labels books with "Personal" if they don't
  // have this record.
  VERSION = 114,
  SAMPLE = 115, // 0x0001 if the book content is only a sample of the full book
  STARTREADING = 116, // Position (4-byte offset) in file at which to open when
  // first opened /**< Start reading */
  ADULT = 117, // Mobipocket Creator adds this if Adult only is checked on its
  // GUI; contents: "yes" /**< <adult> */
  PRICE = 118,    // As text, e.g. "4.99" /**< <srp> */
  CURRENCY = 119, // As text, e.g. "USD" /**< <srp currency="currency"> */
  KF8BOUNDARY = 121,
  FIXEDLAYOUT = 122,     /**< <fixed-layout> */
  BOOKTYPE = 123,        /**< <book-type> */
  ORIENTATIONLOCK = 124, /**< <orientation-lock> */
  COUNTRESOURCES = 125,
  ORIGRESOLUTION = 126, /**< <original-resolution> */
  ZEROGUTTER = 127,     /**< <zero-gutter> */
  ZEROMARGIN = 128,     /**< <zero-margin> */
  KF8COVERURI = 129,
  RESCOFFSET = 131,
  REGIONMAGNI = 132, /**< <region-mag> */

  DICTNAME = 200,    // As text /**< <DictionaryVeryShortName> */
  COVEROFFSET = 201, // Add to first image field in Mobi Header to find PDB
  // record containing the cover image/**< <EmbeddedCover> */
  THUMBOFFSET = 202, // Add to first image field in Mobi Header to find PDB
  // record containing the thumbnail cover image
  HASFAKECOVER = 203,
  CREATORSOFT =
    204, // Known Values: 1=mobigen, 2=Mobipocket Creator, 200=kindlegen
  // (Windows), 201=kindlegen (Linux), 202=kindlegen (Mac).
  CREATORMAJOR = 205,
  CREATORMINOR = 206,
  CREATORBUILD = 207,
  WATERMARK = 208,
  TAMPERKEYS = 209,

  FONTSIGNATURE = 300,

  CLIPPINGLIMIT =
    401, // Integer percentage of the text allowed to be clipped. Usually 10.
  PUBLISHERLIMIT = 402,
  UNK403 = 403,
  TTSDISABLE = 404, // 1 - Text to Speech disabled; 0 - Text to Speech enabled
  UNK405 = 405,     // 1 in this field seems to indicate a rental book
  RENTAL = 406,     // If this field is removed from a rental, the book says it
  // expired in 1969
  UNK407 = 407,
  UNK450 = 450,
  UNK451 = 451,
  UNK452 = 452,
  UNK453 = 453,

  DOCTYPE = 501, // PDOC - Personal Doc; EBOK - ebook; EBSP - ebook sample;
  LASTUPDATE = 502,
  UPDATEDTITLE = 503,
  ASIN504 = 504, // ?? ASIN in this record.
  TITLEFILEAS = 508,
  CREATORFILEAS = 517,
  PUBLISHERFILEAS = 522,
  LANGUAGE = 524, /**< <dc:language> */
  ALIGNMENT =
    525, // ?? horizontal-lr in this record /**< <primary-writing-mode> */
  PAGEDIR = 527,
  OVERRIDEFONTS = 528, /**< <override-kindle-fonts> */
  SORCEDESC = 529,
  DICTLANGIN = 531,
  DICTLANGOUT = 532,
  UNK534 = 534,
  CREATORBUILDREV = 535,
};

class MobiDocumentPrivate : public ITextDocumentPrivate
{
public:
  MobiDocumentPrivate(MobiDocument* parent);
  virtual ~MobiDocumentPrivate();

  MobiDocument* q_ptr;

  void openDocument(const QString& path);
  void saveDocument();

  IEBookInterface* plugin()
  {
    return m_plugin;
  }
  void setPlugin(IEBookInterface* plugin)
  {
    m_plugin = plugin;
  }

  QString title()
  {
  }
  void setTitle(QString title)
  {
  }

protected:
  bool m_loaded;
  QMultiMap<QString, QString> m_exth_tags;
  QMultiMap<ExthBinaryType, QString> m_binary;
  QMultiMap<QString, uint32_t> m_exth_numerics;

  /**< key for decryption, NULL if not set */
  unsigned char* m_drm_key;
  /**< Palmdoc database header structure or NULL if not loaded */
  MOBIPdbHeader* m_palm_header;
  /**< Record0 header structure or NULL if not loaded */
  MOBIRecord0Header* m_mobi_record_0_header;
  /**< MOBI header structure or NULL if not loaded */
  MOBIMobiHeader* m_mobi_header;
  /**< Linked list of EXTH records or NULL if not loaded */
  MOBIExthHeader* m_exth_header;
  /**< Linked list of palmdoc database records or NULL if not loaded */
  MOBIPdbRecord* m_mobi_record;

  void extractExthTags(MOBIData* mobi_data);
  void extractAllRecords(MOBIData* mobi_data);

  void parseExthTagType(MOBIExthMeta tag, char* exth_string);

private:
  Q_DECLARE_PUBLIC(MobiDocument)
};

#endif // MOBIDOCUMENT_P_H
