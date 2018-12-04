#ifndef EPUBDOCUMENT_P_H
#define EPUBDOCUMENT_P_H

#include <QObject>
#include <QElapsedTimer>
#include <QImage>
#include <QSvgRenderer>
#include <QPainter>
#include <QTextCursor>

#include <csvsplitter/csvsplitter.h>

#include "ebookdocument_p.h"
#include "epubdocument.h"
#include "epubcontainer.h"

#include <qlogger/qlogger.h>

class EPubDocumentPrivate : public EBookDocumentPrivate
{

public:

  EPubDocumentPrivate(EPubDocument* parent);
  virtual ~EPubDocumentPrivate();

  bool loaded();
  void openDocument(const QString& path);
  void clearCache();
  EBookContents *cloneData();
  void setClonedData(EBookContents* cloneData);

  void updateDataBlock();


protected:
  QHash<QString, QByteArray> m_svgs;
  QHash<QString, QImage> m_renderedSvgs;
  EPubContainer* m_container;
  EPubItem m_currentItem;
  bool m_loaded;

  EPubDocumentPrivate(EPubDocumentPrivate& d);
  void loadDocument();
  virtual QVariant loadResource(int, const QUrl& );
  void fixImages(QDomDocument &newDocument);
  const QImage &getSvgImage(const QString &id);

private:
  Q_DECLARE_PUBLIC(EPubDocument)

};

#endif // EPUBDOCUMENT_P_H
