#include "mobidocument_p.h"

//#include <QLoggingCategory>
//#include <qlogger/qlogger.h>
// using namespace qlogger;

#include "logging.h"

MobiDocumentPrivate::MobiDocumentPrivate(MobiDocument* parent)
  : q_ptr(parent)
  , m_loaded(false)
{}

MobiDocumentPrivate::~MobiDocumentPrivate() {}

void
MobiDocumentPrivate::parseExthTagType(MOBIExthMeta tag, char* exth_string)
{
  QString t(tag.name);
  QString s(exth_string);

  switch (tag.tag) {
    case EXTH_TITLE:
      m_title = s;
      break;
    case EXTH_AUTHOR:
      m_creators += s;
      break;
    case EXTH_PUBLISHER:
      m_publisher = s;
      break;
    case EXTH_PUBLISHINGDATE:
      m_published = QDate::fromString(s, Qt::ISODate);
      break;
    default:
      // unknown tags ?
      m_exth_tags.insert(t, s);
      break;
  }
}

void
MobiDocumentPrivate::extractExthTags(MOBIData* mobi_data)
{
  const MOBIExthHeader* curr = m_exth_header;
  uint32_t val32;
  while (curr) {
    /* check if it is a known tag and get some more info if it is */
    MOBIExthMeta tag = mobi_get_exthtagmeta_by_tag(MOBIExthTag(curr->tag));
    if (tag.tag == 0) {
      /* unknown tag */
      /* try to print the record both as string and numeric value */
      char* str = new char[curr->size + 1];
      if (!str) {
        printf("Memory allocation failed\n");
        exit(1);
      }
      unsigned i = 0;
      unsigned char* p = static_cast<unsigned char*>(curr->data);
      while (i < curr->size && isprint(*p)) {
        str[i] = static_cast<char>(*p++);
        i++;
      }
      str[i] = '\0';
      val32 = mobi_decode_exthvalue(static_cast<unsigned char*>(curr->data),
                                    curr->size);
      printf("Unknown (%i): %s (%u)\n", curr->tag, str, val32);
      free(str);
    } else {
      /* known tag */
      unsigned i = 0;
      size_t size = curr->size;
      unsigned char* data = static_cast<unsigned char*>(curr->data);
      switch (tag.type) {
          /* numeric */
        case EXTH_NUMERIC:
          val32 = mobi_decode_exthvalue(data, size);
          m_exth_numerics.insert(tag.name, val32);
          break;
          /* string */
        case EXTH_STRING: {
          char* exth_string = mobi_decode_exthstring(mobi_data, data, size);
          if (exth_string) {
            parseExthTagType(tag, exth_string);
            free(exth_string);
          }
          break;
        }
          /* binary */
        case EXTH_BINARY: {
          char* exth_string = new char[2 * size + 1];
          if (!exth_string) {
            continue;
          } else {
            exth_string[0] = '\0';
            while (size) {
              uint8_t val8 = *data++;
              sprintf(&exth_string[i], "%02x", val8);
              i += 2;
              size--;
            }

            if (tag.tag == EXTH_FONTSIGNATURE) {
              QString t(tag.name);
              QString s(exth_string);
              m_binary.insert(ExthBinaryType::FONTSIGNATURE, s);
            }
          }
          free(exth_string);
          break;
        }
      }
    }
    curr = curr->next;
  }
}

void
MobiDocumentPrivate::openDocument(const QString& path)
{
  Q_Q(MobiDocument);

  m_filename = path;

  MOBIData* mobi_data = mobi_init();
  if (mobi_data == nullptr) {
    qCDebug(LOG_MOBI) << QString("Unable to create MOBI_DATA object");
  }

  MOBI_RET mobi_ret = mobi_load_filename(mobi_data, path.toStdString().c_str());
  if (mobi_ret != MOBI_SUCCESS) {
    mobi_free(mobi_data);
    qCDebug(LOG_MOBI) << QString("Unable to read mobi document");
  }

  MOBIRawml* rawml = mobi_init_rawml(mobi_data);
  if (rawml == nullptr) {
    mobi_free(mobi_data);
    qCDebug(LOG_MOBI) << QString("Unable to read mobi document");
  }

  mobi_ret = mobi_parse_rawml(rawml, mobi_data);
  if (mobi_ret != MOBI_SUCCESS) {
    mobi_free(mobi_data);
    mobi_free_rawml(rawml);
    qCDebug(LOG_MOBI) << QString("Unable to read mobi document");
  }

  m_drm_key = mobi_data->drm_key;
  m_palm_header = mobi_data->ph;
  m_mobi_record_0_header = mobi_data->rh;
  m_mobi_header = mobi_data->mh;
  m_exth_header = mobi_data->eh;
  m_mobi_record = mobi_data->rec;

  m_title = m_mobi_header->full_name;

  extractExthTags(mobi_data);
  extractAllRecords(mobi_data);

  /* Free MOBIRawml structure */
  mobi_free_rawml(rawml);
  /* Free MOBIData structure */
  mobi_free(mobi_data);
}

void
MobiDocumentPrivate::saveDocument()
{
  // TODO save mobidocument
}

void
MobiDocumentPrivate::extractAllRecords(MOBIData* mobi_data)
{
  Q_Q(MobiDocument);
  const size_t maxlen = mobi_get_text_maxsize(mobi_data);
  size_t size = maxlen;
  char* data = new char[maxlen];
  MOBI_RET mobi_ret = mobi_get_rawml(mobi_data, data, &size);

  if (mobi_ret != MOBI_SUCCESS) {
    // TODO logging
  }

  QString full_data(data);
  delete[] data;

  //  QTextCursor cursor(q_ptr);
  //  cursor.movePosition(QTextCursor::End);
  //  QTextBlockFormat pageBreak;
  //  pageBreak.setPageBreakPolicy(QTextFormat::PageBreak_AlwaysBefore);
  //  cursor.insertHtml(full_data);
  //  cursor.insertBlock(pageBreak);
  //  q->setBaseUrl(QUrl());
  m_loaded = true;

  emit q->loadCompleted();
}
