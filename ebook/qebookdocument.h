#ifndef EBOOKDOCUMENT_H
#define EBOOKDOCUMENT_H

#include <QDateTime>
#include <QTextDocument>

#include "ebook_global.h"
#include "epubcommon.h"
#include "private/qebookdocument_p.h"

class EBOOKSHARED_EXPORT QEBookDocument : public QTextDocument
{
  Q_OBJECT

public:
  QEBookDocument(QObject* parent = Q_NULLPTR);
  QEBookDocument(QEBookDocumentPrivate* doc, QObject* parent = Q_NULLPTR);
  QEBookDocument(const QEBookDocument& doc);
  ~QEBookDocument();

  bool isModified();
  void setModified(bool modified);
  bool readonly() const;
  void setReadonly(bool readonly);

signals:
  void loadCompleted();

public:
  EBookData* data();

  QString title() const;
  void setTitle(const QString& title);
  QString subject() const;
  void setSubject(const QString& subject);
  QString language() const;
  void setLanguage(const QString& language);
  QDateTime date() const;
  void setDate(const QDateTime& date);
  QStringList authors() const;
  void setAuthors(const QStringList& authors);
  QString authorNames() const;
  QString publisher() const;
  void setPublisher(const QString& publisher);

  QString documentPath() const;
  virtual EBookContents* cloneData() { return new EBookContents(); }
  virtual void setClonedData(EBookContents* /*cloneData*/) {}

protected:
  QEBookDocumentPrivate* const d_ptr;
  QEBookDocument(QEBookDocumentPrivate& d);

  void setDocumentPath(const QString& path);

private:
  Q_DECLARE_PRIVATE(QEBookDocument)
};

Q_DECLARE_METATYPE(QEBookDocument);

#endif // EBOOKDOCUMENT_H
