#ifndef EPUBDOCUMENT_H
#define EPUBDOCUMENT_H

#include <QImage>
#include <QObject>
#include <QTextDocument>
#include <QDateTime>

#include <ebook_global.h>
#include <csvsplitter.h>
#include "epubcontainer.h"

class EPubContainer;

class EBOOKSHARED_EXPORT EPubDocument : public QTextDocument {
  Q_OBJECT

public:
  explicit EPubDocument(QObject *parent = Q_NULLPTR);
  virtual ~EPubDocument();

  bool loaded() { return m_loaded; }
  void openDocument(const QString &path);
  void clearCache() { m_renderedSvgs.clear(); }

  QString title() const;
  void setTitle(const QString &title);
  QString subject() const;
  void setSubject(const QString &subject);
  QString language() const;
  void setLanguage(const QString &language);
  QDateTime date() const;
  void setDate(const QDateTime &date);
  QStringList authors() const;
  void setAuthors(const QStringList &authors);
  QString publisher() const;
  void setPublisher(const QString &publisher);

signals:
  void loadCompleted();

protected:
  virtual QVariant loadResource(int type, const QUrl &url) override;

private slots:
  void loadDocument();

private:
  QHash<QString, QByteArray> m_svgs;
  QHash<QString, QImage> m_renderedSvgs;

  QString m_documentPath;
  EPubContainer *m_container;
  EpubItem m_currentItem;

  bool m_loaded;

  void fixImages(QDomDocument &newDocument);
  const QImage &getSvgImage(const QString &id);
};

#endif // EPUBDOCUMENT_H
