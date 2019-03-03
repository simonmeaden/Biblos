#ifndef EPUBDOCUMENT_P_H
#define EPUBDOCUMENT_P_H

#include <QElapsedTimer>
#include <QImage>
#include <QObject>
#include <QPainter>
#include <QSvgRenderer>
#include <QTextCursor>

#include "epubcontainer.h"
#include "epubdocument.h"
#include "htmlparser.h"

class EPubDocumentPrivate : public ITextDocumentPrivate
{
public:
  EPubDocumentPrivate(EPubDocument* parent);
  virtual ~EPubDocumentPrivate();

  EPubDocument* q_ptr;

  bool loaded();
  void openDocument(const QString& path);
  void saveDocument(const QString& path = QString());
  QString filename();
  //  void clearCache();
  EPubContents* cloneData();
  void setClonedData(EPubContents* cloneData);

  QStringList creators();
  QString title();
  void setTitle(QString title);

  IEBookInterface* plugin() { return m_plugin; }
  void setPlugin(IEBookInterface* plugin) { m_plugin = plugin; }

  QString language() const {}
  void setLanguage(const QString& language) {}
  QDateTime date() const {}
  void setDate(const QDateTime& date) {}
  //  void setDocumentPath(const QString& documentPath);
  Metadata metadata();

  QString buildTocFromFiles();

  bool isModified() const;

protected:
  //  QString m_documentPath;
  bool m_loaded;
  bool m_current_document_index;
  bool m_current_document_lineno;
  EPubContainer* m_container;
  bool m_modified;

  EPubDocumentPrivate(EPubDocumentPrivate& d);
  void loadDocument();
  QString toc();
  //  virtual QVariant loadResource(int, const QUrl&);
  //  void fixImages(SharedDomDocument newDocument);
  //  const QImage& getSvgImage(const QString& id);

private:
  Q_DECLARE_PUBLIC(EPubDocument)
};

#endif // EPUBDOCUMENT_P_H
