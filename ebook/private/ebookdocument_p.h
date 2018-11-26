#ifndef EBOOKDOCUMENTPRIVATE_H
#define EBOOKDOCUMENTPRIVATE_H

#include <QDateTime>
#include <QObject>
#include <QPoint>

#include "../ebookcommon.h"

class EBookDocument;

class EBookDocumentPrivate
{
  Q_DECLARE_PUBLIC(EBookDocument)

public:
  EBookDocumentPrivate(EBookDocument* parent);
  virtual ~EBookDocumentPrivate();

  EBookDocument* const q_ptr;

  bool m_modified;
  bool m_readonly;
  QString m_documentPath;
  EBookData *m_data;

  EBookData *data();
  QString title();
  void setTitle(const QString& title);
  QString subject();
  void setSubject(const QString& subject);
  QString language();
  void setLanguage(const QString& language);
  QDateTime date();
  void setDate(const QDateTime& date);
  QStringList authors();
  void setAuthors(const QStringList& authors);
  QString authorNames();
  QString publisher();
  void setPublisher(const QString& publisher);

  QString documentPath() const;
  void setDocumentPath(const QString &documentPath);
  bool isModified();
  void setModified(bool modified);
  bool readonly() const;
  void setReadonly(bool readonly);

};

#endif // EBOOKDOCUMENTPRIVATE_H
