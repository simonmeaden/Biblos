#ifndef EPUBDOCUMENT_H
#define EPUBDOCUMENT_H
#include "epubcontainer.h"

#include <QDateTime>
#include <QImage>
#include <QObject>
#include <QTextDocument>

#include <csvsplitter/csvsplitter.h>

#include "iebookdocument.h"
#include "interface_global.h"
//#include "private/epubdocument_p.h"
#include "ebookcommon.h"
#include "epubcontainer.h"
#include "epubplugin.h"

struct EPubContents {
  QHash<QString, QByteArray> m_svgs;
  QHash<QString, QImage> m_renderedSvgs;
  EPubContainer* m_container;
  EBookItem m_currentItem;
  bool m_loaded;
};

class EPubDocumentPrivate;

class INTERFACESHARED_EXPORT EPubDocument : public ITextDocument
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(EPubDocument)

public:
  EPubDocument(QObject* parent = Q_NULLPTR);
  EPubDocument(EPubDocumentPrivate* doc, QObject* parent = Q_NULLPTR);
  EPubDocument(const EPubDocument& doc);
  virtual ~EPubDocument() override;

  bool loaded();
  void openDocument(const QString& path) override;
  void clearCache();
  EPubContents* cloneData();
  void setClonedData(EPubContents* cloneData);

  // IEBookDocument interface
  QString filename() const override;
  void setFilename(const QString& filename) override;
  IEBookInterface* plugin() const override;
  void setPlugin(IEBookInterface* plugin) override;

  bool isModified() const override;
  bool readOnly() const override;
  void setReadOnly(const bool readonly) override;

  QString title() override;
  void setTitle(const QString& title) override;
  QString subject() override;
  void setSubject(const QString& subject) override;
  QString language() override;
  void setLanguage(const QString& language) override;
  QDateTime date() override;
  void setDate(const QDateTime& date) override;
  QStringList authors() override;
  void setAuthors(const QStringList& authors) override;
  QString authorNames() override;
  QString publisher() override;
  void setPublisher(const QString& publisher) override;

protected:
  EPubDocumentPrivate* d_ptr;
  EPubDocument(EPubDocumentPrivate& doc);

  QString m_filename;
  IEBookInterface* m_plugin;
  bool m_modified, m_readonly;
};

Q_DECLARE_METATYPE(EPubDocument);

#endif // EPUBDOCUMENT_H
