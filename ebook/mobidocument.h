#ifndef LIBMOBI_H
#define LIBMOBI_H

#include <QDateTime>
#include <QStringList>

#include <qlogger.h>

#include "ebook_global.h"
#include "ebookdocument.h"
#include "mobi.h"

class EBOOKSHARED_EXPORT MobiDocument : public EBookDocument {
  Q_OBJECT

public:
  MobiDocument(QObject *parent = Q_NULLPTR);
  MobiDocument(const MobiDocument &doc);
  ~MobiDocument();

  bool loaded() { return m_loaded; }
  void openDocument(const QString &path);

  // EBookDocument interface
  QString title() const;
  void setTitle(const QString &title);
  QString subject() const;
  void setSubject(const QString &subject);
  QString language() const;
  void setLanguage(const QString &language);
  QDateTime date() const;
  void setDate(const QDateTime &date);
  QStringList authors() const;
  QString authorNames() const;
  void setAuthors(const QStringList &authors);
  QString publisher() const;
  void setPublisher(const QString &publisher);

private slots:
  void loadDocument();

protected:
  QString m_documentPath;
  bool m_loaded;
  MOBIData *m_data;
  MOBIRawml *m_rawdata;

};

Q_DECLARE_METATYPE(MobiDocument);

#endif // LIBMOBI_H
