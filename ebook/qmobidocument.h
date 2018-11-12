#ifndef LIBMOBI_H
#define LIBMOBI_H

#include <QDateTime>
#include <QStringList>
#include <QSharedPointer>

#include <qlogger.h>

#include <stdio.h>

#include "ebook_global.h"
#include "mobi.h"
#include "qebookdocument.h"
#include "epubcommon.h"

#define FULLNAME_MAX 1024

struct PalmHeader
{
  QString name;
  quint16 attributes;
  quint16 version;
  quint32 ctime;
  quint32 mtime;
  quint32 btime;
  quint32 mod_num;
  quint32 appinfo_offset;
  quint32 sortinfo_offset;
  QString type;
  QString creator;
  quint32 uid;
  quint32 next_rec;
  quint16 rec_count;
};

struct MobiHeader
{
  QString fullname;
  PalmHeader* palmheader;
};

class EBOOKSHARED_EXPORT QMobiDocument : public QEBookDocument
{
  Q_OBJECT

public:
  QMobiDocument(QObject* parent = Q_NULLPTR);
  QMobiDocument(const QMobiDocument& doc);
  ~QMobiDocument() override;

  bool loaded() { return m_loaded; }
  void openDocument(const QString& path);

  // EBookDocument interface
  QString title() const;
  void setTitle(const QString& title);
  QString subject() const;
  void setSubject(const QString& subject);
  QString language() const;
  void setLanguage(const QString& language);
  QDateTime date() const;
  void setDate(const QDateTime& date);
  QStringList authors() const;
  QString authorNames() const;
  void setAuthors(const QStringList& authors);
  QString publisher() const;
  void setPublisher(const QString& publisher);

  EBookContents* cloneData() override;
  void setClonedData(EBookContents*) override;

protected:
  QString m_documentPath;
  bool m_loaded;
  MOBI_RET mobi_ret;
  MOBIData* m_mobidata;
  MOBIRawml* m_mobirawdata;

  void loadDocument();
  void extractMobiData(const QString& path);
  void extractFullName(MobiHeader* header);
};

Q_DECLARE_METATYPE(QMobiDocument);

#endif // LIBMOBI_H
