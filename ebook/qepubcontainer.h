#ifndef EPUBCONTAINER_H
#define EPUBCONTAINER_H

#include <QDomNode>
#include <QHash>
#include <QMap>
#include <QMimeDatabase>
#include <QObject>
#include <QPair>
#include <QSet>
#include <QStringList>
#include <QVector>
#include <QList>
#include <QSharedPointer>

#include <quazip/quazip.h>
#include <quazip/quazipfile.h>

#include "epubcommon.h"

class QXmlStreamReader;

class QEPubContainer : public QObject
{
  Q_OBJECT
public:
  explicit QEPubContainer(QObject* parent = Q_NULLPTR);
  ~QEPubContainer();

  bool openFile(const QString path);
  bool saveFile();
  bool closeFile();
  EPubItem epubItem(const QString& id) const { return m_items.value(id); }
  QSharedPointer<QuaZipFile> zipFile(const QString& path);
  QImage image(const QString& id);
  QString metadata(const QString& key);
  void setMetadata(const QString& key, const QString& value);
  QStringList items() { return m_orderedItems; }
  QString standardPage(EPubPageReference::StandardType type)
  {
    return m_standardReferences.value(type).target;
  }
  epubtoc_t toc();

signals:
  void errorHappened(const QString& error);

public slots:

protected:
  bool parseMimetype(QStringList& files);
  bool parseContainer(QStringList& files);
  bool parseContentFile(const QString filepath);
  bool parseToc(QStringList& files);
  bool writeToc();
  bool parseMetadataItem(const QDomNode& metadataNode);
  bool parseManifestItem(const QDomNode& manifestNodes,
                         const QString currentFolder);
  bool parseSpineItem(const QDomNode& spineNode);
  bool parseGuideItem(const QDomNode& guideItem);

  static const QString MIMETYPE_FILE, METADATA_FOLDER, CONTAINER_FILE, TOC_FILE;

  const QuaZip* getFile(const QString& path);

  QuaZip* m_archive = Q_NULLPTR;
  QString m_filename;

  QHash<QString, QString> m_metadata;
  QHash<QString, QMap<QString, QString>> m_othermetatags;
  epubtoc_t m_toc;

  QHash<QString, EPubItem> m_items;
  QStringList m_orderedItems;
  QSet<QString> m_unorderedItems;

  QHash<EPubPageReference::StandardType, EPubPageReference>
    m_standardReferences;
  QHash<QString, EPubPageReference> m_otherReferences;
  QMimeDatabase m_mimeDatabase;
};

#endif // EPUBCONTAINER_H
