#ifndef EPUBDOCUMENT_P_H
#define EPUBDOCUMENT_P_H

#include <QObject>
#include <QElapsedTimer>
#include <QImage>
#include <QSvgRenderer>
#include <QPainter>
#include <QTextCursor>

#include "csvsplitter.h"

#include "qebookdocument_p.h"
#include "qepubdocument.h"
#include "qepubcontainer.h"

#include <qlogger.h>


class EPubDocumentPrivate : public QEBookDocumentPrivate
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
  QEPubContainer* m_container;
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
