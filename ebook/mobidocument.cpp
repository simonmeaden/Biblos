#include "mobidocument.h"

using namespace qlogger;

MobiDocument::MobiDocument(QObject *parent)
  : EBookDocument(parent), m_loaded(false) {}

MobiDocument::MobiDocument(const MobiDocument &doc) {}

MobiDocument::~MobiDocument() {
  if (m_data) {
    mobi_free(m_data);
  }
  if (m_rawdata) {
    mobi_free_rawml(m_rawdata);
  }
}

void MobiDocument::openDocument(const QString &path) {
//  m_data = mobi_init();

//  if (!m_data) {
//    QLOG_WARN(QString("Unable to create mobi data object."));
//  }

//  MOBI_RET mobi_ret = mobi_load_filename(m_data, path.toStdString().c_str());
//  if (mobi_ret != MOBI_SUCCESS) {
//    mobi_free(m_data);
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

//  m_rawdata = mobi_init_rawml(m_data);
//  if (m_rawdata) {
//    mobi_free(m_data);
//    QLOG_WARN(QString("Unable to create raw mobi data object."));
//  }

//  mobi_ret = mobi_parse_rawml(m_rawdata, m_data);
//  if (mobi_ret != MOBI_SUCCESS) {
//    mobi_free(m_data);
//    mobi_free_rawml(m_rawdata);
//    QLOG_WARN(QString("Unable to parse mobi file."));
//  }

}

QString MobiDocument::title() const { return QString(); }

void MobiDocument::setTitle(const QString &title) {}

QString MobiDocument::subject() const { return QString(); }

void MobiDocument::setSubject(const QString &subject) {}

QString MobiDocument::language() const { return QString(); }

void MobiDocument::setLanguage(const QString &language) {}

QDateTime MobiDocument::date() const { return QDateTime(); }

void MobiDocument::setDate(const QDateTime &date) {}

QStringList MobiDocument::authors() const { return QStringList(); }

void MobiDocument::setAuthors(const QStringList &authors) {}

QString MobiDocument::authorNames() const { return QString(); }

QString MobiDocument::publisher() const { return QString(); }

void MobiDocument::setPublisher(const QString &publisher) {}

void MobiDocument::loadDocument() {}
