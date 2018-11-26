#include "mobidocument.h"

using namespace qlogger;

MobiDocument::MobiDocument(QObject* parent)
  : EBookDocument(parent)
  , m_loaded(false)
  , m_mobidata(Q_NULLPTR)
{}

MobiDocument::MobiDocument(const MobiDocument& doc)
  : EBookDocument(doc.parent())
{}

MobiDocument::~MobiDocument()
{
  //  if (m_mobirawdata) {
  //    mobi_free_rawml(m_mobirawdata);
  //  }
  //  if (m_mobidata) {
  //    mobi_free(m_mobidata);
  //  }
}

void
MobiDocument::extractFullName(MobiHeader* /*header*/)
{
  //  if (m_mobidata->mh && m_mobidata->mh->full_name) {
  //    char fullname[FULLNAME_MAX + 1];
  //    if (mobi_get_fullname(m_mobidata, fullname, FULLNAME_MAX) ==
  //    MOBI_SUCCESS) {
  //      header->fullname = QString(fullname);
  //    }
  //  }
}

void
MobiDocument::extractMobiData(const QString& /*path*/)
{
  //  m_mobidata = mobi_init();

  //  if (!m_mobidata) {
  //    QLOG_WARN(QString("Unable to create mobi data object."));
  //  }

  //  mobi_ret =
  //    mobi_load_filename(m_mobidata, path.toStdString().c_str());
  //  if (mobi_ret != MOBI_SUCCESS) {
  //    mobi_free(m_mobidata);
  //    if (mobi_ret == MOBI_FILE_NOT_FOUND) {
  //      QLOG_WARN(QString("Unable to load mobi file."));
  //    } else if (mobi_ret == MOBI_FILE_ENCRYPTED) {
  //      QLOG_WARN(QString("Mobi file was encrypted."));
  //    } else if (mobi_ret == MOBI_FILE_UNSUPPORTED) {
  //      QLOG_WARN(QString("Mobi file format is not supported."));
  //    } else if (mobi_ret == MOBI_DATA_CORRUPT) {
  //      QLOG_WARN(QString("File was corrupted."));
  //    } else if (mobi_ret == MOBI_DRM_PIDINV ||
  //               mobi_ret == MOBI_DRM_KEYNOTFOUND ||
  //               mobi_ret == MOBI_DRM_UNSUPPORTED) {
  //      QLOG_WARN(QString("Mobi is DRM'd."));
  //    }
  //  }
}

void
MobiDocument::openDocument(const QString& /*path*/)
{
  //  extractMobiData(path);

  //  m_mobirawdata = mobi_init_rawml(m_mobidata);
  //  if (!m_mobirawdata) {
  //    mobi_free(m_mobidata);
  //    QLOG_WARN(QString("Unable to create raw mobi data object."));
  //  }

  //  mobi_ret = mobi_parse_rawml(m_mobirawdata, m_mobidata);
  //  if (mobi_ret != MOBI_SUCCESS) {
  //    mobi_free(m_mobidata);
  //    mobi_free_rawml(m_mobirawdata);
  //    QLOG_WARN(QString("Unable to parse mobi file."));
  //  }

  //  MobiHeader* header;
  //  extractFullName(header);

  //  if (m_mobidata->ph) {
  //    char name[33];
  //    strcpy(name, m_mobidata->ph->name);
  //    header->palmheader->name = QString::fromLatin1(name);

  //    //     printf("name: %s\n", m->ph->name);
  //    //     printf("attributes: %hu\n", m->ph->attributes);
  //    //     printf("version: %hu\n", m->ph->version);
  //    //     struct tm * timeinfo = mobi_pdbtime_to_time(m->ph->ctime);
  //    //     printf("ctime: %s", asctime(timeinfo));
  //    //     timeinfo = mobi_pdbtime_to_time(m->ph->mtime);
  //    //     printf("mtime: %s", asctime(timeinfo));
  //    //     timeinfo = mobi_pdbtime_to_time(m->ph->btime);
  //    //     printf("btime: %s", asctime(timeinfo));
  //    //     printf("mod_num: %u\n", m->ph->mod_num);
  //    //     printf("appinfo_offset: %u\n", m->ph->appinfo_offset);
  //    //     printf("sortinfo_offset: %u\n", m->ph->sortinfo_offset);
  //    //     printf("type: %s\n", m->ph->type);
  //    //     printf("creator: %s\n", m->ph->creator);
  //    //     printf("uid: %u\n", m->ph->uid);
  //    //     printf("next_rec: %u\n", m->ph->next_rec);
  //    //     printf("rec_count: %u\n", m->ph->rec_count);
  //  }
}

QString
MobiDocument::title() const
{
  return QString();
}

void
MobiDocument::setTitle(const QString& /*title*/)
{}

QString
MobiDocument::subject() const
{
  return QString();
}

void
MobiDocument::setSubject(const QString& /*subject*/)
{}

QString
MobiDocument::language() const
{
  return QString();
}

void
MobiDocument::setLanguage(const QString& /*language*/)
{}

QDateTime
MobiDocument::date() const
{
  return QDateTime();
}

void
MobiDocument::setDate(const QDateTime& /*date*/)
{}

QStringList
MobiDocument::authors() const
{
  return QStringList();
}

void
MobiDocument::setAuthors(const QStringList& /*authors*/)
{}

QString
MobiDocument::authorNames() const
{
  return QString();
}

QString
MobiDocument::publisher() const
{
  return QString();
}

void
MobiDocument::setPublisher(const QString& /*publisher*/)
{}

EBookContents*
MobiDocument::cloneData()
{
  return new EBookContents();
}

void
MobiDocument::setClonedData(EBookContents* /*cloneData*/)
{}

void
MobiDocument::loadDocument()
{}
