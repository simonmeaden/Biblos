#ifndef EPUBCONTAINER_H
#define EPUBCONTAINER_H

#include <QDomNode>
#include <QHash>
#include <QList>
#include <QMap>
#include <QMimeDatabase>
#include <QObject>
#include <QPair>
#include <QSet>
#include <QSharedPointer>
#include <QStringList>
#include <QStringLiteral>
#include <QTextStream>
#include <QVector>
#include <QtSvg>

#include <quazip5/quazip.h>
#include <quazip5/quazipfile.h>

#include "authors.h"
#include "dcterms.h"
#include "ebookcommon.h"
#include "foaf.h"
#include "library.h"
#include "marcrelator.h"

#include "epubcommon.h"
#include <ebookmetadata.h>

class QXmlStreamReader;

class EPubContainer : public QObject
{
  Q_OBJECT
public:
  explicit EPubContainer(QObject* parent = nullptr);
  ~EPubContainer();

  //  bool loadFile(const QString path);
  QString filename();
  void setFilename(QString filename);
  bool saveFile(const QString& filepath = QString());
  bool closeFile();
  //  QByteArray epubItem(const QString& id) const;
  //  QSharedPointer<QuaZipFile> zipFile(const QString& path);
  QImage image(const QString& id, QSize image_size = QSize());
  // metadata is stored in a QMultiHash to allow multiple values
  // of a key. eg. there might be more than one "creator" tag.
  QStringList itemKeys();
  SharedManifestItem item(QString key);
  QString css(QString key);
  QString javascript(QString key);
  QString itemDocument(QString key);
  QStringList spineKeys();
  QStringList imageKeys();
  QStringList cssKeys();
  QStringList jsKeys();
  QString tocAsString();
  QStringList creators();

  Metadata metadata();
  EPubManifest manifest();

  QString buildTocfromHtml();

signals:
  void errorHappened(const QString& error);

public slots:

protected:
  // Base epub data
  int m_version;
  QString m_package_xmlns;
  QString m_package_language;
  QString m_package_prefix;
  QString
    m_package_direction; // text direction - overridden  by Undicode values.
  QString m_package_id;  // unique identifier.

  // only one container per epub.
  //  QString m_container_version;
  //  QString m_container_xmlns;
  //  QString m_container_fullpath;
  //  QString m_container_mediatype;
  LibraryDB* m_library;
  EBookAuthorsDB* m_authors;

  //  bool parseMimetype();
  bool writeMimetype(QuaZip* save_zip);
  //  bool parseContainer();
  bool writeContainer(QuaZip* save_zip);
  bool parsePackageFile(QString& full_path);
  bool writePackageFile(QuaZip* save_zip);

  bool parseManifestItem(const QDomNode& manifest_node,
                         const QString current_folder);
  void extractHeadInformationFromHtmlFile(SharedManifestItem item,
                                          QString container);

  SharedSpineItem parseSpineItem(const QDomNode& metadata_element,
                                 SharedSpineItem item);
  bool saveSpineItem();
  bool parseTocFile();
  bool parseGuideItem(const QDomNode& guideItem);
  bool parseLandmarksItem(const QDomNode& guideItem);
  bool saveLandmarksItem();
  bool parseBindingsItem(const QDomNode& bindingsItem);
  bool saveBindingsItem();

  const QuaZip* getFile(const QString& path);

  QuaZip* m_archive = nullptr;
  QString m_filename;
  QStringList m_files;

  // metadata/manifest/spine etc.
  //  QByteArray m_mimetype;
  //  Metadata m_metadata;
  //  EPubManifest m_manifest;
  //  EPubSpine m_spine;

  // might be more than one root content file.
  QMap<QString, QMap<QString, DomDocument>> m_rootfiles;
  QMap<QString, DomDocument> m_current_rootfile;
  // a map of dom elements within the  that might be modified.
  QMap<QString, QDomElement*> m_metadata_nodes;
  int m_toc_chapter_index;

  SharedTocItem parseNavPoint(QDomElement navpoint,
                              QString& formatted_toc_data);

  void createAnchorPointForChapter(SharedTocItem toc_item,
                                   SharedManifestItem manifest_item);
  //  void createChapterAnchorPoints(SharedSpineItem spine_item);
  QString extractTagText(int anchor_start, QString document_string);
  void handleSubNavpoints(QDomElement navpoint, QString& formatted_toc_string);

  //  static const QString MIMETYPE_FILE;
  //  static const QByteArray MIMETYPE;
  //  static const QString METADATA_FOLDER;
  //  static const QString CONTAINER_FILE;
  //  static const QString TOC_FILE;

  //  static const QString TITLE;
  //  static const QString CREATOR;
  //  static const QString IDENTIFIER;
  //  static const QString LANGUAGE;

  //  static const QString TOC_TITLE;
  //  static const QString LIST_START;
  //  static const QString LIST_END;
  //  static const QString LIST_ITEM;
  //  static const QString LIST_BUILD_ITEM;
  //  static const QString LIST_FILEPOS;

  //  static const QString HTML_DOCTYPE;
  //  static const QString XML_HEADER;
  //  static const QString HTML_XMLNS;
  //  //  static const QString HEAD_META_CONTENT;
};

#endif // EPUBCONTAINER_H
