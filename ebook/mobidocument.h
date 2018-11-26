#ifndef LIBMOBI_H
#define LIBMOBI_H

#include <QDateTime>
#include <QSharedPointer>
#include <QStringList>

#include <stdio.h>

#include <qlogger/qlogger.h>

#include "ebook_global.h"
#include "ebookcommon.h"
#include "ebookdocument.h"
#include "mobi.h"

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

class EBOOKSHARED_EXPORT MobiDocument : public EBookDocument
{
  Q_OBJECT

public:
  MobiDocument(QObject* parent = Q_NULLPTR);
  MobiDocument(const MobiDocument& doc);
  ~MobiDocument() override;

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

Q_DECLARE_METATYPE(MobiDocument);

#endif // LIBMOBI_H
