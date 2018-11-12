#ifndef EPUBDOCUMENT_H
#define EPUBDOCUMENT_H
#include "qepubcontainer.h"

#include <QDateTime>
#include <QImage>
#include <QObject>
#include <QTextDocument>

#include <csvsplitter.h>

#include "ebook_global.h"
#include "qebookdocument.h"
//#include "private/epubdocument_p.h"
#include "epubcommon.h"
#include "qepubcontainer.h"

class EPubDocumentPrivate;

class EBOOKSHARED_EXPORT EPubDocument : public QEBookDocument
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(EPubDocument)

public:
  EPubDocument(QObject* parent = Q_NULLPTR);
  EPubDocument(EPubDocumentPrivate* doc, QObject* parent = Q_NULLPTR);
  EPubDocument(const EPubDocument& doc);
  virtual ~EPubDocument() override;

  bool loaded();
  void openDocument(const QString& path);
  void clearCache();
  EBookContents* cloneData() override;
  void setClonedData(EBookContents* cloneData) override;

protected:
  EPubDocument(EPubDocumentPrivate& doc);
};

Q_DECLARE_METATYPE(EPubDocument);

#endif // EPUBDOCUMENT_H
