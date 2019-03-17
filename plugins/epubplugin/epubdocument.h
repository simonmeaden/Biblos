#ifndef EPUBDOCUMENT_H
#define EPUBDOCUMENT_H

#include "epubcontainer.h"

#include <QDateTime>
#include <QDir>
#include <QDomDocument>
#include <QElapsedTimer>
#include <QIODevice>
#include <QImage>
#include <QObject>
#include <QPainter>
#include <QSvgRenderer>
#include <QTextBlock>
#include <QTextCursor>
#include <QTextDocument>
#include <QThread>

#include <csvsplitter/csvsplitter.h>

#include "authors.h"
#include "ebookcommon.h"
#include "epubcontainer.h"
#include "epubplugin.h"
#include "htmlparser.h"
#include "iebookdocument.h"
#include "interface_global.h"
#include "library.h"

struct EPubContents
{
  QHash<QString, QByteArray> m_svgs;
  QHash<QString, QImage> m_renderedSvgs;
  EPubContainer* m_container;
  bool m_loaded;
};

class EPubDocumentPrivate;

class INTERFACESHARED_EXPORT EPubDocument : public ITextDocument
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(EPubDocument)

public:
  EPubDocument(QObject* parent = nullptr);
  EPubDocument(EPubDocumentPrivate* doc, QObject* parent = nullptr);
  EPubDocument(const EPubDocument& doc);
  virtual ~EPubDocument() override;

  bool loaded();
  void openDocument(const QString& path) override;
  void saveDocument(const QString& path = QString()) override;
  //  void clearCache();
  EPubContents* cloneData();
  void setClonedData(EPubContents* cloneData);

  // IEBookDocument interface
  QString filename() override;
  void setFilename(const QString& filename) override;

  QString tocAsString() override;

  // IEBookDocument interface
  IEBookInterface* plugin() override;
  void setPlugin(IEBookInterface* plugin) override;
  QDate published() override;
  void setPublished(const QDate& published) override;
  QString buildTocFromData() override;

  EBookDocumentType type() const override { return EPUB; }
  bool isModified() override;
  QString title() override;
  void setTitle(const QString& title) override;
  QString subject() override;
  void setSubject(const QString& subject) override;
  QString language() override;
  void setLanguage(const QString& language) override;
  QDateTime date() override;
  void setDate(const QDateTime& date) override;
  QStringList creators() override;
  //  void setCreators(const QStringList& creators) override;
  QString creatorNames(const QStringList& authors) override;
  QString publisher() override;
  void setPublisher(const QString& publisher) override;

  Metadata metadata() override;
  QMap<QString, QString> pages() override;
  QStringList spine() override;
  QStringList cssKeys() override;
  CSSMap css() override;
  QString css(QString key) override;
  QString javascript(QString key) override;
  ManifestItem itemByHref(QString href) override;

signals:
  void orderChanged();
  void listRemoved(ItemList list);
  void wordRemoved(int index, QString word);
  void wordChanged(int index, QString old_word, QString new_word);

protected:
  EPubDocumentPrivate* d_ptr;
  EPubDocument(EPubDocumentPrivate& doc);

  bool m_modified, m_readonly;

  // IEBookDocument interface
public:
};

Q_DECLARE_METATYPE(EPubDocument);

#endif // EPUBDOCUMENT_H
