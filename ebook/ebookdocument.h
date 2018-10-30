#ifndef EBOOKDOCUMENT_H
#define EBOOKDOCUMENT_H

#include <QDateTime>
#include <QTextDocument>

#include "ebook_global.h"

struct EBookData {
  QPoint position;
  QString title;
  QString subject;
  QString language;
  QString author_names;
  QStringList authors;
  QDateTime date;
  QString publisher;
};

class EBOOKSHARED_EXPORT EBookDocument : public QTextDocument {
  Q_OBJECT

public:
  EBookDocument(QObject *parent = Q_NULLPTR);
  EBookDocument(EBookDocument &doc);
  ~EBookDocument();

  bool isModified();
  void setModified(bool modified);
  bool readonly() const;
  void setReadonly(bool readonly);

signals:
  void loadCompleted();

public:
  EBookData data();
  // Interface stuff - must be implenmented in children
  virtual QString title() const = 0;
  virtual void setTitle(const QString &title) = 0;
  virtual QString subject() const = 0;
  virtual void setSubject(const QString &subject) = 0;
  virtual QString language() const = 0;
  virtual void setLanguage(const QString &language) = 0;
  virtual QDateTime date() const = 0;
  virtual void setDate(const QDateTime &date) = 0;
  virtual QStringList authors() const = 0;
  virtual void setAuthors(const QStringList &authors) = 0;
  virtual QString authorNames() const = 0;
  virtual QString publisher() const = 0;
  virtual void setPublisher(const QString &publisher) = 0;

  QString documentPath() const;

protected:
  bool m_modified;
  bool m_readonly;
  QString m_documentPath;
  EBookData m_data;
};

Q_DECLARE_METATYPE(EBookData);

#endif // EBOOKDOCUMENT_H
