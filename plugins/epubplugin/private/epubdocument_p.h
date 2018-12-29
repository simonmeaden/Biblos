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

class EPubDocumentPrivate : public ITextDocumentPrivate
{
public:
  EPubDocumentPrivate(EPubDocument* parent);
  virtual ~EPubDocumentPrivate();

  EPubDocument* q_ptr;

  bool loaded();
  void openDocument(const QString& path);
  void saveDocument();
  void clearCache();
  EPubContents* cloneData();
  void setClonedData(EPubContents* cloneData);

  IEBookInterface* plugin() { return m_plugin; }
  void setPlugin(IEBookInterface* plugin) { m_plugin = plugin; }

  QString language() const {}
  void setLanguage(const QString& language) {}
  QDateTime date() const {}
  void setDate(const QDateTime& date) {}
  void setDocumentPath(const QString& documentPath);

protected:
  QList<EPubCreator*> m_creators;

  //  QHash<QString, QByteArray> m_svgs;
  //  QHash<QString, QImage> m_renderedSvgs;
  EPubContainer* m_container;
  EBookItem m_currentItem;
  QString m_documentPath;
  bool m_loaded;
  QString m_concatenated_authors;

  EPubDocumentPrivate(EPubDocumentPrivate& d);
  void loadDocument();
  //  virtual QVariant loadResource(int, const QUrl&);
  //  void fixImages(SharedDomDocument newDocument);
  //  const QImage& getSvgImage(const QString& id);

private:
  Q_DECLARE_PUBLIC(EPubDocument)
};

#endif // EPUBDOCUMENT_P_H
