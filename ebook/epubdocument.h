#ifndef EPUBDOCUMENT_H
#define EPUBDOCUMENT_H
#include "epubcontainer.h"

#include <QDateTime>
#include <QImage>
#include <QObject>
#include <QTextDocument>

#include "ebook_global.h"
#include "ebookdocument.h"
#include <csvsplitter.h>

class EPubContainer;

class EBOOKSHARED_EXPORT EPubDocument : public EBookDocument {
  Q_OBJECT

public:
  explicit EPubDocument(QObject *parent = Q_NULLPTR);
  EPubDocument(const EPubDocument &doc);
  virtual ~EPubDocument() override;

  bool loaded() { return m_loaded; }
  void openDocument(const QString &path);
  void clearCache() { m_renderedSvgs.clear(); }

  // EBookDocument interface
  QString title() const override;
  void setTitle(const QString &title) override;
  QString subject() const override;
  void setSubject(const QString &subject) override;
  QString language() const override;
  void setLanguage(const QString &language) override;
  QDateTime date() const override;
  void setDate(const QDateTime &date) override;
  QStringList authors() const override;
  void setAuthors(const QStringList &authors) override;
  QString authorNames() const override;
  QString publisher() const override;
  void setPublisher(const QString &publisher) override;

protected:
  virtual QVariant loadResource(int type, const QUrl &url) override;

private slots:
  void loadDocument();

private:
  QHash<QString, QByteArray> m_svgs;
  QHash<QString, QImage> m_renderedSvgs;

  EPubContainer *m_container;
  EpubItem m_currentItem;

  bool m_loaded;

  void fixImages(QDomDocument &newDocument);
  const QImage &getSvgImage(const QString &id);

  // EBookDocument interface
public:
};

Q_DECLARE_METATYPE(EPubDocument);

#endif // EPUBDOCUMENT_H
